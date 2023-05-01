/*
  Welcome to JP Learning
*/
#include <SoftwareSerial.h>
#include <DHT.h>

// GPIO Pins
byte TX_PIN = 5, RX_PIN = 4;
byte LED_PIN = 2;
byte DHT_PIN = 14;

SoftwareSerial Soft_Serial(RX_PIN, TX_PIN);

#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);
void setup() {
  Serial.begin(115200);
  Soft_Serial.begin(9600);
  dht.begin();

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  Serial.println("\n\nWelcome to JP Learning\n");
  Serial.println("Receiver Start\n");
}
void loop() {
  //If Serial Data is available
  while (Soft_Serial.available() > 0) {
    char req = Soft_Serial.read();
    Serial.println("\nreq = " + String(req));
    if (req == 'D') {
      Serial.println("Request Received for Data");

      float DHT_Temperature = dht.readTemperature();
      if (isnan(DHT_Temperature)) {
        Serial.println("Failed to read from DHT11 sensor!");
        delay(1000);
      } else {
        Serial.println("DHT Temperature: " + String(DHT_Temperature) + " °C");

        Soft_Serial.write(String(DHT_Temperature).c_str());
      }
    }
    digitalWrite(LED_PIN, LOW);
    delay(100);
    digitalWrite(LED_PIN, HIGH);
    Soft_Serial.flush();
  }
}
