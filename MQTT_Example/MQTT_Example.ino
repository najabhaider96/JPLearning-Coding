/*
Welcome to JP Learning
*/
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define LED_PIN 4
#define DHTPIN 5

#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// Wi-Fi Credentials
const char* ssid = "Your SSID";
const char* password = "Your Password";
// MQTT Details
const char* mqtt_server = "********";
const int mqtt_port = 1883;
const char* mqtt_user = "********";
const char* mqtt_password = "********";
const char* mqtt_clientId = "********";
const char* topic_publish = "********";
const char* topic_subscribe = "********";

WiFiClient esp_client;
void callback(char* topic, byte* payload, unsigned int length);
//PubSubClient mqtt_client(esp_client);
//PubSubClient mqtt_client(mqtt_server, mqtt_port, esp_client);
//PubSubClient mqtt_client(mqtt_server, mqtt_port, null, esp_client);
PubSubClient mqtt_client(mqtt_server, mqtt_port, callback, esp_client);

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  dht.begin();
  delay(5000);
  Serial.println("\n\nWelcome to JP Learning\n");
  setup_wifi();
  //  mqtt_client.setServer(mqtt_server, mqtt_port);
  //  mqtt_client.setCallback(callback);
  mqtt_connect();
}

void loop() {
  float DHT_Temperature = dht.readTemperature();
  float DHT_Humidity = dht.readHumidity();
  if (isnan(DHT_Temperature) || isnan(DHT_Humidity)) {
    Serial.println("\n\nFailed to read from DHT sensor!");
  } else {
    Serial.println("\n\nDHT Temperature: " + String(DHT_Temperature) + " °C");
    Serial.println("DHT Humidity: " + String(DHT_Humidity) + " %");

    // Publish Temperature Data
    String pkt = "{";
    pkt += "\"type\":\"temperature\",";
    pkt += "\"value\": " + String(DHT_Temperature) + "";
    pkt += "}";
    mqtt_publish((char*) pkt.c_str());

    // Publish Humidity Data
    String pkt2 = "{";
    pkt2 += "\"type\":\"humidity\",";
    pkt2 += "\"value\": " + String(DHT_Humidity) + "";
    pkt2 += "}";
    mqtt_publish((char*) pkt2.c_str());
  }

  if (!mqtt_client.loop()) {
    mqtt_connect();
  }

  delay(2000);
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

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void mqtt_connect() {
  // Loop until we're reconnected
  while (!mqtt_client.connected()) {
    Serial.print("Attempting MQTT connection...");

    Serial.println("Reconnecting MQTT client to : " + String(mqtt_server) + ":" + String(mqtt_port));
    Serial.println("mqtt_clientId : " + String(mqtt_clientId));
    Serial.println("mqtt_user : " + String(mqtt_user));
    Serial.println("mqtt_password : " + String(mqtt_password));

    // Attempt to connect
    //    if (mqtt_client.connect(mqtt_clientId))
    if (mqtt_client.connect(mqtt_clientId, mqtt_user, mqtt_password))
    {
      Serial.println("connected");
      mqtt_publish("hello world");
      // Subscribe
      mqtt_client.subscribe(topic_subscribe);
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
    Serial.println("Publish ok");
  else
    Serial.println("Publish failed");
}
void callback(char* topic, byte* payload, unsigned int length) {
  String command;
  Serial.print("\n\nMessage arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
    command += (char)payload[i];

  if (command.length() > 0)
    Serial.println("\nCMD receive is : " + command);

  DynamicJsonDocument doc(1024);
  deserializeJson(doc, command);
  JsonObject obj = doc.as<JsonObject>();

  String value = obj[String("value")];
  Serial.println("\nCMD value is : " + value);
  // Switch on the LED if an 1 was received as first character
  if (value == "1") {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED ON by Application");

    mqtt_publish((char*) command.c_str());
  } else {
    digitalWrite(LED_PIN, LOW);
    Serial.println("LED OFF by Application");

    mqtt_publish((char*) command.c_str());
  }
}
