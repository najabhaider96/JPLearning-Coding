/*
Welcome to JP Learning
*/
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

// For TLS Certificate Upload
#include <time.h>
#include <TZ.h>
#include <LittleFS.h>
#include <CertStoreBearSSL.h>

#define RELAY_PIN 4
#define DHT_PIN 5

#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);

// Wi-Fi Credentials
const char *ssid = "Your SSID";
const char *password = "Your Password";

// MQTT Broker Details
String device_id = "Device0001";
const char *mqtt_server = "Your Cluster Address";
const int mqtt_port = 8883;
const char *mqtt_user = "Your Created User";
const char *mqtt_password = "Your Created Password";
const char *mqtt_clientId = "Deivce_Device0001";
const char *topic_publish = "SensorData";
const char *topic_subscribe = "CommandRequest";

BearSSL::CertStore certStore;
WiFiClientSecure esp_client;
void callback(char *topic, byte *payload, unsigned int length);
//PubSubClient mqtt_client(mqtt_server, mqtt_port, callback, esp_client);

PubSubClient *mqtt_client;

WiFiClient esp_client_http;
HTTPClient http;
String requestUrl = "http://Your IP Address:8080/events/last/deviceId/" + device_id + "/type/Light";

// Data Sending Time
unsigned long CurrentMillis, PreviousMillis, DataSendingTime = (unsigned long)1000 * 10;

// Variable
byte lightStatus;
void setup()
{
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  dht.begin();
  LittleFS.begin();
  delay(5000);
  Serial.println("\n\nWelcome to JP Learning\n");
  setup_wifi();
  get_lastEvent();
  setDateTime();

  int numCerts = certStore.initCertStore(LittleFS, PSTR("/certs.idx"), PSTR("/certs.ar"));
  Serial.printf("Number of CA certs read: %d\n", numCerts);
  if (numCerts == 0)
  {
    Serial.printf("No certs found. Did you run certs-from-mozilla.py and upload the LittleFS directory before running?\n");
    return; // Can't connect to anything w/o certs!
  }

  BearSSL::WiFiClientSecure *bear = new BearSSL::WiFiClientSecure();
  // Integrate the cert store with this connection
  bear->setCertStore(&certStore);

  mqtt_client = new PubSubClient(*bear);

  mqtt_client->setServer(mqtt_server, mqtt_port);
  mqtt_client->setCallback(callback);

  mqtt_connect();
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

      // Publish Temperature Data
      String pkt = "{";
      pkt += "\"device_id\": \"" + device_id + "\", ";
      pkt += "\"type\": \"Temperature\", ";
      pkt += "\"value\": " + String(DHT_Temperature) + "";
      pkt += "}";
      mqtt_publish((char *)pkt.c_str());

      // Publish Humidity Data
      String pkt2 = "{";
      pkt2 += "\"device_id\": \"" + device_id + "\", ";
      pkt2 += "\"type\": \"Humidity\", ";
      pkt2 += "\"value\": " + String(DHT_Humidity) + "";
      pkt2 += "}";
      mqtt_publish((char *)pkt2.c_str());
    }
  }

  if (!mqtt_client->loop())
    mqtt_connect();
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
void get_lastEvent()
{
  //Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\n\nPerforming HTTP GET Request\n");

    // HTTP Details
    http.begin(esp_client_http, requestUrl);
    //    http.setAuthorization("Basic token");
    //    http.setAuthorization("Bearer token");

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
        digitalWrite(RELAY_PIN, LOW);
        Serial.println("\nLast state is Light ON\n");
      }
      else
      {
        lightStatus = 0;
        digitalWrite(RELAY_PIN, HIGH);
        Serial.println("\nLast state is Light OFF\n");
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
void setDateTime()
{
  // You can use your own timezone, but the exact time is not used at all.
  // Only the date is needed for validating the certificates.
  configTime(TZ_Europe_Berlin, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2)
  {
    delay(100);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println();

  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.printf("%s %s", tzname[0], asctime(&timeinfo));
}

void mqtt_connect()
{
  // Loop until we're reconnected
  while (!mqtt_client->connected())
  {
    Serial.println("\nAttempting MQTT connection...");

    Serial.println("Reconnecting MQTT client to : " + String(mqtt_server) + ":" + String(mqtt_port));
    Serial.println("mqtt_clientId : " + String(mqtt_clientId));
    Serial.println("mqtt_user : " + String(mqtt_user));
    Serial.println("mqtt_password : " + String(mqtt_password));
    // Attempt to connect
    if (mqtt_client->connect(mqtt_clientId, mqtt_user, mqtt_password))
    {
      Serial.println("MQTT Client Connected");
      // Subscribe
      mqtt_subscribe(topic_subscribe);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client->state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void mqtt_publish(char *data)
{
  mqtt_connect();
  Serial.println("Publish Topic: \"" + String(topic_publish) + "\"");
  if (mqtt_client->publish(topic_publish, data))
    Serial.println("Publish \"" + String(data) + "\" ok");
  else
    Serial.println("Publish \"" + String(data) + "\" failed");
}
void mqtt_subscribe(const char *topic)
{
  if (mqtt_client->subscribe(topic))
    Serial.println("Subscribe \"" + String(topic) + "\" ok");
  else
    Serial.println("Subscribe \"" + String(topic) + "\" failed");
}
void callback(char *topic, byte *payload, unsigned int length)
{
  String command;
  Serial.print("\n\nMessage arrived [");
  Serial.print(topic);
  Serial.println("] ");
  for (int i = 0; i < length; i++)
    command += (char)payload[i];

  if (command.length() > 0)
    Serial.println("Command receive is : " + command);

  DynamicJsonDocument doc(1024);
  deserializeJson(doc, command);
  JsonObject obj = doc.as<JsonObject>();

  String id = obj[String("device_id")];
  String type = obj[String("type")];
  String value = obj[String("value")];
  Serial.println("\nCommand device_id is : " + id);
  Serial.println("Command type is : " + type);
  Serial.println("Command value is : " + value);

  if (id == device_id && type == "Light")
  {
    if (value == "1")
    {
      lightStatus = 1;
      digitalWrite(RELAY_PIN, LOW);
      Serial.println("\nLight ON by Application");
    }
    else
    {
      lightStatus = 0;
      digitalWrite(RELAY_PIN, HIGH);
      Serial.println("\nLight OFF by Application");
    }
    mqtt_publish((char *)command.c_str());
  }
}
