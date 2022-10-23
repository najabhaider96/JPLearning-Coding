/*
Welcome to JP Learning
*/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define TRIGGER_PIN 12
#define ECHO_PIN 14
#define LED_PIN 5

// Wi-Fi Credentials
const char* ssid = "Your SSID";
const char* password = "Your Password";

// MQTT Broker Details
String device_id = "Device0001";
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_user = "Device0001";
const char* mqtt_password = "Device0001";
const char* mqtt_clientId = "Deivce_Device0001";
const char* topic_publish = "JPLearning_SensorData";
const char* topic_subscribe = "JPLearning_CommandRequest";

WiFiClient esp_client;
void callback(char* topic, byte* payload, unsigned int length);
PubSubClient mqtt_client(mqtt_server, mqtt_port, callback, esp_client);

// Data Sending Time
unsigned long CurrentMillis, PreviousMillis, DataSendingTime = (unsigned long) 1000 * 10;

// Variable
float distanceCentimeter, distanceInch, level;
void setup() {
  Serial.begin(115200);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  delay(5000);
  Serial.println("\n\nWelcome to JP Learning\n");
  setup_wifi();
  mqtt_connect();
}

void loop() {
  // Clears the trigPin condition
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  float duration = pulseIn(ECHO_PIN, HIGH);
  // Calculating the distance in cm
  distanceCentimeter = duration * 0.017; // Speed of sound wave divided by 2 (go and back)
  // Calculating the distance in inches (in = cm * 0.393701)
  distanceInch = distanceCentimeter * 0.393701;

  level = 100 - ((distanceInch / 15) * 100);

  Serial.println("\n\nDistance = " + String(distanceCentimeter) + " cm, " + String(distanceInch) + " in");
  Serial.println("Wastebin level = " + String(level) + " %");

  if (level >= 0 && level <= 100) {
    if (level >= 80) {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("\nLevel is 80% or more");
    } else {
      digitalWrite(LED_PIN, LOW);
      Serial.println("\nLevel is below 80%");
    }

    // Devices State Sync Request
    CurrentMillis = millis();
    if (CurrentMillis - PreviousMillis > DataSendingTime) {
      PreviousMillis = CurrentMillis;

      // Publish Distance Data
      String pkt = "{";
      pkt += "\"device_id\": \"Device00001\", ";
      pkt += "\"distanceCentimeter\": \"" + String(distanceCentimeter) + "\", ";
      pkt += "\"distanceInch\": \"" + String(distanceInch) + "\", ";
      pkt += "\"level\": \"" + String(level) + "\"";
      pkt += "}";
      mqtt_publish((char*) pkt.c_str());
    }
  } else
    Serial.println("\nWrong reading");

  if (!mqtt_client.loop())
    mqtt_connect();
  delay(1000);
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
    //    if (mqtt_client.connect(mqtt_clientId, mqtt_user, mqtt_password)) {
    if (mqtt_client.connect(mqtt_clientId)) {
      Serial.println("MQTT Client Connected");
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
}
