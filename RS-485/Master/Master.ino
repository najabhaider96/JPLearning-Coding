/*
  Welcome to JP Learning
*/
#include <SoftwareSerial.h>

// GPIO Pins
byte TX_PIN = 4, RX_PIN = 5;
byte DE_RE_PIN = 16, LED_PIN = 2;

SoftwareSerial Soft_Serial(RX_PIN, TX_PIN);

// Data Read Timeout
unsigned long CurrentMillis, PreviousMillis, DataReadTimeout = (unsigned long)1000 * 10;
void setup() {
  Serial.begin(115200);
  Soft_Serial.begin(9600);

  pinMode(DE_RE_PIN, OUTPUT);     //DE/RE Controling pin of RS-485
  digitalWrite(DE_RE_PIN, HIGH);  //DE/RE=HIGH Transmit Enabled
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  Serial.println("\n\nWelcome to JP Learning\n");
  Serial.println("Sender Start\n");
}
void loop() {
  // Sending Data Request
  digitalWrite(DE_RE_PIN, HIGH);  //DE/RE=HIGH Transmit Enabled
  char req = 'D';
  Soft_Serial.write(req);
  Serial.println("\nRequest Send for Data");

  // Receiving Data After Request
  digitalWrite(DE_RE_PIN, LOW);  //DE/RE=LOW Receive Enabled
  while (true) {
    if (Soft_Serial.available() > 0) {
      Serial.println("Request Data Received");
      String data = Soft_Serial.readString();
      Serial.println("data = " + data);

      digitalWrite(LED_PIN, LOW);
      delay(100);
      digitalWrite(LED_PIN, HIGH);
      PreviousMillis = millis();
      break;
    }

    CurrentMillis = millis();
    if (CurrentMillis - PreviousMillis > DataReadTimeout) {
      PreviousMillis = CurrentMillis;
      Serial.println("Request Data Timout");
      break;
    }
  }
  delay(1000);
}
