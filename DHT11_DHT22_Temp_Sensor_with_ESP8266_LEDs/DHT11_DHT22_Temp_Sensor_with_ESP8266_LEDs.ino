#include "DHT.h"

#define DHTPIN1 D5 // DPIO 14
#define DHTPIN2 D6 // DPIO 12
#define LED_PIN1 4
#define LED_PIN2 5

#define DHTTYPE1 DHT11
#define DHTTYPE2 DHT22

DHT dht11(DHTPIN1, DHTTYPE1);
DHT dht22(DHTPIN2, DHTTYPE2);

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  dht11.begin();
  dht22.begin();
}

void loop() {
  // For DHT11 and LED1
  float DHT11_Temp = dht11.readTemperature();
  if (isnan(DHT11_Temp)) {
    Serial.println("\n\nFailed to read from DHT11 sensor!");
  } else {
    Serial.println("\n\nDHT11 Temperature: " + String(DHT11_Temp) + "°C");
    if (DHT11_Temp > 32) {
      Serial.println("LED1 ON");
      digitalWrite(LED_PIN1, HIGH);
    } else {
      Serial.println("LED1 OFF");
      digitalWrite(LED_PIN1, LOW);
    }
  }

  // For DHT22 and LED2
  float DHT22_Temp = dht22.readTemperature();
  if (isnan(DHT22_Temp)) {
    Serial.println("\nFailed to read from DHT22 sensor!");
  } else {
    Serial.println("\nDHT22 Temperature: " + String(DHT22_Temp) + "°C");
    if (DHT22_Temp >= 33) {
      Serial.println("LED2 ON");
      digitalWrite(LED_PIN2, HIGH);
    } else {
      Serial.println("LED2 OFF");
      digitalWrite(LED_PIN2, LOW);
    }
  }
  delay(2000);
}
