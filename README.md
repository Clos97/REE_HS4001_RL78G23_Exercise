# REE_HS4001_RL78G23_Exercise

Diese Training Exercise wurde mir von [[Panagiotis Moustakas]] im Rahmen meines Rampups bei Renesas gestellt. Das Dokument mit der Aufgabe ist [Training exercise for RL78-G23.pdf](file:///C%3A%5CUsers%5Ca5153359%5CDocuments%5C01%20-%20Onboarding%20Renesas%5CTraining%20exercise%20for%20RL78-G23.pdf). Der gesamte Fortschritt wird in einem [GitHub Repository](https://github.com/Clos97/REE_HS4001_RL78G23_Exercise) getrackt -> Bitte beachte, dass die unten stehenden beschreibungen der einzelnen Aufgabenteile ebenfalls in der Beschreibung der Release Versionen vorhanden sind.


| ![](https://www.renesas.com/sites/default/files/styles/two_columns/public/qciot-hs4001pocz-pmod-board.jpg?itok=tpyNxCd1)                                                        | ![](https://www.renesas.com/sites/default/files/rl78-g23-64p-fpb-board-box.jpg)                                                                                                                                                                         |
| ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| [QCIOT-HS4001POCZ](https://www.renesas.com/en/products/sensor-products/environmental-sensors/humidity-temperature-sensors/qciot-hs4001pocz-relative-humidity-sensor-pmod-board) | [FPB-RL78/G23-64p](https://www.renesas.com/en/products/microcontrollers-microprocessors/rl78-low-power-8-16-bit-mcus/rtk7rlg230clg000bj-rl78g23-64p-fast-prototyping-board-rl78g23-64p-fpb?srsltid=AfmBOorJzMDep3PxpTA-xD71cSGDCI9gCZ0FEG-yP5jVRFysE3TGfUme) |

## Aufgabenstellung

### HW & SW
- Use the RL78/G23 and connect an I2C sensor. The sensor could be: 
  - Any Renesas sensor like HS400x

### Main parts
- Read temperature from the Sensor via I2C, store it in the Data Flash & display it on a Serial Terminal (v1.0)
- Implementing the internal temperature sensor: (v2.0)
  - Read temperature from the RL78 Internal Temperature Sensor, store it into the Data Flash & display it on a Serial Terminal.
  - Compare both temperatures, store it into the Data Flash & display the result.
- Reading the Data Periodically: (v2.1)
  - Setup a timer to read temperature from external and internal sensor periodically.
- Implementing the RTC: (v3.0)
  - Configure RTC and display the current time/date.
- Low Power Mode: (v4.0)
  - Configure button: when pressed, stop measuring temperature and put the device into low power mode (stop or halt mode). Measure the power consumption (talk with Orestis about this).
  - Configure button: when pressed, wake up the device and start temperature measurements


_Optional:_ implement security & encryption  (discuss with finish the project) (v5.0)
