/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
*  File Name    : H_T_Sensor_RL78_Training.c
*  Description  : Main Program
*  Creation Date: 2025-02-13
*  This file was generated by Smart Configurator.
***********************************************************************************************************************/
#include "r_smc_entry.h"
#include "r_rfd_common_api.h"
#include "r_rfd_common_control_api.h"
#include "r_rfd_data_flash_api.h"
#include "G23_FPB_data_flash.h"

/* Including the APIs for the HS4001 Sensor Module */
#include "r_hs400x_if.h"
#include "r_comms_i2c_if.h"
#include "r_bsp_config.h"

/* Define the States for the State Machine */
typedef enum {
    STATE_WAITING, // State when the Machine is in Idle and the Timer is Running
	STATE_WAITING_FOR_HT_CALC,
    STATE_READ_HT_SENSOR_START,
	STATE_READ_HT_SENSOR_READ,
	STATE_READ_HT_SENSOR_CALC,
    STATE_READ_INTERNAL_TEMPERATURE,
	STATE_STORE_VALUE,
	STATE_PRINT_DATA,
	STATE_COMPARE,
	STATE_ENTERING_LPM,
	STATE_ESCAPING_LPM,
	STATE_ERROR
} state_t;

// Necessary for data flash access
#define WRITE_BUFFER_SIZE_FLASH 	6U
#define DF_WRITE_START_ADDRESS    	(0x000F1000uL)
#define WRITE_BUFFER_SIZE_UART		50U
/* Define global variables */
static rm_hs400x_raw_data_t             gs_hs400x_raw_data;
static volatile rm_hs400x_data_t        gs_hs400x_data;
static fsp_err_t						err;
state_t									g_currentState = STATE_WAITING;
extern bool								g_interrupt_flag_ADC;
extern bool								g_interrupt_flag_UART;
extern bool								g_interrupt_flag_USRSW;
uint16_t								g_temperature_internal[1] = {0};
uint8_t									g_dataFlash_blockNumber = 0;
st_rtc_counter_value_t					g_rtc_counterValue;


int main (void);
void g_comms_i2c_bus0_quick_setup(void);
void g_hs400x_sensor0_quick_setup(void);
void timer_callback(void);
void external_button_callback();
uint8_t bcd_to_decimal(uint8_t bcd);

/* Quick setup for g_comms_i2c_bus0. */
void g_comms_i2c_bus0_quick_setup(void)
{
/* bus has been opened by startup procees */
}

/* Quick setup for g_hs400x_sensor0. */
void g_hs400x_sensor0_quick_setup(void)
{
	/* Open HS400X sensor instance, this must be done before calling any HS400X API */
	err = RM_HS400X_Open(g_hs400x_sensor0.p_ctrl, g_hs400x_sensor0.p_cfg);
	if(err != FSP_SUCCESS)
	{
		g_currentState = STATE_ERROR;
	}
}

/* Callback Functions*/
void hs400x_user_i2c_callback0(rm_hs400x_callback_args_t * p_args)
{
	if (RM_HS400X_EVENT_SUCCESS == p_args->event)
	    {
			switch(g_currentState)
			{
			case STATE_WAITING:
				g_currentState = STATE_READ_HT_SENSOR_READ;
				break;
			case STATE_WAITING_FOR_HT_CALC:
				g_currentState = STATE_READ_HT_SENSOR_CALC;
				break;
			}
	    }
	    else
	    {
	    	g_currentState = STATE_ERROR;
	    }
}

void timer_callback()
{
	g_currentState = STATE_READ_HT_SENSOR_START;
	PIN_WRITE(LED1) = ~PIN_WRITE(LED1);
}

void external_button_callback()
{
	g_currentState = (g_currentState == STATE_ENTERING_LPM) ? STATE_ESCAPING_LPM : STATE_ENTERING_LPM;
}

