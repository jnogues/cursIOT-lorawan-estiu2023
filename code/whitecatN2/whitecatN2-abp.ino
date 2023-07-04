// Curs estiu 2023. Formacio professorat
// ttn ABP whitecatboard N2
// guarda frame counter
// afegit #define hal_init LMICHAL_init a project_config
// Cal ajustar a Europa a project_config
// jnogues@irp.cat

#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include <Preferences.h>
Preferences preferences;
#include <SimpleTimer.h>
SimpleTimer timerr;

//msb
static const PROGMEM u1_t NWKSKEY[16] = { 0x00, 0x0C, 0x09, 0xBE, 0xE9, 0x0F, 0x12, 0x80, 0x31, 0x77, 0xC3, 0x21, 0x92, 0xAC, 0xC2, 0x97 };
static const u1_t PROGMEM APPSKEY[16] = { 0x00, 0x00, 0x00, 0x55, 0x96, 0x92, 0x24, 0x00, 0x92, 0x1B, 0xE0, 0x00, 0x00, 0x97, 0xA9, 0xB4 };
static const u4_t DEVADDR = 0x00000000 ; // <-- Change this address for every node!

void os_getArtEui (u1_t* buf) { }
void os_getDevEui (u1_t* buf) { }
void os_getDevKey (u1_t* buf) { }

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  1*60//1*60 temps que dorm en segons, per tant un minut
#define TIME_ALIVE 40*1000 //temps despert en ms, per tant 40s, potser millor reduir

static osjob_t sendjob;

// Schedule TX every this many seconds (might become longer due to duty
// cycle limitations).
const unsigned TX_INTERVAL = 60;//temps entre enviaments, necessari si no dorm

// Pin mapping
#define NSS_GPIO        9
#define RESET_GPIO      13
#define DIO0_GPIO       36
#define DIO1_GPIO       37
#define DIO2_GPIO       38

const lmic_pinmap lmic_pins = {
  .nss = NSS_GPIO,
  .rxtx = LMIC_UNUSED_PIN,
  .rst = RESET_GPIO,
  .dio = {DIO0_GPIO, DIO1_GPIO, DIO2_GPIO},//DIO2_GPIO
};

RTC_DATA_ATTR unsigned int bootCount = 0;

void setup() {
  setCpuFrequencyMhz(80);
  esp_err_t esp_bluedroid_disable();
  esp_err_t esp_bt_controller_disable();
  esp_err_t esp_wifi_stop();

  Serial.begin(115200);
  delay(2000);
  Serial.println("Starting");
  pinMode(27, OUTPUT);//encen radio
  pinMode(22, OUTPUT);//stat led
  digitalWrite(27, HIGH);
  digitalWrite(22, HIGH);
  SPI.begin(18, 19, 23, 9); //SCK,MISO,MOSI,SS

  Serial.print("CPU speed=");
  Serial.println( getCpuFrequencyMhz() );
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));
  print_wakeup_reason();
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);//configurem el temps de dormir
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " Seconds");

  timerr.setInterval(500L, toggle);
  timerr.setInterval(TIME_ALIVE, go2sleep);//60 sec despert

  lmicInit();//Inicialitzo llibreria LMIC

  Serial.println();
  Serial.println("-------------------------------------------------------------------------------");

}

void loop() {

  os_runloop_once();
  timerr.run();

}

//****************************************************
void go2sleep()
{
  digitalWrite(27, LOW);//apago radio
  digitalWrite(22, LOW);//apago led
  delay(500);
  Serial.println("Going to sleep now");
  Serial.flush();
  esp_deep_sleep_start();
}

//****************************************************
unsigned int fcnt_read() {
  preferences.begin("fcnt", true);
  unsigned int counter = preferences.getUInt("counter", 0);
  preferences.end();
  return counter;
}

void fcnt_up() {
  preferences.begin("fcnt", false);
  unsigned int counter = preferences.getUInt("counter", 0);
  preferences.putUInt("counter", counter + 1);
  preferences.end();
}


