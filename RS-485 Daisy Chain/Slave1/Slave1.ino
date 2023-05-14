/*
  Welcome to JP Learning
*/
#include <SoftwareSerial.h>

// GPIO Pins
byte TX_PIN = 4, RX_PIN = 5;
byte DE_RE_PIN = 16;

SoftwareSerial Soft_Serial(RX_PIN, TX_PIN);

//Variables
byte inLen, _inBuff[8] = { 0 }, outLen, _outBuff[7] = { 0 };
byte slaveID = 0x01, functionCode = 0x03;

// Request Receive Timeout
unsigned long CurrentMillis, PreviousMillis, Timeout = (unsigned long)10;
void setup() {
  Serial.begin(115200);
  Soft_Serial.begin(9600);
  Soft_Serial.flush();  // flush transmit buffer

  pinMode(DE_RE_PIN, OUTPUT);    //DE/RE Controling pin of RS-485
  digitalWrite(DE_RE_PIN, LOW);  //DE/RE=LOW Receive Enabled

  Serial.println("\n\nWelcome to JP Learning\n");
  Serial.println("Slave 0x" + String(slaveID, HEX) + " Start\n");
}
void loop() {
  // Receiving Enable
  digitalWrite(DE_RE_PIN, LOW);  //DE/RE=LOW Receive Enabled
  //If Serial Data is available
  // inLen = 0;
  while (Soft_Serial.available() > 0) {
    byte temp = Soft_Serial.read();
    Serial.println("Request[" + String(inLen) + "]: " + String(temp, DEC) + " " + String(temp, HEX));
    _inBuff[inLen] = temp;
    inLen++;
    PreviousMillis = millis();
  }

  if (inLen >= 8) {
    Serial.println("\ninLen = " + String(inLen));
    byte unitAddr = _inBuff[0];
    byte function = _inBuff[1];

    if (unitAddr != slaveID) {
      Serial.println("Address Not OK: 0x" + String(unitAddr, HEX));
      inLen = 0;
      Soft_Serial.flush();            // flush transmit buffer
      return;
    }
    Serial.println("Address OK: 0x" + String(unitAddr, HEX));

    if (function != functionCode) {
      Serial.println("Function Code Not OK: 0x" + String(function, HEX));
      inLen = 0;
      Soft_Serial.flush();            // flush transmit buffer
      return;
    }
    Serial.println("Function Code OK: 0x" + String(unitAddr, HEX));

    byte crc[2];
    // append CRC
    uint16_t u16CRC = 0xFFFF;
    for (byte i = 0; i < inLen - 2; i++) {
      u16CRC = crc16_update(u16CRC, _inBuff[i]);
    }
    crc[0] = lowByte(u16CRC);
    crc[1] = highByte(u16CRC);

    // Serial.println("crc[0] = " + String(crc[0], HEX));
    // Serial.println("crc[1] = " + String(crc[1], HEX));
    if (_inBuff[inLen - 2] != crc[0] || _inBuff[inLen - 1] != crc[1]) {
      Serial.println("CRC Not OK: 0x" + String(u16CRC, HEX));
      inLen = 0;
      Soft_Serial.flush();            // flush transmit buffer
      return;
    }
    Serial.println("CRC OK: 0x" + String(u16CRC, HEX));

    // Transmition Enable
    digitalWrite(DE_RE_PIN, HIGH);  //DE/RE=HIGH Transmit Enabled
    Soft_Serial.flush();            // flush transmit buffer
    inLen = 0;

    int value = random(1, 1000);
    Serial.println("\nvalue: " + String(value) + " 0x" + String(value, HEX) + "\n\n");
    // Serial.println("value[0] = " + String(lowByte(value), HEX));
    // Serial.println("value[1] = " + String(highByte(value), HEX));

    outLen = 2;
    _outBuff[0] = slaveID;
    _outBuff[1] = functionCode;
    _outBuff[2] = outLen;
    _outBuff[3] = highByte(value);
    _outBuff[4] = lowByte(value);

    // append CRC
    u16CRC = 0xFFFF;
    for (byte i = 0; i < 5; i++) {
      u16CRC = crc16_update(u16CRC, _outBuff[i]);
    }
    // Serial.println("\nu16CRC = " + String(u16CRC, HEX));
    // Serial.println("crc[0] = " + String(lowByte(u16CRC), HEX));
    // Serial.println("crc[1] = " + String(highByte(u16CRC), HEX));

    _outBuff[5] = lowByte(u16CRC);
    _outBuff[6] = highByte(u16CRC);

    Soft_Serial.write(_outBuff, sizeof(_outBuff));
    Serial.println("\n");
  } else if (inLen > 0) {
    CurrentMillis = millis();
    if (CurrentMillis - PreviousMillis > Timeout) {
      PreviousMillis = CurrentMillis;
      inLen = 0;
      Serial.println("\nTimout\n");
    }
  }
}
static uint16_t crc16_update(uint16_t crc, uint8_t a) {
  crc ^= a;
  for (int i = 0; i < 8; ++i) {
    if (crc & 1)
      crc = (crc >> 1) ^ 0xA001;
    else
      crc = (crc >> 1);
  }
  return crc;
}
