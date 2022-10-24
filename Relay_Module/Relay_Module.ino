#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Wi-Fi Credentials
const char* ssid = "Your SSID";
const char* password = "Your Password";

// Access Point Credentials
const char* ap_ssid = "JP Learning";
const char* ap_password = "123456789";

// LED PINs Define
#define RELAY1_PIN 4
#define RELAY2_PIN 5

bool RELAY1_STATUS, RELAY2_STATUS;

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  digitalWrite(RELAY1_PIN, HIGH);
  digitalWrite(RELAY2_PIN, HIGH);
  Serial.println("\n\nWelcome to JP Learning\n");

  // Configure Access Point Mode
  setup_ap();
  // Configure Station Mode
  setup_wifi();

  // Define all routes
  server.on("/", handle_OnConnect);
  server.on("/relay1_on", RELAY1_on);
  server.on("/relay1_off", RELAY1_off);
  server.on("/relay2_on", RELAY2_on);
  server.on("/relay2_off", RELAY2_off);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient();

  if (RELAY1_STATUS)
    digitalWrite(RELAY1_PIN, LOW);
  else
    digitalWrite(RELAY1_PIN, HIGH);

  if (RELAY2_STATUS)
    digitalWrite(RELAY2_PIN, LOW);
  else
    digitalWrite(RELAY2_PIN, HIGH);
}
void setup_ap() {
  Serial.println("\n\nConfiguring access point...");
  Serial.println("SSID = \"" + String(ap_ssid) + "\", PASSWORD = \"" + String(ap_password) + "\"");
  WiFi.softAP(ap_ssid, ap_password);
  delay(100);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
}
void setup_wifi() {
  Serial.println();
  Serial.print("\n\nConnecting to ");
  Serial.println("\"" + String(ssid) + "\"");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("STA IP address: ");
  Serial.println(WiFi.localIP());
}

void handle_OnConnect() {
  RELAY1_STATUS = LOW;
  RELAY2_STATUS = LOW;
  Serial.println("Client Connected");
  server.send(200, "text/html", SendHTML(RELAY1_STATUS, RELAY2_STATUS));
}

void RELAY1_on() {
  RELAY1_STATUS = HIGH;
  Serial.println("RELAY 1: ON");
  server.send(200, "text/html", SendHTML(RELAY1_STATUS, RELAY2_STATUS));
}

void RELAY1_off() {
  RELAY1_STATUS = LOW;
  Serial.println("RELAY 1: OFF");
  server.send(200, "text/html", SendHTML(RELAY1_STATUS, RELAY2_STATUS));
}

void RELAY2_on() {
  RELAY2_STATUS = HIGH;
  Serial.println("RELAY 2: ON");
  server.send(200, "text/html", SendHTML(RELAY1_STATUS, RELAY2_STATUS));
}

void RELAY2_off() {
  RELAY2_STATUS = LOW;
  Serial.println("RELAY 2: OFF");
  server.send(200, "text/html", SendHTML(RELAY1_STATUS, RELAY2_STATUS));
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t RELAY1_STATUS, uint8_t RELAY2_STATUS) {
  String html = "<!DOCTYPE html> <html>\n";
  html += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  html += "<title>Lgihts Control</title>\n";
  html += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  html += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h2 {color: #444444;margin-bottom: 50px;}\n";
  html += ".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 30px;}\n";
  html += ".button-on {background-color: #00afd3;}\n";
  html += ".button-off {background-color: #5766ae;}\n";
  html += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  html += "</style>\n";
  html += "</head>\n";
  html += "<body>\n";
  html += "<h1>Welcome to JP Learning</h1>\n";
  html += "<h2>ESP8266 Web Server For Lights Control</h2>\n";

  if (RELAY1_STATUS)
    html += "<p>RELAY 1 STATUS: ON</p><a class=\"button button-off\" href=\"/relay1_off\">OFF</a>\n";
  else
    html += "<p>RELAY 1 STATUS: OFF</p><a class=\"button button-on\" href=\"/relay1_on\">ON</a>\n";

  if (RELAY2_STATUS)
    html += "<p>RELAY 2 STATUS: ON</p><a class=\"button button-off\" href=\"/relay2_off\">OFF</a>\n";
  else
    html += "<p>RELAY 2 STATUS: OFF</p><a class=\"button button-on\" href=\"/relay2_on\">ON</a>\n";

  html += "</body>\n";
  html += "</html>\n";
  return html;
}
