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

String Host = "http://192.168.0.150:8000";
String dataSendUrl = Host + "/practice/index.php";
String lightLastEventUrl = dataSendUrl + "?device_id=" + device_id + "&type=Light&limit=1";

// Data Sending Time
unsigned long CurrentMillis, PreviousMillis, DataSendingTime = (unsigned long)1000 * 10;

// Variable
byte lightStatus, oldLightStatus;
void setup()
{
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  dht.begin();
  delay(5000);
  Serial.println("\n\nWelcome to JP Learning\n");
  setup_wifi();
  get_lastEvent(lightLastEventUrl);
}

void loop()
{
  get_command(lightLastEventUrl);

  // float DHT_Temperature = random(5, 40);
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

    // Devices State Sync Request
    CurrentMillis = millis();
    if (CurrentMillis - PreviousMillis > DataSendingTime)
    {
      PreviousMillis = CurrentMillis;

      // Publish Temperature Data
      String pkt = "{";
      pkt += "\"device_id\": \"" + device_id + "\", ";
      pkt += "\"type\": \"Temperature\", ";
      pkt += "\"value\": " + String(DHT_Temperature) + "";
      pkt += "}";

      send_data(dataSendUrl, pkt);

      // Publish Humidity Data
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
void get_lastEvent(String requestUrl)
{
  //Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\n\nPerforming HTTP GET Request\n");

    // HTTP Details
    http.begin(esp_client, requestUrl);
    // Send HTTP GET request
    int httpResponseCode = http.GET();
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    if (httpResponseCode == HTTP_CODE_OK)
    {
      String payload = http.getString();
      Serial.println("\nResponse payload: " + payload);

      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);
      JsonObject obj = doc.as<JsonObject>();

      String type = obj[String("type")];
      String value = obj[String("value")];
      Serial.println("\ntype: " + type);
      Serial.println("value: " + value);
      if (value == "1")
      {
        lightStatus = 1;
        digitalWrite(RELAY_PIN, HIGH);
        Serial.println("\nLast state is Light ON\n");
      }
      else
      {
        lightStatus = 0;
        digitalWrite(RELAY_PIN, LOW);
        Serial.println("\nLast state is Light OFF\n");
      }
      oldLightStatus = lightStatus;
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
  {
    Serial.println("WiFi Disconnected");
  }
}
void get_command(String requestUrl)
{
  //Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\n\nPerforming HTTP GET Request\n");

    // HTTP Details
    http.begin(esp_client, requestUrl);
    // Send HTTP GET request
    int httpResponseCode = http.GET();
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    if (httpResponseCode == HTTP_CODE_OK)
    {
      String payload = http.getString();
      Serial.println("\nResponse payload: " + payload);

      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);
      JsonObject obj = doc.as<JsonObject>();

      String type = obj[String("type")];
      String value = obj[String("value")];
      Serial.println("\ntype: " + type);
      Serial.println("value: " + value);
      if (value == "1")
      {
        lightStatus = 1;
        digitalWrite(RELAY_PIN, HIGH);
        Serial.println("\nLast state is Light ON\n");
      }
      else
      {
        lightStatus = 0;
        digitalWrite(RELAY_PIN, LOW);
        Serial.println("\nLast state is Light OFF\n");
      }

      // Check oldState with newState of Light if changed then Update data
      if (oldLightStatus != lightStatus)
      {
        oldLightStatus = lightStatus;
        // Publish Light Data
        send_data_light(dataSendUrl + "?device_id=" + device_id, payload);
      }
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
  {
    Serial.println("WiFi Disconnected");
  }
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
void send_data_light(String requestUrl, String body)
{
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\n\nRequestUrl: " + requestUrl);
    Serial.println("POST body: " + body);

    http.begin(esp_client, requestUrl);
    // Send HTTP GET request
    int httpResponseCode = http.PUT(body);
    Serial.print("\nHTTP Response code: ");
    Serial.println(httpResponseCode);

    if (httpResponseCode == HTTP_CODE_OK)
    {
      String response_body = http.getString();
      Serial.println("response_body: " + response_body);
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
