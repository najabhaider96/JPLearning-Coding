#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Wi-Fi Credentials
const char* ssid = "Your SSID";
const char* password = "Your Password";

// Access Point Credentials
const char* ap_ssid = "JP Learning";
const char* ap_password = "123456789";

// LED PINs Define
#define LED_WHITE_PIN 14
#define LED_RED_PIN 12
#define LED_GREEN_PIN 13

bool LED_WHITE_STATUS, LED_RED_STATUS, LED_GREEN_STATUS;

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(LED_WHITE_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  Serial.println("\n\nWelcome to JP Learning\n");

  // Configure Access Point Mode
  setup_ap();
  // Configure Station Mode
  setup_wifi();

  // Define all routes
  server.on("/", handle_OnConnect);
  server.on("/led_white_on", led_white_on);
  server.on("/led_white_off", led_white_off);
  server.on("/led_red_on", led_red_on);
  server.on("/led_red_off", led_red_off);
  server.on("/led_green_on", led_green_on);
  server.on("/led_green_off", led_green_off);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient(); 
  
  if (LED_WHITE_STATUS)
    digitalWrite(LED_WHITE_PIN, HIGH);
  else
    digitalWrite(LED_WHITE_PIN, LOW);

  if (LED_RED_STATUS)
    digitalWrite(LED_RED_PIN, HIGH);
  else
    digitalWrite(LED_RED_PIN, LOW);

  if (LED_GREEN_STATUS)
    digitalWrite(LED_GREEN_PIN, HIGH);
  else
    digitalWrite(LED_GREEN_PIN, LOW);
}
void setup_ap() {
  Serial.println("\n\nConfiguring access point...");
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
  LED_WHITE_STATUS = LOW;
  LED_RED_STATUS = LOW;
  LED_GREEN_STATUS = LOW;
  Serial.println("Client Connected");
  server.send(200, "text/html", SendHTML(LED_WHITE_STATUS, LED_RED_STATUS, LED_GREEN_STATUS));
}

void led_white_on() {
  LED_WHITE_STATUS = HIGH;
  Serial.println("WHITE LED: ON");
  server.send(200, "text/html", SendHTML(LED_WHITE_STATUS, LED_RED_STATUS, LED_GREEN_STATUS));
}

void led_white_off() {
  LED_WHITE_STATUS = LOW;
  Serial.println("WHITE LED: OFF");
  server.send(200, "text/html", SendHTML(LED_WHITE_STATUS, LED_RED_STATUS, LED_GREEN_STATUS));
}

void led_red_on() {
  LED_RED_STATUS = HIGH;
  Serial.println("RED LED: ON");
  server.send(200, "text/html", SendHTML(LED_WHITE_STATUS, LED_RED_STATUS, LED_GREEN_STATUS));
}

void led_red_off() {
  LED_RED_STATUS = LOW;
  Serial.println("RED LED: OFF");
  server.send(200, "text/html", SendHTML(LED_WHITE_STATUS, LED_RED_STATUS, LED_GREEN_STATUS));
}

void led_green_on() {
  LED_GREEN_STATUS = HIGH;
  Serial.println("GREEN LED: ON");
  server.send(200, "text/html", SendHTML(LED_WHITE_STATUS, LED_RED_STATUS, LED_GREEN_STATUS));
}

void led_green_off() {
  LED_GREEN_STATUS = LOW;
  Serial.println("GREEN LED: OFF");
  server.send(200, "text/html", SendHTML(LED_WHITE_STATUS, LED_RED_STATUS, LED_GREEN_STATUS));
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t LED_WHITE_STATUS, uint8_t LED_RED_STATUS, uint8_t LED_GREEN_STATUS) {
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
  
  if (LED_WHITE_STATUS)
    html += "<p>LED WHITE STATUS: ON</p><a class=\"button button-off\" href=\"/led_white_off\">OFF</a>\n";
  else
    html += "<p>LED WHITE STATUS: OFF</p><a class=\"button button-on\" href=\"/led_white_on\">ON</a>\n";

  if (LED_RED_STATUS)
    html += "<p>LED RED STATUS: ON</p><a class=\"button button-off\" href=\"/led_red_off\">OFF</a>\n";
  else
    html += "<p>LED RED STATUS: OFF</p><a class=\"button button-on\" href=\"/led_red_on\">ON</a>\n";

  if (LED_GREEN_STATUS)
    html += "<p>LED GREEN STATUS: ON</p><a class=\"button button-off\" href=\"/led_green_off\">OFF</a>\n";
  else
    html += "<p>LED GREEN STATUS: OFF</p><a class=\"button button-on\" href=\"/led_green_on\">ON</a>\n";

  html += "</body>\n";
  html += "</html>\n";
  return html;
}
