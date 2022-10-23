/*
Welcome to JP Learning
*/
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define LED_PIN 4
#define DHT_PIN 5

#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);

// Wi-Fi Credentials
const char* ssid = "Your SSID";
const char* password = "Your Password";

// MQTT Broker Details
String device_id = "Your device_id";
const char* mqtt_server = "your EMQX Cloud host address";
const int mqtt_port = 1883; // your EMQX Cloud host MQTT port
const char* mqtt_user = "your EMQX Cloud authentation user";
const char* mqtt_password = "your EMQX Cloud authentation password";
const char* mqtt_clientId = "your client id for this device";
const char* topic_publish = "your publish topic";
const char* topic_subscribe = "your subscribe topic";

WiFiClient esp_client;
void callback(char* topic, byte* payload, unsigned int length);
PubSubClient mqtt_client(mqtt_server, mqtt_port, callback, esp_client);

// Data Sending Time
unsigned long CurrentMillis, PreviousMillis, DataSendingTime = (unsigned long) 1000 * 10;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  dht.begin();
  delay(5000);
  Serial.println("\n\nWelcome to JP Learning\n");
  setup_wifi();
  mqtt_connect();
}

void loop() {
  float DHT_Temperature = dht.readTemperature();
  float DHT_Humidity = dht.readHumidity();
  if (isnan(DHT_Temperature) || isnan(DHT_Humidity)) {
    Serial.println("\n\nFailed to read from DHT22 sensor!");
    delay(1000);
  } else {
    Serial.println("\n\nDHT Temperature: " + String(DHT_Temperature) + " °C");
    Serial.println("DHT Humidity: " + String(DHT_Humidity) + " %");
    delay(1000);

    // Devices State Sync Request
    CurrentMillis = millis();
    if (CurrentMillis - PreviousMillis > DataSendingTime) {
      PreviousMillis = CurrentMillis;

      // Publish Temperature Data
      String pkt = "{";
      pkt += "\"device_id\": \"Device00001\", ";
      pkt += "\"type\": \"Temperature\", ";
      pkt += "\"value\": " + String(DHT_Temperature) + "";
      pkt += "}";
      mqtt_publish((char*) pkt.c_str());

      // Publish Humidity Data
      String pkt2 = "{";
      pkt2 += "\"device_id\": \"Device00001\", ";
      pkt2 += "\"type\": \"Humidity\", ";
      pkt2 += "\"value\": " + String(DHT_Humidity) + "";
      pkt2 += "}";
      mqtt_publish((char*) pkt2.c_str());
    }
  }

  if (!mqtt_client.loop())
    mqtt_connect();
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

void mqtt_connect() {
  // Loop until we're reconnected
  while (!mqtt_client.connected()) {
    Serial.println("Attempting MQTT connection...");
    // Attempt to connect
    if (mqtt_client.connect(mqtt_clientId, mqtt_user, mqtt_password)) {
      Serial.println("MQTT Client Connected");
      mqtt_publish((char*)("Hi from " + device_id).c_str());
      // Subscribe
      mqtt_subscribe(topic_subscribe);
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void mqtt_publish(char * data) {
  mqtt_connect();
  if (mqtt_client.publish(topic_publish, data))
    Serial.println("Publish \"" + String(data) + "\" ok");
  else
    Serial.println("Publish \"" + String(data) + "\" failed");
}
void mqtt_subscribe(const char * topic) {
  if (mqtt_client.subscribe(topic))
    Serial.println("Subscribe \"" + String(topic) + "\" ok");
  else
    Serial.println("Subscribe \"" + String(topic) + "\" failed");
}
void callback(char* topic, byte* payload, unsigned int length) {
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

  if (id == device_id && type == "Light") {
    if (value == "1") {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("\nLED ON by Application");

      mqtt_publish((char*) command.c_str());
    } else {
      digitalWrite(LED_PIN, LOW);
      Serial.println("\nLED OFF by Application");

      mqtt_publish((char*) command.c_str());
    }
  }
}
