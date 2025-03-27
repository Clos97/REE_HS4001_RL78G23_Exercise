# REE_HS4001_RL78G23_Exercise

Diese Training Exercise wurde mir von Panagiotis Moustakas im Rahmen meines Rampups bei Renesas gestellt. Das training dient dem Zweck die Grundlagen des RL78/G23 zu erlernen.


| ![](https://www.renesas.com/sites/default/files/styles/two_columns/public/qciot-hs4001pocz-pmod-board.jpg?itok=tpyNxCd1)                                                        | ![](https://www.renesas.com/sites/default/files/rl78-g23-64p-fpb-board-box.jpg)                                                                                                                                                                         |
| ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| [QCIOT-HS4001POCZ](https://www.renesas.com/en/products/sensor-products/environmental-sensors/humidity-temperature-sensors/qciot-hs4001pocz-relative-humidity-sensor-pmod-board) | [FPB-RL78/G23-64p](https://www.renesas.com/en/products/microcontrollers-microprocessors/rl78-low-power-8-16-bit-mcus/rtk7rlg230clg000bj-rl78g23-64p-fast-prototyping-board-rl78g23-64p-fpb?srsltid=AfmBOorJzMDep3PxpTA-xD71cSGDCI9gCZ0FEG-yP5jVRFysE3TGfUme) |

## Aufgabenstellung

### HW & SW
- Use the RL78/G23 and connect an I2C sensor. The sensor could be: 
  - Any Renesas sensor like HS400x

### Main parts
- Read temperature from the Sensor via I2C ✔️
- Store the Data in the Data Flash ✔️
- Display it on a Serial Terminal ✔️
- Implementing the internal temperature sensor: 
  - Read temperature from the RL78 Internal Temperature Sensor ✔️
  - Store it into the Data Flash ✔️
  - Display it on a Serial Terminal ✔️ -> Displaying just as raw data
  - Compare both temperatures, store it into the Data Flash & display the result. ❌
- Reading the Data Periodically:  
  - Setup a timer to read temperature from external and internal sensor periodically. ✔️
- Implementing the RTC:
  - Configure RTC and display the current time/date. ✔️
- Low Power Mode:
  - Configure button: when pressed, stop measuring temperature and put the device into low power mode (stop or halt mode). ✔️
  - Configure button: when pressed, wake up the device and start temperature measurements ✔️


_Optional:_ implement security & encryption  (discuss with finish the project) ❌

## Important Notes
### Manuelle Verkabelung

- Kein PMOD Interface → Verkabelung des Sensors Manuell
![PXL_20250325_083826386 MP](https://github.com/user-attachments/assets/2b633879-1031-49ff-8880-b838be70af46)

### Integration Temperatursensor (v1.0)

- Download des Software Moduls über `Components -> plus -> download RL78 Software Integration System Modules -> HS400x`
- Zusätzlich muss das r_comms_i2c Modul über den gleichen Weg gedownloaded werden 
- Mit `STRG + H` kann man einen Advanced Search im e2-Studio machen

> [!Important]
> Sollte der Download nicht möglich sein, kann man die Module auch manuell downloaden. Beispielsweise das `r_comms_i2c`-Modul: 
> 
> **Download und File Structure**
> - Download von der [Renesas Website RL78/G23](https://www.renesas.com/en/products/microcontrollers-microprocessors/rl78-low-power-8-16-bit-mcus/rl78g23-new-generation-rl78-general-purpose-microcontrollers-further-refined-low-power-performance-and?srsltid=AfmBOop_dlbiGyYFvlGjxG6_OXwUp8M2bCfdOt8J3trk-Or_FGw--aXW&software-title-filter=hs#design_development) -> Software & Tools -> Comms Modul
>
>  **Einfügen der Module**
>- Finden des Ordners: `Help -> Installation Details -> Support Folders -> e2 studio download area -> RL78 Modules`
>- Kopieren des gesamten Ordner Inhalt `Generic Modules`
>- e2-Studio neu starten

> [!TIP]
> Nachdem weder der HS4001 weder noch der ZMOD4410 funktionieren, habe ich die Spannung 3V3 am Sensor gemessen und festgestellt, dass lediglich 0.7 V anliegen.
> 
> **Lösung:**
> GND und 3V3 waren vertauscht -> Anscheinend ist eine Schutzdiode vor Verpolung verbaut, welche die Spannung auf 0.7V abklemmt
***
> [!TIP]
> - Weder mit dem HS4001 noch dem ZMOD4410 kann eine Kommunikation aufgebaut werden.
> - Sobald ein Sendevorgang stattfindet, bricht dieser ab
> -- Wenn ich das Programm mit dem [ZMOD4410](https://www.mouser.de/datasheet/2/698/REN_ZMOD4410_DST_20240313-1999773.pdf) laufen lasse, dann bekomme ich den Error FSP_ERR_TIMEOUT im `RM_ZMOD4XXX_Open(g_zmod4xxx_sensor0.p_ctrl, g_zmod4xxx_sensor0.p_cfg);`
> - Wenn ich das selbe mit dem HS4001 mache, dann erhalte ich den Error FSP_IN_USE
>   
>***
> **Lösung:** 
> - Innerhalb des Package mit den Beispielprojekten befindet sich ein PDF namens `r01an5899ej0160-zmod-mcu.pdf` 
> - Auf S.82  beschrieben die User Modifikation der Callbacks der `Config_IICA0_user.c` Datei
***
> [!TIP]
> Die I2C Kommunikation mit einem ZMOD Sensor kann durch das Schreiben der Register 0x88 bis 0x8B verifiziert werden. 
> Eine genauere Erläuterung ist in folgenden FAQ zu finden: [ZMOD4xxx Validate I2C Communication](https://en-support.renesas.com/knowledgeBase/20506425)
***
#### Erstellen eines Automatengraphens
![image](https://github.com/user-attachments/assets/a49c0c2a-d6be-4a39-afea-a9f6a63c62ee)

### 🔒Implementieren des Data Flashes
- DataFlash 8 Kbytes -> Siehe 📚[Groups User Manual RL78/G23 S. 1299](https://www.renesas.com/en/document/man/rl78g23-users-manual-hardware?r=1496636)
- Durchführen der Implementierung des Dataflashes anhand dieser 📚[Application Note](https://www.renesas.com/en/document/apn/rl78-family-renesas-flash-driver-rl78-type-01-sc-version-data-flash-application-note?r=488896)
- Download der Middleware aus dem Downloadbereich von e2 Studio
	- Flash Common `r_rfd_rl78_t01_common`
	- Dataflash `r_rfd_rl78_t01_dataflash`
- Zum Implementieren müssen keine Einstellungen getroffen werden (nur Generade Code)
- Enablen des Data flashes im `r_bsp` Stack
- Dieses 📹[Video](https://www.renesas.com/en/video/how-use-renesas-flash-driver-rl78-type-01-sc-version-e-studio) hilft bei der Implementierung
- [[Alexandros Zikas]] schickt mir die folgenden Informationen: 
	- ❓[Community: How to create a project from scratch to use the Renesas Flash Driver RL78 Type 01 on RL78/G2x devices ?](https://en-support.renesas.com/knowledgeBase/21757166)
	- 📚[6.1.3.2 The setting of user definition macro](https://www.renesas.com/en/document/man/renesas-flash-driver-rl78-type-01-users-manual-rl78g23)
- Implementierung erfolgreich mit Hilfe des Beispielcodes. 
- Wichtig: `R_RFD_SetFlashMemoryMode(R_RFD_ENUM_FLASH_MODE_UNPROGRAMMABLE)` muss entfernt/ auskommentiert werden, da sonst nach einem Durchlauf der Flash nicht mehr beschrieben werden kann

### 📣Implementieren des Loggens über UART

![](https://soldered.com/productdata/2023/03/uart-connection-two-devices.png)

- Hier Informationen zum 📚[Digilent USBUART Pmbo Modul](https://digilent.com/reference/_media/pmod:pmod:pmodusbuart_rm.pdf)
- Für die Übertragung muss die `flHP` Clock verwendet werden -> mit der Middle Speed Clock hat es nicht geklappt 
- Teiler auf 8 gestellt
- Salea Logic kann in der Einstellung Async Serial die Daten lesen und auch in einem Terminal ausgeben

### 🕘Implementieren der Realt Time Clock (RTC)

- Implementierung ist relativ trivial 
- Ausgabewert ist BCD Time Format

> [!NOTE]
> - BCD (Binary-Coded Decimal) speichert jede Dezimalziffer in 4 Bit
> - Beispiel **12:00:00 Uhr** Format: 
> 	- Stunden 12: `0x12` (0001 0010)
> 	- Minuten 00: `0x00` (0000 0000)
> 	- Sekunden 00: `0x00` (0000 0000)
> - Beispiel Berechnen der Stunden 0001 0010
> 	1. `bcd >> 4`: 0001 0010 → 0000 0001 (dezimal 1)
> 	2. 1 * 10 = 10
> 	3. `bcd & 0x0F`: 0001 0010 → entspricht Dezimal 2
> 	4. 10 +2 = 12
> 
> ```c
>// Convert BCD to decimal
>uint8_t bcd_to_decimal(uint8_t bcd)
>{
>	return ((bcd >> 4) * 10) + (bcd & 0x0F);
>}
>```

### 🔋Implementieren des Low Power Modes

![PXL_20250325_083830023 MP](https://github.com/user-attachments/assets/ed5a949d-3258-4cd6-9056-f0d6d75f208d)


- Der Low Power Mode heißt beim RL78 "Standby Functions" Kapitel 23

| Modus      | Beschreibung                                                 | CPU-Takt | Peripherie                                    | Stromverbrauch | Wiederaufnahme                                          |
| ---------- | ------------------------------------------------------------ | -------- | --------------------------------------------- | -------------- | ------------------------------------------------------- |
| **HALT**   | CPU-Takt gestoppt, Peripherie bleibt aktiv                   | Stopp    | Weiter aktiv                                  | Gering         | Sofort nach Interrupt                                   |
| **STOP**   | Gesamtes System gestoppt, maximale Stromersparnis            | Stopp    | Meist gestoppt                                | Höchste        | nach Interrupt mit Verzögerung wegen Taktstabilisierung |
| **SNOOZE** | Spezielle Peripherie kann aktiv bleiben, CPU bleibt gestoppt | Stopp    | Ausgewählte Module aktiv (ADC, SAU, DTC etc.) | Mittel         | Automatisch durch Peripherie-Ereignisse                 |

> [!IMPORTANT]
> 1. **STOP-Modus** nur mit Hauptsystemtakt erlaubt, nicht mit Subsystemtakt.
> 2. Vor dem **STOP-Modus** sollten Peripheriemodule, die den Hauptsystemtakt nutzen, deaktiviert werden (außer SNOOZE-Module).
> 3. Der **Low-Speed On-Chip Oscillator** kann wahlweise weiterlaufen oder gestoppt werden (WDTON / WUTMMCK0 Bit).

- Messung der Power Consumption kann mit Hilfe der folgenden Tools gemacht werden 
	- [DA14531-00FXDEVKT-P](https://www.renesas.com/en/products/wireless-connectivity/bluetooth-low-energy/da14531-00fxdevkt-p-smartbond-tiny-da14531-bluetooth-low-energy-51-system-chip-development-kit-pro)
	- [Power Profiler aus der SmartSnippets Toolbox](https://www.renesas.com/en/software-tool/smartbond-development-tools)
- Funktioniert mit den meisten BLE EVKs -> lediglich der Header ändert sich

> [!Important]
> for the DA14531 Dev Kit Pro you will need to connect GND and then on header J9
> **Wichtig für RL78:** 
> Entfernen des Headers J17 (siehe 📚[User Manual RL78/G23 Tabelle 6-1](https://www.renesas.com/en/document/man/rl78g23-64p-fast-prototyping-board-users-manual?r=1496646))
> 
> **Verbindungen:**
> - RL78 J17-2 to DA14531 J4-1
> - RL78 GND to DA14531 GND (Sqquare Symbol)

- Inbetriebnahme des Programms [SmartSnippets Toolbox](https://www.renesas.com/en/software-tool/smartbond-development-tools)
	- Board -> Power Profiler -> COM Port wählen 
	- Nichts anschließen und "Offset Calibration" 
	- Control -> Initialize 
	- Control -> Start
***
**Mehr Informationen sind im dazugehörigen Obsidian Dokument enthalten**
