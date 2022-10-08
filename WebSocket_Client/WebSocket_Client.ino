#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>

// Wi-Fi Credentials
const char* ssid = "Your SSID";
const char* password = "Your Password";

// Server Details
//String serverHost = "Your PC IP";
//int serverPort = 8080;
String serverHost = "jiyepakistanlearning.herokuapp.com";
int serverPort = 80;

// Data Sending Time
unsigned long CurrentMillis, PreviousMillis, DataSendingTime = (unsigned long) 1000 * 10;

WebSocketsClient webSocket;

bool clientConnect = false;
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("\n[WSc] Disconnected!\n");
      clientConnect = false;
      break;
    case WStype_CONNECTED: {
        Serial.printf("\n[WSc] Connected to url: %s\n", payload);

        // send message to server when Connected
        webSocket.sendTXT("Connected");
        clientConnect = true;
      }
      break;
    case WStype_TEXT:
      Serial.printf("\n[WSc] get text: %s\n", payload);

      // send message to server
      // webSocket.sendTXT("message here");
      break;
    case WStype_BIN:
      Serial.printf("\n[WSc] get binary length: %u\n", length);
      hexdump(payload, length);

      // send data to server
      // webSocket.sendBIN(payload, length);
      break;
    case WStype_PING:
      // pong will be send automatically
      Serial.printf("[WSc] get ping\n");
      break;
    case WStype_PONG:
      // answer to a ping we send
      Serial.printf("\n[WSc] get pong\n");
      break;
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\nWelcome to JP Learning\n");
  setup_wifi();

  // server address, port, URL and protocol // URL and Protocol should not be change
  webSocket.begin(serverHost, serverPort, "/", "jp-learning");

  // event handler
  webSocket.onEvent(webSocketEvent);

  //	// use HTTP Basic Authorization this is optional remove if not needed
  //  	webSocket.setAuthorization("user", "Password");

  // try ever 5000 again if connection has failed
  webSocket.setReconnectInterval(5000);

  // start heartbeat (optional)
  // ping server every 15000 ms
  // expect pong from server within 3000 ms
  // consider connection disconnected if pong is not received 2 times
  //    webSocket.enableHeartbeat(15000, 3000, 2);
}

void loop() {
  webSocket.loop();

  CurrentMillis = millis();
  if (CurrentMillis - PreviousMillis > DataSendingTime) {
    PreviousMillis = CurrentMillis;

    if (clientConnect) {
      String data = "Random from JPLearning's ESP8266 is " + String(random(1, 999999));
      Serial.println("\n\nSending data: " + data);
      webSocket.sendTXT(data.c_str());
    }
  }
}
void setup_wifi() {
  Serial.print("\nConnecting to ");
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
