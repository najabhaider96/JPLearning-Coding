/*
  Welcome to JP Learning
*/
#include <SoftwareSerial.h>

// GPIO Pins
byte TX_PIN = 4, RX_PIN = 5;
byte DE_RE_PIN = 16, LED_PIN = 2;

SoftwareSerial Soft_Serial(RX_PIN, TX_PIN);

void setup() {
  Serial.begin(115200);
  Soft_Serial.begin(9600);

  pinMode(DE_RE_PIN, OUTPUT);    //DE/RE Controling pin of RS-485
  digitalWrite(DE_RE_PIN, LOW);  //DE/RE=LOW Receive Enabled
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  Serial.println("\n\nWelcome to JP Learning\n");
  Serial.println("Receiver Start\n");
}
void loop() {
  //If Serial Data is available
  while (Soft_Serial.available() > 0) {
    Serial.println(Soft_Serial.read());
    digitalWrite(LED_PIN, LOW);
    delay(100);
    digitalWrite(LED_PIN, HIGH);
    Soft_Serial.flush();
  }
}
