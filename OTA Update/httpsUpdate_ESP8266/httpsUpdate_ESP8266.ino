/*
  Welcome to JP Learning
*/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <CertStoreBearSSL.h>

// Wi-Fi Credentials
const char* ssid = "Your SSID";
const char* password = "Your Password";

String codeVersion = "1.0.0";
String updateURL = "https://Your file address .bin";

// Update Time
unsigned long CurrentMillis, PreviousMillis, UpdateTime = (unsigned long) 1000 * 10;
void setup() {
  Serial.begin(115200);
  delay(5000);
  Serial.println("\n\nWelcome to JP Learning\n");
  setup_wifi();

  ESPhttpUpdate.setClientTimeout(2000);  // default was 8000

  Serial.println("\nCode Version: \"" + codeVersion + "\"\n");
}

void loop() {
  // Check update in every 10 seconds
  CurrentMillis = millis();
  if (CurrentMillis - PreviousMillis > UpdateTime) {
    PreviousMillis = CurrentMillis;

    Serial.println("\nChecking new update.\n");

    // wait for WiFi connection
    if (WiFi.status() == WL_CONNECTED) {
      BearSSL::WiFiClientSecure client;
      client.setInsecure();

      // The line below is optional. It can be used to blink the LED on the board during flashing
      // The LED will be on during download of one buffer of data from the network. The LED will
      // be off during writing that buffer to flash
      // On a good connection the LED should flash regularly. On a bad connection the LED will be
      // on much longer than it will be off. Other pins than LED_BUILTIN may be used. The second
      // value is used to put the LED on. If the LED is on with HIGH, that value should be passed
      ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW);

      // Add optional callback notifiers
      ESPhttpUpdate.onStart(update_started);
      ESPhttpUpdate.onEnd(update_finished);
      ESPhttpUpdate.onProgress(update_progress);
      ESPhttpUpdate.onError(update_error);

      Serial.println("OTA url is : " + updateURL + "\nOTA Begin...");

      //      t_httpUpdate_return ret = ESPhttpUpdate.update(client, updateURL);
      // Or:
      t_httpUpdate_return ret = ESPhttpUpdate.update(client, "Your server address", 443, "Your file path .bin");
      
      switch (ret) {
        case HTTP_UPDATE_FAILED: Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str()); break;

        case HTTP_UPDATE_NO_UPDATES: Serial.println("HTTP_UPDATE_NO_UPDATES"); break;

        case HTTP_UPDATE_OK: Serial.println("HTTP_UPDATE_OK"); break;
      }
    }
  }
}
void update_started() {
  Serial.println("CALLBACK:  HTTP update process started");
}
void update_finished() {
  Serial.println("CALLBACK:  HTTP update process finished");
}
void update_progress(int cur, int total) {
  Serial.printf("CALLBACK:  HTTP update process at %d of %d bytes...\n", cur, total);
}
void update_error(int err) {
  Serial.printf("CALLBACK:  HTTP update fatal error code %d\n", err);
}
void setup_wifi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println("\"" + String(ssid) + "\"");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