//****************************************************
/* segons exemple llibreria
  void tts_send(osjob_t* j){
    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println(F("OP_TXRXPEND, not sending"));
    } else {
        // Prepare upstream data transmission at the next possible time.
        fcnt_up();//INCREMENTO FRAME COUNTER A EEPROM
        LMIC_setTxData2(1, mydata, sizeof(mydata)-1, 0);
        Serial.println(F("Packet queued"));
    }
    // Next TX is scheduled after TX_COMPLETE event.
  }
*/
//segons Jaume
void tts_send(osjob_t* j)
{
  if (LMIC.opmode & OP_TXRXPEND) {
    Serial.println(F("OP_TXRXPEND, not sending"));
    return;
  }
  byte dades[4];
  memset(dades, 0, 4); //esborra estat
  dades[0] = 0;
  dades[1] = 1;
  dades[2] = random(10,99);// humitat random
  dades[3] = random(15,35);// temperatura random
  fcnt_up();
  LMIC_setTxData2(1, dades, 4, 0);// port 1
  Serial.println(F("[RFM95] Packet queued"));
}

void onEvent (ev_t ev) {
  Serial.print(os_getTime());
  Serial.print(": ");
  switch (ev) {
    case EV_SCAN_TIMEOUT:
      Serial.println(F("EV_SCAN_TIMEOUT"));
      break;
    case EV_BEACON_FOUND:
      Serial.println(F("EV_BEACON_FOUND"));
      break;
    case EV_BEACON_MISSED:
      Serial.println(F("EV_BEACON_MISSED"));
      break;
    case EV_BEACON_TRACKED:
      Serial.println(F("EV_BEACON_TRACKED"));
      break;
    case EV_JOINING:
      Serial.println(F("EV_JOINING"));
      break;
    case EV_JOINED:
      Serial.println(F("EV_JOINED"));
      break;
    /*
      || This event is defined but not used in the code. No
      || point in wasting codespace on it.
      ||
      || case EV_RFU1:
      ||     Serial.println(F("EV_RFU1"));
      ||     break;
    */
    case EV_JOIN_FAILED:
      Serial.println(F("EV_JOIN_FAILED"));
      break;
    case EV_REJOIN_FAILED:
      Serial.println(F("EV_REJOIN_FAILED"));
      break;
    case EV_TXCOMPLETE:
      Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
      if (LMIC.txrxFlags & TXRX_ACK)
        Serial.println(F("Received ack"));
      if (LMIC.dataLen) {
        Serial.println(F("Received "));
        Serial.println(LMIC.dataLen);
        Serial.println(F(" bytes of payload"));
        Serial.println(LMIC.frame[LMIC.dataBeg], HEX);
        if (LMIC.frame[LMIC.dataBeg] == 0x51) {
          Serial.println("Rele ON");
          //digitalWrite(rele,HIGH);
        }
        if (LMIC.frame[LMIC.dataBeg] == 0x71) {
          Serial.println("Rele OFF");
          //digitalWrite(rele,LOW);
        }
      }
      // Schedule next transmission
      os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL), tts_send);
      break;
    case EV_LOST_TSYNC:
      Serial.println(F("EV_LOST_TSYNC"));
      break;
    case EV_RESET:
      Serial.println(F("EV_RESET"));
      break;
    case EV_RXCOMPLETE:
      // data received in ping slot
      Serial.println(F("EV_RXCOMPLETE"));
      break;
    case EV_LINK_DEAD:
      Serial.println(F("EV_LINK_DEAD"));
      break;
    case EV_LINK_ALIVE:
      Serial.println(F("EV_LINK_ALIVE"));
      break;
    /*
      || This event is defined but not used in the code. No
      || point in wasting codespace on it.
      ||
      || case EV_SCAN_FOUND:
      ||    Serial.println(F("EV_SCAN_FOUND"));
      ||    break;
    */
    case EV_TXSTART:
      Serial.println(F("EV_TXSTART"));
      break;
    case EV_TXCANCELED:
      Serial.println(F("EV_TXCANCELED"));
      break;
    case EV_RXSTART:
      /* do not print anything -- it wrecks timing */
      break;
    case EV_JOIN_TXCOMPLETE:
      Serial.println(F("EV_JOIN_TXCOMPLETE: no JoinAccept"));
      break;
    default:
      Serial.print(F("Unknown event: "));
      Serial.println((unsigned) ev);
      break;
  }
}


//***********************************************************************
void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  switch (wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;
  }
}

void toggle()
{
  digitalWrite(22, !digitalRead(22));
}


