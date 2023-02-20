/*
  Welcome to JP Learning
*/
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

#define RELAY_PIN 4
#define DHT_PIN 5

#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);

// Wi-Fi Credentials
const char *ssid = "Your SSID";
const char *password = "Your Password";

String device_id = "Device0001";
WiFiClient esp_client;
HTTPClient http;

// Replace with PC IP with PORT
String Host = "http://192.168.0.150:8000";
String dataSendUrl = Host + "/practice/index.php";

// Data Sending Time
unsigned long CurrentMillis, PreviousMillis, DataSendingTime = (unsigned long)1000 * 10;

// Variable
byte lightStatus;
void setup()
{
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  dht.begin();
  delay(5000);
  Serial.println("\n\nWelcome to JP Learning\n");
  setup_wifi();
}

void loop()
{
  float DHT_Temperature = dht.readTemperature();
  float DHT_Humidity = dht.readHumidity();
  if (isnan(DHT_Temperature) || isnan(DHT_Humidity))
  {
    Serial.println("\n\nFailed to read from DHT11 sensor!");
    delay(1000);
  }
  else
  {
    Serial.println("\n\nDHT Temperature: " + String(DHT_Temperature) + " °C");
    Serial.println("DHT Humidity: " + String(DHT_Humidity) + " %");
    delay(1000);

    // Devices State Sync Request
    CurrentMillis = millis();
    if (CurrentMillis - PreviousMillis > DataSendingTime)
    {
      PreviousMillis = CurrentMillis;

      // Send Temperature Data
      String pkt = "{";
      pkt += "\"device_id\": \"" + device_id + "\", ";
      pkt += "\"type\": \"Temperature\", ";
      pkt += "\"value\": " + String(DHT_Temperature) + "";
      pkt += "}";

      send_data(dataSendUrl, pkt);

      // Send Humidity Data
      String pkt2 = "{";
      pkt2 += "\"device_id\": \"" + device_id + "\", ";
      pkt2 += "\"type\": \"Humidity\", ";
      pkt2 += "\"value\": " + String(DHT_Humidity) + "";
      pkt2 += "}";

      send_data(dataSendUrl, pkt2);
    }
  }
  delay(1000);
}

void setup_wifi()
{
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println("\"" + String(ssid) + "\"");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
void send_data(String requestUrl, String body)
{
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\n\nRequestUrl: " + requestUrl);
    Serial.println("POST body: " + body);

    http.begin(esp_client, requestUrl);
    // Send HTTP GET request
    int httpResponseCode = http.POST(body);
    Serial.print("\nHTTP Response code: ");
    Serial.println(httpResponseCode);

    if (httpResponseCode == HTTP_CODE_CREATED)
    {
      String response_body = http.getString();
      Serial.println("response_body: " + response_body);

      DynamicJsonDocument doc(1024);
      deserializeJson(doc, response_body);
      JsonObject obj = doc.as<JsonObject>();

      String response = obj[String("response")];
      Serial.println("response: " + response);
    }
    else
    {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else
    Serial.println("WiFi is not connected");
}
