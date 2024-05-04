/*
  Welcome to JP Learning
*/
#include <DHT.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

#define DHT_PIN 5
#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);

// Wi-Fi Credentials
const char* ssid = "Your SSID";
const char* password = "Your Password";

String HTTPS_POST_URL = "Your HTTPS Post URL";
String HTTPS_GET_URL = "Your HTTPS Get URL";

WiFiClientSecure client;
HTTPClient https;

String device_id = "Device0001";

void setup() {
  Serial.begin(115200);
  dht.begin();
  delay(5000);
  Serial.println("\n\nWelcome to JP Learning\n");
  setup_wifi();

  // Most important line
  client.setInsecure();
  HTTPS_GET(HTTPS_GET_URL);
}

void loop() {
  float DHT_Temperature = dht.readTemperature();
  float DHT_Humidity = dht.readHumidity();
  DHT_Temperature = 22.4;
  DHT_Humidity = 34;
//  if (isnan(DHT_Temperature) || isnan(DHT_Humidity)) {
//    Serial.println("\n\nFailed to read from DHT11 sensor!");
//    delay(1000);
//  } else 
  {
    Serial.println("\n\nDHT Temperature: " + String(DHT_Temperature) + " °C");
    Serial.println("DHT Humidity: " + String(DHT_Humidity) + " %");

    // POST Temperature Data
    String pkt = "{";
    pkt += "\"device_id\": \"" + device_id + "\", ";
    pkt += "\"type\": \"Temperature\", ";
    pkt += "\"value\": " + String(DHT_Temperature) + "";
    pkt += "}";
    HTTPS_POST(HTTPS_POST_URL, pkt);

    // POST Humidity Data
    String pkt2 = "{";
    pkt2 += "\"device_id\": \"" + device_id + "\", ";
    pkt2 += "\"type\": \"Humidity\", ";
    pkt2 += "\"value\": " + String(DHT_Humidity) + "";
    pkt2 += "}";
    HTTPS_POST(HTTPS_POST_URL, pkt2);
  }
  delay(1000);
}
void setup_wifi() {
  Serial.print("\nConnecting to ");
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

void HTTPS_GET(String HTTPS_GET_URL) {
  Serial.println("\nGetting from: " + HTTPS_GET_URL);

  Serial.println("Conntecting to server..");
  if (https.begin(client, HTTPS_GET_URL)) {
    int httpsCode = https.GET();
    if (httpsCode > 0) {
      Serial.println("httpsCode: " + String(httpsCode));
      if (httpsCode == HTTP_CODE_OK) {
        String ServerResponse = https.getString();
        Serial.println("\nServerResponse: " + ServerResponse);
      }
    } else
      Serial.print("Failed to GET");
  }
  else
    Serial.print("Failed to connect to server");
  https.end();
}
void HTTPS_POST(String HTTPS_POST_URL, String PostPacket) {
  Serial.println("\nPosting to: " + HTTPS_POST_URL);
  Serial.println("PostPacket: " + PostPacket);

  Serial.println("Conntecting to server..");
  if (https.begin(client, HTTPS_POST_URL)) {
    https.addHeader("Content-Type", "application/json");
    int httpsCode = https.POST(PostPacket);
    if (httpsCode > 0) {
      Serial.println("httpsCode: " + String(httpsCode));
      if (httpsCode == HTTP_CODE_OK || httpsCode == HTTP_CODE_CREATED) {
        String ServerResponse = https.getString();
        Serial.println("\nServerResponse: " + ServerResponse);
      }
    } else
      Serial.print("Failed to POST");
  }
  else
    Serial.print("Failed to connect to server");
  https.end();
}
