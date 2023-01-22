#include <SoftwareSerial.h>

// Pins define for SoftwareSerial
const byte RX_PIN1 = 4, TX_PIN1 = 5;
const byte RX_PIN2 = 12, TX_PIN2 = 14;
const byte RX_PIN3 = 2, TX_PIN3 = 13;

// Set up a new SoftwareSerial object
SoftwareSerial Soft_Serial1(RX_PIN1, TX_PIN1);
SoftwareSerial Soft_Serial2(RX_PIN2, TX_PIN2);
SoftwareSerial Soft_Serial3(RX_PIN3, TX_PIN3);

void setup() {
  Serial.begin(115200);
  Soft_Serial1.begin(9600);
  Soft_Serial2.begin(9600);
  Soft_Serial3.begin(9600);
  
  // Soft_Serial.begin(9600, SWSERIAL_8N1);
  // Soft_Serial.begin(9600, SWSERIAL_8N2);
  // Soft_Serial.begin(9600, SWSERIAL_8E1);
  // Soft_Serial.begin(9600, SWSERIAL_8E2);
  // Soft_Serial.begin(9600, SWSERIAL_8O1);
  // Soft_Serial.begin(9600, SWSERIAL_8O2);
}

void loop() {
  while (Soft_Serial.available() > 0) {
    Serial.write(Soft_Serial.read());
  }
  while (Serial.available() > 0) {
    Soft_Serial.write(Serial.read());
  }
}
