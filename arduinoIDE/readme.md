# IDE, llibreries i  plaques per instal·lar.
## IDE
* Instal·leu l'Arduino [IDE 2.1.0](https://www.arduino.cc/en/software).
## Gestor de plaques
* Per tal de tenir suport a les ESP32 i Heltec afegir a preferències:

`https://github.com/HelTecAutomation/CubeCell-Arduino/releases/download/V1.5.0/package_CubeCell_index.json`

`https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series/releases/download/0.0.7/package_heltec_esp32_index.json`

`https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
## Llibreies amb instal·lació manual (descàrrega a Discord)
* simpleTimer.
* Grove_BME280.
## Llibreries a instal·lar pel gestor de llibreries de l'IDE (+ info a Discord)
* MCCI LoRaWAN LMIC library 4.1.1 , necessària pels ESP32 genèrics amb xip SX1276
* Heltec ESP32 Dev-Boards 1.1.1 , els __Heltec ESP32 Wifi LoRa V3 porten xip SX1262 i han d'utilitzar la llibreria de Heltec.__

* Per la llibreria MCCI cal configurar el fitxer de text **C:\Users\lora\Documents\Arduino\libraries\MCCI_LoRaWAN_LMIC_library\project_config** perquè quedi així:
```
// project-specific definitions
#define CFG_eu868 1
//#define CFG_us915 1
//#define CFG_au915 1
//#define CFG_as923 1
// #define LMIC_COUNTRY_CODE LMIC_COUNTRY_CODE_JP      /* for as923-JP; also define CFG_as923 */
//#define CFG_kr920 1
//#define CFG_in866 1
#define CFG_sx1276_radio 1
//#define LMIC_USE_INTERRUPTS
#define hal_init LMICHAL_init
```
    
  
   
