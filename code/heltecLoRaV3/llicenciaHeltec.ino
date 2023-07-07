int64_t chipid;
void setup() {
  Serial.begin(115200);
}

void loop() {
  //The chip ID is essentially its MAC address(length: 6 bytes).
  chipid=ESP.getEfuseMac();
  Serial.printf("ESP32 Chip ID = %04X",(uint16_t)(chipid>>32));//High 2 bytes
  Serial.printf("%08X\n",(uint32_t)chipid);// Low 4bytes.
  delay(3000);
}
