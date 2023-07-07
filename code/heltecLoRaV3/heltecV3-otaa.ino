// per linux cal seguir això http://community.heltec.cn/t/wireless-stick-lite-v3-reboots-after-uploading-a-simple-code/11982/10
// Curs estiu 2023. Formacio professorat
// ttn OTAA heltec lora 32 V3
// Amb llibreria LoraWan_app de Heltec
// jnogues@irp.cat

#include "LoRaWan_APP.h"
uint32_t license[4]={ 0x00000000,0x0000000,0x00000000,0x00000000};

/* OTAA para*/
uint8_t devEui[] = { 0x00, 0xB3, 0x00, 0x7E, 0x00, 0x00, 0xDB, 0x08 };//msb
uint8_t appEui[] = { 0xAA, 0xBB, 0x00, 0xBB, 0xAA, 0x00, 0x00, 0xBB };//msb
uint8_t appKey[] = { 0xC0, 0x00, 0x00, 0xF8, 0x40, 0x00, 0x10, 0xA2, 0x00, 0x0E, 0x41, 0x00, 0x6F, 0x34, 0x3F, 0xA8 };//msb

/* ABP para*///no serveix
uint8_t nwkSKey[] = { 0x15, 0xb1, 0x00, 0xef, 0xa4, 0x63, 0xdf, 0xbe, 0x3d, 0x11, 0x18, 0x1e, 0x1e, 0xc7, 0xda,0x85 };
uint8_t appSKey[] = { 0xd7, 0x2c, 0x00, 0x00, 0x00, 0xdc, 0xca, 0xbf, 0x55, 0xee, 0x4a, 0x77, 0x8d, 0x16, 0xef,0x67 };
uint32_t devAddr =  ( uint32_t )0x00000000;

/*LoraWan channelsmask, default channels 0-7*/ 
uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };

/*LoraWan region, select in arduino IDE tools*/
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION;

/*LoraWan Class, Class A and Class C are supported*/
DeviceClass_t  loraWanClass = CLASS_A;

/*the application data transmission duty cycle.  value in [ms].*/
uint32_t appTxDutyCycle = 120*1000;//temps dormint en ms

/*OTAA or ABP*/
bool overTheAirActivation = true;//true per OTAA, false per ABP

/*ADR enable*/
bool loraWanAdr = true;//false

/* Indicates if the node is sending confirmed or unconfirmed messages */
bool isTxConfirmed = false;

/* Application port */
uint8_t appPort = 2;
/*!
* Number of trials to transmit the frame, if the LoRaMAC layer did not
* receive an acknowledgment. The MAC performs a datarate adaptation,
* according to the LoRaWAN Specification V1.0.2, chapter 18.4, according
* to the following table:
*
* Transmission nb | Data Rate
* ----------------|-----------
* 1 (first)       | DR
* 2               | DR
* 3               | max(DR-1,0)
* 4               | max(DR-1,0)
* 5               | max(DR-2,0)
* 6               | max(DR-2,0)
* 7               | max(DR-3,0)
* 8               | max(DR-3,0)
*
* Note, that if NbTrials is set to 1 or 2, the MAC will not decrease
* the datarate, in case the LoRaMAC layer did not receive an acknowledgment
*/
uint8_t confirmedNbTrials = 8;

#define VBATpin 1
#define ADCCTRLpin 37
uint16_t VBATvalue = 0;
#define LED 35
#define INT_PIN 0
bool alarmaFoc = 0;

/* Prepares the payload of the frame */
static void prepareTxFrame( uint8_t port )
{
  /*appData size is LORAWAN_APP_DATA_MAX_SIZE which is defined in "commissioning.h".
  *appDataSize max value is LORAWAN_APP_DATA_MAX_SIZE.
  *if enabled AT, don't modify LORAWAN_APP_DATA_MAX_SIZE, it may cause system hanging or failure.
  *if disabled AT, LORAWAN_APP_DATA_MAX_SIZE can be modified, the max value is reference to lorawan region and SF.
  *for example, if use REGION_CN470, 
  *the max value for different DR can be found in MaxPayloadOfDatarateCN470 refer to DataratesCN470 and BandwidthsCN470 in "RegionCN470.h".
  */
    uint16_t VBAT = readVBAT();
    //boolean alarmaFoc = digitalRead(INT_PIN);
    Serial.println(" ");
    Serial.print("VBAT = ");
    Serial.println(VBAT);
    appDataSize = 4;
    appData[0] = lowByte(VBAT);
    appData[1] = highByte(VBAT);
    appData[2] = alarmaFoc;
    appData[3] = random(15,35);//temperatura random
}