void lmicInit()
{
  // LMIC init
  os_init();
  // Reset the MAC state. Session and pending data transfers will be discarded.
  LMIC_reset();
  LMIC_setClockError(MAX_CLOCK_ERROR * 5 / 100); // A treure
  // Set static session parameters. Instead of dynamically establishing a session
  // by joining the network, precomputed session parameters are be provided.
#ifdef PROGMEM
  // On AVR, these values are stored in flash and only copied to RAM
  // once. Copy them to a temporary buffer here, LMIC_setSession will
  // copy them into a buffer of its own again.
  uint8_t appskey[sizeof(APPSKEY)];
  uint8_t nwkskey[sizeof(NWKSKEY)];
  memcpy_P(appskey, APPSKEY, sizeof(APPSKEY));
  memcpy_P(nwkskey, NWKSKEY, sizeof(NWKSKEY));
  LMIC_setSession (0x13, DEVADDR, nwkskey, appskey);
#else
  // If not running an AVR with PROGMEM, just use the arrays directly
  LMIC_setSession (0x13, DEVADDR, NWKSKEY, APPSKEY);
#endif
#if defined(CFG_eu868)
  // Set up the channels used by the Things Network, which corresponds
  // to the defaults of most gateways. Without this, only three base
  // channels from the LoRaWAN specification are used, which certainly
  // works, so it is good for debugging, but can overload those
  // frequencies, so be sure to configure the full frequency range of
  // your network here (unless your network autoconfigures them).
  // Setting up channels should happen after LMIC_setSession, as that
  // configures the minimal channel set. The LMIC doesn't let you change
  // the three basic settings, but we show them here.
  LMIC_setupChannel(0, 868100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(1, 868300000, DR_RANGE_MAP(DR_SF12, DR_SF7B), BAND_CENTI);      // g-band
  LMIC_setupChannel(2, 868500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(3, 867100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(4, 867300000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(5, 867500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(6, 867700000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(7, 867900000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(8, 868800000, DR_RANGE_MAP(DR_FSK,  DR_FSK),  BAND_MILLI);      // g2-band
  // TTN defines an additional channel at 869.525Mhz using SF9 for class B
  // devices' ping slots. LMIC does not have an easy way to define set this
  // frequency and support for class B is spotty and untested, so this
  // frequency is not configured here.
#elif defined(CFG_us915) || defined(CFG_au915)
  // NA-US and AU channels 0-71 are configured automatically
  // but only one group of 8 should (a subband) should be active
  // TTN recommends the second sub band, 1 in a zero based count.
  // https://github.com/TheThingsNetwork/gateway-conf/blob/master/US-global_conf.json
  LMIC_selectSubBand(1);
#elif defined(CFG_as923)
  // Set up the channels used in your country. Only two are defined by default,
  // and they cannot be changed.  Use BAND_CENTI to indicate 1% duty cycle.
  // LMIC_setupChannel(0, 923200000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);
  // LMIC_setupChannel(1, 923400000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);

  // ... extra definitions for channels 2..n here
#elif defined(CFG_kr920)
  // Set up the channels used in your country. Three are defined by default,
  // and they cannot be changed. Duty cycle doesn't matter, but is conventionally
  // BAND_MILLI.
  // LMIC_setupChannel(0, 922100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_MILLI);
  // LMIC_setupChannel(1, 922300000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_MILLI);
  // LMIC_setupChannel(2, 922500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_MILLI);

  // ... extra definitions for channels 3..n here.
#elif defined(CFG_in866)
  // Set up the channels used in your country. Three are defined by default,
  // and they cannot be changed. Duty cycle doesn't matter, but is conventionally
  // BAND_MILLI.
  // LMIC_setupChannel(0, 865062500, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_MILLI);
  // LMIC_setupChannel(1, 865402500, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_MILLI);
  // LMIC_setupChannel(2, 865985000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_MILLI);

  // ... extra definitions for channels 3..n here.
#else
# error Region not supported
#endif

  // Disable link check validation
  LMIC_setLinkCheckMode(0);
  LMIC_setAdrMode(0);

  // TTN uses SF9 for its RX2 window.
  LMIC.dn2Dr = DR_SF9;

  // Set data rate and transmit power for uplink
  LMIC_setDrTxpow(DR_SF12, 14);

  LMIC.seqnoUp = fcnt_read();//LLEGEIXO FRAME COUNTER A EEPROM
  // Start job
  tts_send(&sendjob);

}

