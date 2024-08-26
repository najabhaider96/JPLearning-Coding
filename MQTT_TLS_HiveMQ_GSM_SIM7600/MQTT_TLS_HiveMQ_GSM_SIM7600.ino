/*
Welcome to JP Learning
*/
// Configure TinyGSM library
#define TINY_GSM_MODEM_SIM7600  // Modem is SIM7600

#include <TinyGsmClient.h>
#include <SSLClient.h>
#include <PubSubClient.h>

#include "ca_cert.h"

// ESP32 LilyGO-T-SIM7600G pins definition
#define MODEM_UART_BAUD 115200
#define MODEM_DTR 25
#define MODEM_TX 26
#define MODEM_RX 27
#define MODEM_PWRKEY 4
#define LED_PIN 12

// Set serial for debug console (to the Serial Monitor)
#define SerialMon Serial
// Set serial for AT commands (to the SIM7600 module)
#define SerialAT Serial1

// Your GPRS credentials (leave empty, if missing)
const char apn[] = "Your APN";       // Your APN
const char gprs_user[] = ""; // User
const char gprs_pass[] = ""; // Password
const char simPIN[] = "";    // SIM card PIN code, if any

// MQTT Broker Details
String device_id = "Device0001";
const char* mqtt_server = "your MQTT Broker host address";
const int mqtt_port = 8883; // TCP-TLS Port
const char* mqtt_user = "your MQTT Broker authentation user";
const char* mqtt_password = "your MQTT Broke authentation password";
const char* mqtt_clientId = "your client id for this device";
const char* topic_publish = "your publish topic";
const char* topic_subscribe = "your subscribe topic";

// GSM Clients
TinyGsm sim_modem(SerialAT);
TinyGsmClient gsm_transpor_layer(sim_modem);

// SSL Secure Client using GSM client
SSLClient secure_presentation_layer(&gsm_transpor_layer);

// MQTT Client
void callback(char* topic, byte* payload, unsigned int length);
PubSubClient mqtt_client(mqtt_server, mqtt_port, callback, secure_presentation_layer);

void setup() {
  SerialMon.begin(115200);
  delay(5000);
  Serial.println("\n\nWelcome to JP Learning\n");

  // Set SIM module baud rate and UART pins
  SerialAT.begin(MODEM_UART_BAUD, SERIAL_8N1, MODEM_RX, MODEM_TX);

  //Add CA Certificate
  secure_presentation_layer.setCACert(root_ca);

  // SIM modem initial setup
  setupModem();
}
void loop() {
  SerialMon.print("Initializing modem...");
  if (!sim_modem.init()) {
    SerialMon.print(" fail... restarting modem...");
    setupModem();
    // Restart takes quite some time
    // Use modem.init() if you don't need the complete restart
    if (!sim_modem.restart()) {
      SerialMon.println(" fail... even after restart");
      return;
    }
  }
  SerialMon.println(" OK");

  // General information
  String name = sim_modem.getModemName();
  Serial.println("Modem Name: " + name);
  String modem_info = sim_modem.getModemInfo();
  Serial.println("Modem Info: " + modem_info);

  //  // Unlock your SIM card with a PIN if needed
  //  if (strlen(simPIN) && sim_modem.getSimStatus() != 3)
  //  {
  //    sim_modem.simUnlock(simPIN);
  //  }

  // Set modes
  /*
    2 Automatic
    13 GSM only
    38 LTE only
    51 GSM and LTE only
  * * * */
  //  sim_modem.setNetworkMode(2);
  //  delay(3000);
  /*
    1 CAT-M
    2 NB-Iot
    3 CAT-M and NB-IoT
  * * */
  //  sim_modem.setPreferredMode(3);
  //  delay(3000);

  // Wait for network availability
  SerialMon.print("Waiting for network...");
  if (!sim_modem.waitForNetwork()) {
    SerialMon.println(" fail");
    delay(10000);
    return;
  }
  SerialMon.println(" OK");

  // Connect to the GPRS network
  SerialMon.print("Connecting to network...");
  if (!sim_modem.isNetworkConnected())  {
    SerialMon.println(" fail");
    delay(10000);
    return;
  }
  SerialMon.println(" OK");

  // Connect to APN
  SerialMon.print("Connecting to APN: ");
  SerialMon.print(apn);
  if (!sim_modem.gprsConnect(apn, gprs_user, gprs_pass)) {
    SerialMon.println(" fail");
    return;
  }
  digitalWrite(LED_PIN, HIGH);
  SerialMon.println(" OK");

  // More info..
  Serial.println("");
  String ccid = sim_modem.getSimCCID();
  Serial.println("CCID: " + ccid);
  String imei = sim_modem.getIMEI();
  Serial.println("IMEI: " + imei);
  String cop = sim_modem.getOperator();
  Serial.println("Operator: " + cop);
  IPAddress local = sim_modem.localIP();
  Serial.println("Local IP: " + String(local));
  int csq = sim_modem.getSignalQuality();
  Serial.println("Signal quality: " + String(csq));

  // MQTT Test loop
  // As long as we have connectivity
  while (sim_modem.isGprsConnected()) {
    // We maintain connectivity with the broker
    if (!mqtt_client.connected())
      reconnect();
    // We are listening to the events
    mqtt_client.loop();
  }

  // Disconnect GPRS and PowerOff
  // Apparently the "gprsDisconnect()" method (TinyGSM) are not working well with the SIM7600...
  // ...you have to use additionally "poweroff()".
  // With that, the modem can be connected again in the next cycle of the loop.
  //sim_modem.gprsDisconnect();
  //sim_modem.poweroff();

  delay(15000);
}
void turnModemOn() {
  digitalWrite(LED_PIN, LOW);

  pinMode(MODEM_PWRKEY, OUTPUT);
  digitalWrite(MODEM_PWRKEY, LOW);
  delay(1000); //Datasheet Ton mintues = 1S
  digitalWrite(MODEM_PWRKEY, HIGH);
}
void turnModemOff() {
  digitalWrite(MODEM_PWRKEY, LOW);
  delay(1500); //Datasheet Ton mintues = 1.2S
  digitalWrite(MODEM_PWRKEY, HIGH);

  digitalWrite(LED_PIN, LOW);
}
void setupModem() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(MODEM_PWRKEY, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  turnModemOff();
  delay(1000);
  turnModemOn();
  delay(5000);
}
// To connect to the broker
void reconnect() {
  // Loop until we're reconnected
  while (!mqtt_client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqtt_client.connect(mqtt_clientId, mqtt_user, mqtt_password)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      mqtt_client.publish(topic_publish, "hello world");
      // ... and resubscribe
      mqtt_client.subscribe(topic_subscribe);
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println("...try again in 5 seconds");
      delay(5000);
    }
  }
}
// For read the MQTT events
void callback(char *topic, uint8_t *payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
    Serial.print((char)payload[i]);
  Serial.println();
}