//downlink data handle function example
void downLinkDataHandle(McpsIndication_t *mcpsIndication)
{
  Serial.printf("+REV DATA:%s,RXSIZE %d,PORT %d\r\n",mcpsIndication->RxSlot?"RXWIN2":"RXWIN1",mcpsIndication->BufferSize,mcpsIndication->Port);
  Serial.print("+REV DATA:");
  for(uint8_t i=0;i<mcpsIndication->BufferSize;i++)
  {
    Serial.printf("%02X",mcpsIndication->Buffer[i]);
  }
  Serial.println();
  uint32_t color=mcpsIndication->Buffer[0]<<16|mcpsIndication->Buffer[1]<<8|mcpsIndication->Buffer[2];
#if(LoraWan_RGB==1)
  turnOnRGB(color,5000);
  turnOffRGB();
#endif
}

void setup() 
{
  setCpuFrequencyMhz(80);
  esp_err_t esp_bluedroid_disable();
  esp_err_t esp_bt_controller_disable();
  esp_err_t esp_wifi_stop();
  Serial.begin(115200);
  Serial.println(" ");
  //Mcu.setlicense(license);
  Mcu.begin();
  deviceState = DEVICE_STATE_INIT;
  pinMode(ADCCTRLpin, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  delay(50);
  digitalWrite(LED, LOW);
  print_wakeup_reason();
  //pinMode(INT_PIN, INPUT);
}

void loop()
{
  switch( deviceState )
  {
    case DEVICE_STATE_INIT:
    {
#if(LORAWAN_DEVEUI_AUTO)
      LoRaWAN.generateDeveuiByChipID();
#endif 
      LoRaWAN.init(loraWanClass,loraWanRegion);
      //LoRaWAN.setDataRateForNoADR(DR_4);
      //DR_0 SF12
      //DR_2 SF10
      //DR_3 SF9
      //DR_4 SF8
      //DR_5 SF7
      //DR_6 SF7BW250, sembla que no fuciona
      break;
    }
    case DEVICE_STATE_JOIN:
    {
      LoRaWAN.join();
      break;
    }
    case DEVICE_STATE_SEND:
    {
      if (alarmaFoc == 0) appPort = 2;
      if (alarmaFoc == 1) appPort = 3;
      prepareTxFrame( appPort );
      LoRaWAN.send();
      deviceState = DEVICE_STATE_CYCLE;
      break;
    }
    case DEVICE_STATE_CYCLE:
    {
      // Schedule next packet transmission
      if (alarmaFoc == 0) {appTxDutyCycle = 120 * 1000;} 
      if (alarmaFoc == 1) {appTxDutyCycle = 30 * 1000; delay(10000);} 
      txDutyCycleTime = appTxDutyCycle + randr( -APP_TX_DUTYCYCLE_RND, APP_TX_DUTYCYCLE_RND );
      LoRaWAN.cycle(txDutyCycleTime);
      deviceState = DEVICE_STATE_SLEEP;
      break;
    }
    case DEVICE_STATE_SLEEP:
    {
      esp_sleep_enable_ext0_wakeup((gpio_num_t)INT_PIN,0);
      LoRaWAN.sleep(loraWanClass);
      break;
    }
    default:
    {
      deviceState = DEVICE_STATE_INIT;
      break;
    }
  }
}


uint16_t readVBAT()
{
  digitalWrite(ADCCTRLpin, LOW);
  uint16_t kk = analogRead(VBATpin);
  digitalWrite(ADCCTRLpin, HIGH);
  return kk;
}

void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : 
    {
      Serial.println("Wakeup caused by external signal using RTC_IO, ALARMA FOC!!!!"); 
      alarmaFoc = 1;
      break;
    }
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : 
    {
      Serial.println("Wakeup caused by timer"); 
      alarmaFoc = 0;
      break;
    }
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}
