/*
  Welcome to JP Learning
*/
// /*
#include <ESP8266WiFi.h>

// Wi-Fi Credentials
const char* ssid = "Your SSID";
const char* password = "You Password";

// Analyzer configuration values
uint8_t slaveIp[] = { 192, 168, 0, 52 };
uint16_t slavePort = 502;

WiFiClient slave;

uint8_t sendBuffer_Freq[] = { 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x01, 0x03, 0x80, 0x0C, 0x00, 0x02 };  // Frequency
uint8_t sendBuffer_V1[] = { 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x01, 0x03, 0x80, 0x06, 0x00, 0x02 };  // Phase voltage U1
uint8_t sendBuffer_V2[] = { 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x01, 0x03, 0x80, 0x08, 0x00, 0x02 };  // Phase voltage U2
uint8_t sendBuffer_V3[] = { 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x01, 0x03, 0x80, 0x0A, 0x00, 0x02 };  // Phase voltage U3
uint8_t sendBuffer_I1[] = { 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x01, 0x03, 0x80, 0x0E, 0x00, 0x02 };  // Effective value current I1
uint8_t sendBuffer_I2[] = { 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x01, 0x03, 0x80, 0x10, 0x00, 0x02 };  // Effective value current I2
uint8_t sendBuffer_I3[] = { 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x01, 0x03, 0x80, 0x12, 0x00, 0x02 };  // Effective value current I3

byte no_Byte, incomingByte[13] = { 0 };

uint32_t _lastRequestTime;
uint32_t _timeout = 1000;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\nWelcome to JP Learning\n");
  setup_wifi();
}

void loop() {
  if (!slave.connected()) {
    Serial.println("Slave Not Connected");
    slave.stop();
    slave.connect(slaveIp, slavePort);
  }

  if (slave.connected()) {
    Serial.println("\n\nRequest Data");
    float freq = readData(sendBuffer_Freq);
    float v1 = readData(sendBuffer_V1);
    float v2 = readData(sendBuffer_V2);
    float v3 = readData(sendBuffer_V3);
    float c1 = readData(sendBuffer_I3);
    float c2 = readData(sendBuffer_I3);
    float c3 = readData(sendBuffer_I3);

    Serial.println("\n\nFrequency = " + String(freq, 2) + " Hz");
    Serial.println("V1 = " + String(v1, 2) + " V");
    Serial.println("V2 = " + String(v2, 2) + " V");
    Serial.println("V3 = " + String(v3, 2) + " V");
    Serial.println("C1 = " + String(c1, 2) + " A");
    Serial.println("C2 = " + String(c2, 2) + " A");
    Serial.println("C3 = " + String(c3, 2) + " A");
    Serial.println();
  }
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
float readData(byte sendBuffer[]) {
  float value = -1;
  if (sendRequest(sendBuffer)) {
    available();
    if (no_Byte >= 13) {
      String incomingBytesInString[no_Byte];
      // Serial.println("\n\nno_Byte = " + String(no_Byte));
      for (int i = 0; i < no_Byte; i++) {
        // Serial.print("incomingByte[" + String(i) + "] = ");
        // Serial.println(String(incomingByte[i]) + " " + String(incomingByte[i], HEX));

        if (String(incomingByte[i]).length() == 1)
          incomingBytesInString[i] = "0" + String(incomingByte[i], HEX);
        else
          incomingBytesInString[i] = String(incomingByte[i], HEX);
      }

      String data1 = incomingBytesInString[9] + incomingBytesInString[10];
      String data2 = incomingBytesInString[11] + incomingBytesInString[12];
      value = DecTofloat(HexToDec(data2 + data1));
      no_Byte = 0;
    }
  }
  return value;
}
bool sendRequest(byte sendBuffer[]) {
  // Ignore pending data
  while (slave.available()) {
    slave.read();
  }
  // Send the packet
  int ret = slave.write(sendBuffer, 12);
  // Serial.println("ret = " + String(ret));
  bool res = false;
  if (ret == 12) {
    res = true;
    _lastRequestTime = millis();
  }

  return res;
}
void available() {
  while (true) {
    if (slave.available()) {
      do {
        byte temp = slave.read();
        Serial.println("Response[" + String(no_Byte) + "]: " + String(temp, DEC) + " " + String(temp, HEX));
        incomingByte[no_Byte] = temp;
        no_Byte++;
      } while (slave.available());
      break;
    } else if (millis() - _lastRequestTime > _timeout) {
      Serial.println("Read timout");
      break;
    }
  }
}
int HexToDec(String hexString) {
  int decValue = 0, nextInt;
  for (int i = 0; i < hexString.length(); i++) {
    nextInt = int(hexString.charAt(i));
    if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
    if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
    if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
    nextInt = constrain(nextInt, 0, 15);
    decValue = (decValue * 16) + nextInt;
  }
  return decValue;
}
float DecTofloat(uint32_t x) {
  return (*(float*)&x);
}
