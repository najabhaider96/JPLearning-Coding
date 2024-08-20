/*
  Welcome to JP Learning
*/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>

// Access Point Credentials
const char* ap_ssid = "OTA-Update";
const char* ap_password = "12345678"; 

ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

void setup() {
  Serial.begin(115200);
  delay(5000);
  Serial.println("\n\nWelcome to JP Learning\n");

  WiFi.softAP(ap_ssid, ap_password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  // Initialize OTA Web Updater
  httpUpdater.setup(&server);

  // Start Web Server
  server.begin();
  Serial.println("HTTP server started");

  Serial.print("You can upload a new firmware by visiting http://");
  Serial.print(IP);
  Serial.println("/update in your web browser");
}

void loop() {
  server.handleClient();
}