int main(void)
{
	EI();

	/* Open the Bus */
	g_comms_i2c_bus0_quick_setup();
	/* Open HS400X */
	g_hs400x_sensor0_quick_setup();

	// Start the Timer
	R_Config_TAU0_0_Start();

	// Prepare the ADC for conversion
	R_Config_ADC_Set_OperationOn();

	// ====Setting up the DataFlash access====
	uint32_t       l_mclk_freq;
	e_rfd_ret_t    l_e_rfd_status_flag;
	e_sample_ret_t l_e_sam_ret_status;

	l_mclk_freq = R_BSP_GetFclkFreqHz()/1000000;

	// Initialize RFD library with the CPU frequency
	l_e_rfd_status_flag = R_RFD_Init((uint8_t)l_mclk_freq);
	if (R_RFD_ENUM_RET_STS_OK != l_e_rfd_status_flag)
		g_currentState = STATE_ERROR;
	// Set the flash sequencer in Data flash memory programming mode
	l_e_rfd_status_flag = R_RFD_SetFlashMemoryMode(R_RFD_ENUM_FLASH_MODE_DATA_PROGRAMMING);
	if (R_RFD_ENUM_RET_STS_OK != l_e_rfd_status_flag)
			g_currentState = STATE_ERROR;

	// Config UART
	R_Config_UARTA1_Create();

	// Configure the RTC
	R_Config_RTC_Create();
	R_Config_RTC_Start();

	// Configure the external Interrupt
	R_Config_INTC_Create();
	R_Config_INTC_INTP0_Start();


	//=======================================
	MD_STATUS g_uarta1_tx_end = 0U;     /* uartA0 transmission end */

	// Main loop
	while(1){

		switch(g_currentState)
		{
			case STATE_WAITING:
				break;
			case STATE_WAITING_FOR_HT_CALC:
				break;
			case STATE_READ_HT_SENSOR_START:
				err = RM_HS400X_MeasurementStart(g_hs400x_sensor0.p_ctrl);
				if(err == FSP_SUCCESS)
				{
					g_currentState = STATE_WAITING;
				}else{

					g_currentState = STATE_ERROR;
				}
				break;
			case STATE_READ_HT_SENSOR_READ:
				err = RM_HS400X_Read(g_hs400x_sensor0.p_ctrl, &gs_hs400x_raw_data);
				if(err == FSP_SUCCESS)
				{
					g_currentState = STATE_WAITING_FOR_HT_CALC;
				}else{

					g_currentState = STATE_ERROR;
				}

				break;
			case STATE_READ_HT_SENSOR_CALC:
				err = RM_HS400X_DataCalculate(g_hs400x_sensor0.p_ctrl,&gs_hs400x_raw_data,(rm_hs400x_data_t *)&gs_hs400x_data);
				//TODO: Hier muss der Sensor eingebunden werden
				gs_hs400x_data;



				g_currentState = (err == FSP_SUCCESS) ? STATE_READ_INTERNAL_TEMPERATURE : STATE_ERROR;
				break;

			case STATE_READ_INTERNAL_TEMPERATURE:


				g_interrupt_flag_ADC = false;
				R_Config_ADC_Start();

				// Wait for the interrupt of the ADC
				while(!g_interrupt_flag_ADC);

				// Read the ADC value
				ADIF = 0U;						/* Clear INTAD request */
				R_Config_ADC_Get_Result_12bit(g_temperature_internal);
				R_Config_ADC_Stop ();

				g_currentState = STATE_STORE_VALUE;
				break;
			case STATE_STORE_VALUE:
				// Store the Data in the Data Flash

				// Check if the block is blank -> if not erase
				R_RFD_BlankCheckDataFlashReq(g_dataFlash_blockNumber);
				l_e_sam_ret_status = Sample_CheckCFDFSeqEnd();
				if (SAMPLE_ENUM_RET_ERR_ACT_BLANKCHECK == l_e_sam_ret_status)
				{
					R_RFD_EraseDataFlashReq(g_dataFlash_blockNumber);

					l_e_sam_ret_status = Sample_CheckCFDFSeqEnd();

					if (SAMPLE_ENUM_RET_STS_OK != l_e_sam_ret_status){
						g_currentState = STATE_ERROR;
						break;
					}
				}
			    else if (SAMPLE_ENUM_RET_STS_OK != l_e_sam_ret_status)
			    {
			    	g_currentState = STATE_ERROR;
					break;
			    }

				// Write data to the block
				uint16_t integer_external_temp = gs_hs400x_data.temperature.integer_part;
				uint16_t decimal_external_temp = gs_hs400x_data.temperature.decimal_part;

				/*Since the size of the datatype of internal temperature and external is uint16, it takes 2 bytes of space*/
				uint8_t write_buffer[WRITE_BUFFER_SIZE_FLASH] = {
				    (uint8_t)(g_temperature_internal[0] & 0xFF),       // Low-Byte
				    (uint8_t)((g_temperature_internal[0] >> 8) & 0xFF), // High-Byte
					(uint8_t)(integer_external_temp & 0xFF),       // Low-Byte
					(uint8_t)((integer_external_temp >> 8) & 0xFF), // High-Byte
					(uint8_t)(decimal_external_temp & 0xFF),       // Low-Byte
					(uint8_t)((decimal_external_temp >> 8) & 0xFF) // High-Byte
				};


				uint16_t byte_count = 0;
				for (byte_count = 0; byte_count < WRITE_BUFFER_SIZE_FLASH; byte_count++)
				{

					R_RFD_WriteDataFlashReq(DF_WRITE_START_ADDRESS + (g_dataFlash_blockNumber*4) + byte_count, &write_buffer[byte_count] );

					l_e_sam_ret_status = Sample_CheckCFDFSeqEnd();

					if (SAMPLE_ENUM_RET_STS_OK != l_e_sam_ret_status)
					{
						g_currentState = STATE_ERROR;
						break;
					}

				}

				// Set flash sequence in non-programmable mode after all operations are completed
				// If this is uncommented the flash will be locked and reprogramming isn't possible
//				l_e_rfd_status_flag = R_RFD_SetFlashMemoryMode(R_RFD_ENUM_FLASH_MODE_UNPROGRAMMABLE);
//
//				if (R_RFD_ENUM_RET_STS_OK != l_e_rfd_status_flag)
//				{
//					g_currentState = STATE_ERROR;
//				}
				g_currentState = STATE_PRINT_DATA;
				//P5_bit.no3 = 0;
				g_dataFlash_blockNumber++;
				if(g_dataFlash_blockNumber > 9) g_dataFlash_blockNumber = 0;
				break;
			case STATE_PRINT_DATA:
				// Use the RTC to Add a Timestamp
				R_Config_RTC_Get_CounterValue(&g_rtc_counterValue);

				// Convert BCD to decimal for readable output
				uint8_t hour = bcd_to_decimal(g_rtc_counterValue.hour);
				uint8_t minute = bcd_to_decimal(g_rtc_counterValue.min);
				uint8_t second = bcd_to_decimal(g_rtc_counterValue.sec);

				// Prepare text output buffer
				char tx_buf1[WRITE_BUFFER_SIZE_UART] = {0};
				uint16_t length = sprintf(tx_buf1,
				                     "%02d:%02d:%02d | Int: %d (raw) | Ext: %d.%02d C\r\n",
				                     hour, minute, second,
				                     g_temperature_internal[0],
									 gs_hs400x_data.temperature.integer_part, gs_hs400x_data.temperature.decimal_part);

				// UART senden, aber nur die relevante Länge
				R_Config_UARTA1_Start();
				g_uarta1_tx_end = R_Config_UARTA1_Send(tx_buf1,length);

				// blocking: wait until transmitting done
				while(!g_interrupt_flag_UART);

				R_Config_UARTA1_Stop();
				g_interrupt_flag_UART = false; // reset the flag

				g_currentState = STATE_WAITING;
				break;

			case STATE_ENTERING_LPM:
				// Stop the timer in order to enter LPM
				R_Config_TAU0_0_Stop();
				PIN_WRITE(LED2) = true; // Set LEDs High
				// Turn LED Off for additional power Saving
				PIN_WRITE(LED1) = true;
				g_interrupt_flag_USRSW = false; // reset interrupt flag otherwise the loop will be skipped
				do{
					STOP();
				}while(!g_interrupt_flag_USRSW);

				break;
			case STATE_ESCAPING_LPM:
				// Start the timer Again and continue with the normal work
				R_Config_TAU0_0_Start();
				g_interrupt_flag_USRSW = false;
				g_currentState = STATE_WAITING;

				break;

			case STATE_ERROR:

				PIN_WRITE(LED2) = false; // Set LEDs High
				break;
			default:
				g_currentState = STATE_ERROR;
			break;
		}

	}

    return 0;
}

// Custom Functions
// Convert BCD to decimal
uint8_t bcd_to_decimal(uint8_t bcd)
{
	return ((bcd >> 4) * 10) + (bcd & 0x0F);
}
