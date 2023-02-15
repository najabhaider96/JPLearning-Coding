// #define ENABLE_DEBUG

#ifdef ENABLE_DEBUG
#define DEBUG_ESP_PORT Serial
#define NODEBUG_WEBSOCKETS
#define NDEBUG
#endif

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "SinricPro.h"
#include "SinricProSwitch.h"

// Wi-Fi Credentials
const char* ssid = "Your Wi-Fi SSID";
const char* password = "Your Wi-Fi PASSWORD";

// Sinric Pro Credentials
#define APP_KEY "Your APP KEY of Sinric Pro"
#define APP_SECRET "Your APP SECRET of Sinric Pro"
#define SWITCH_ID_1 "Your SWITCH ID of Sinric Pro"

#define RELAY_PIN 5


bool onPowerState1(const String& deviceId, bool& state) {
  if (state) {
    Serial.println("Device 1 turned ON");
    digitalWrite(RELAY_PIN, LOW);
  } else {
    Serial.println("Device 1 turned OFF");
    digitalWrite(RELAY_PIN, HIGH);
  }
  return true;  // request handled properly
}

// setup function for SinricPro
void setupSinricPro() {
  // add devices and callbacks to SinricPro
  SinricProSwitch& mySwitch1 = SinricPro[SWITCH_ID_1];
  mySwitch1.onPowerState(onPowerState1);

  // setup SinricPro
  SinricPro.onConnected([]() {
    Serial.println("Connected to SinricPro\r\n");
  });
  SinricPro.onDisconnected([]() {
    Serial.println("Disconnected from SinricPro\r\n");
  });
  SinricPro.restoreDeviceStates(true); // Uncomment to restore the last known state from the server.

  SinricPro.begin(APP_KEY, APP_SECRET);
}

// main setup function
void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  delay(1000);
  Serial.println("\n\nWelcome to JP Learning\n");
  setup_wifi();

  setupSinricPro();
}

void loop() {
  SinricPro.handle();
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}