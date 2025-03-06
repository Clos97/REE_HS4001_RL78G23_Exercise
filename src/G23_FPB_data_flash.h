/*
 * G23_FPB_data_flash.h
 *
 *  Created on: 11 Ιαν 2025
 *      Author: azikas
 */

#ifndef G23_FPB_DATA_FLASH_H_
#define G23_FPB_DATA_FLASH_H_


/* Defines */
#define SAMPLE_VALUE_U08_MASK1_FSQ_STATUS_ERR_CFDF_SEQUENCER  (0x10u)
#define SAMPLE_VALUE_U08_MASK1_FSQ_STATUS_ERR_EXTRA_SEQUENCER (0x20u)
#define SAMPLE_VALUE_U08_MASK1_FSQ_STATUS_ERR_ERASE           (0x01u)
#define SAMPLE_VALUE_U08_MASK1_FSQ_STATUS_ERR_WRITE           (0x02u)
#define SAMPLE_VALUE_U08_MASK1_FSQ_STATUS_ERR_BLANKCHECK      (0x08u)
#define SAMPLE_VALUE_U32_DF_BASE_ADDR                         (0x000F1000u)
#define L_MCLK_FREQ_1MHz  (1000000uL)
#define L_MCLK_ROUNDUP_VALUE (999999uL)


#define SAMPLE_VALUE_U01_MASK0_1BIT                           (0u)
#define SAMPLE_VALUE_U01_MASK1_1BIT                           (1u)
#define SAMPLE_VALUE_U08_MASK0_8BIT                           (0x00u)
#define SAMPLE_VALUE_U08_MASK1_8BIT                           (0xFFu)



typedef enum e_sample_ret
{
    SAMPLE_ENUM_RET_STS_OK                 = 0x00u,
    SAMPLE_ENUM_RET_ERR_PARAMETER          = 0x10u,
    SAMPLE_ENUM_RET_ERR_CONFIGURATION      = 0x11u,
    SAMPLE_ENUM_RET_ERR_MODE_MISMATCHED    = 0x12u,
    SAMPLE_ENUM_RET_ERR_CHECK_WRITE_DATA   = 0x13u,
    SAMPLE_ENUM_RET_ERR_CFDF_SEQUENCER     = 0x20u,
    SAMPLE_ENUM_RET_ERR_EXTRA_SEQUENCER    = 0x21u,
    SAMPLE_ENUM_RET_ERR_ACT_ERASE          = 0x22u,
    SAMPLE_ENUM_RET_ERR_ACT_WRITE          = 0x23u,
    SAMPLE_ENUM_RET_ERR_ACT_BLANKCHECK     = 0x24u,
    SAMPLE_ENUM_RET_ERR_CMD_ERASE          = 0x30u,
    SAMPLE_ENUM_RET_ERR_CMD_WRITE          = 0x31u,
    SAMPLE_ENUM_RET_ERR_CMD_BLANKCHECK     = 0x32u,
    SAMPLE_ENUM_RET_ERR_CMD_SET_EXTRA_AREA = 0x33u,
} e_sample_ret_t;

R_RFD_FAR_FUNC e_sample_ret_t Sample_CheckCFDFSeqEnd(void)
{
    /* Local variable definition */
    e_sample_ret_t l_e_sam_ret_value;
    uint8_t        l_u08_status_flag;

    /* Set local variables */
    l_e_sam_ret_value = SAMPLE_ENUM_RET_STS_OK;

    /* Sequencer busy loop step1 */
    while (R_RFD_ENUM_RET_STS_BUSY == R_RFD_CheckCFDFSeqEndStep1())
    {
        /* No operation */
        /* It is possible to write the program for detecting timeout here as necessity requires */
    }

    /* Sequencer busy loop step2 */
    while (R_RFD_ENUM_RET_STS_BUSY == R_RFD_CheckCFDFSeqEndStep2())
    {
        /* No operation */
        /* It is possible to write the program for detecting timeout here as necessity requires */
    }

    /* Action error check */
    R_RFD_GetSeqErrorStatus(&l_u08_status_flag);

    /* Check error status */
    if (SAMPLE_VALUE_U08_MASK0_8BIT != (l_u08_status_flag & SAMPLE_VALUE_U08_MASK1_FSQ_STATUS_ERR_CFDF_SEQUENCER))
    {
        /* Set return value */
        l_e_sam_ret_value = SAMPLE_ENUM_RET_ERR_CFDF_SEQUENCER;
    }
    else if (SAMPLE_VALUE_U08_MASK0_8BIT != (l_u08_status_flag & SAMPLE_VALUE_U08_MASK1_FSQ_STATUS_ERR_ERASE))
    {
        /* Set return value */
        l_e_sam_ret_value = SAMPLE_ENUM_RET_ERR_ACT_ERASE;
    }
    else if (SAMPLE_VALUE_U08_MASK0_8BIT != (l_u08_status_flag & SAMPLE_VALUE_U08_MASK1_FSQ_STATUS_ERR_WRITE))
    {
        /* Set return value */
        l_e_sam_ret_value = SAMPLE_ENUM_RET_ERR_ACT_WRITE;
    }
    else if (SAMPLE_VALUE_U08_MASK0_8BIT != (l_u08_status_flag & SAMPLE_VALUE_U08_MASK1_FSQ_STATUS_ERR_BLANKCHECK))
    {
        /* Set return value */
        l_e_sam_ret_value = SAMPLE_ENUM_RET_ERR_ACT_BLANKCHECK;
    }
    else
    {
        /* Set return value */
        l_e_sam_ret_value = SAMPLE_ENUM_RET_STS_OK;
    }

    /* Clear sequencer registers */
    R_RFD_ClearSeqRegister();

    return (l_e_sam_ret_value);
}

#endif /* G23_FPB_DATA_FLASH_H_ */
