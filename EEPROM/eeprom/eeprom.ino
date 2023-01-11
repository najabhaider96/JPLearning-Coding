/*
  Welcome to JP Learning
*/
#include <EEPROM.h>

void setup() {
  Serial.begin(115200);
  Serial.println("\n\nWelcome to JP Learning\n");
}

void loop() {
  clear();
  delay(1000);
  write(10, "JP Learning*");
  write(100, "Internet*");
  write(200, "jplearning*");
  delay(1000);
  String data = read(10);
  Serial.println("\ndata: " + data);
  String ssid = read(100);
  Serial.println("\nssid: " + ssid);
  String password = read(200);
  Serial.println("\npassword: " + password);
  delay(1000);
  String data2 = read(10, 300);
  Serial.println("\ndata2: " + data2);
  delay(5000);
}

void clear() {
  EEPROM.begin(512);
  for (int i = 0; i < 512; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
  EEPROM.end();
  Serial.println("EEPROM Cleared");
}
void write(int index, String str) {
  EEPROM.begin(512);
  int i = 0;
  while (i < str.length()) {
    Serial.println("i: " + String(i) + ", index: " + String(index) + ", str[i]: " + str[i]);
    EEPROM.write(index, str[i]);
    index++;
    i++;
  }
  EEPROM.commit();
  EEPROM.end();
  Serial.println("EEPROM Write");
}
String read(int index) {
  EEPROM.begin(512);
  String str = "";
  for (int i = index; i < 512; i++) {
    char temp = EEPROM.read(i);
    if (temp == '*')
      break;
    if (temp != NULL)
      str += temp;
  }
  EEPROM.end();
  return str;
}
String read(int Start_Index, int End_Index) {
  EEPROM.begin(512);
  String str = "";
  for (int i = Start_Index; i <= End_Index; i++) {
    char temp = EEPROM.read(i);
    if (temp == '*')
      break;
    if (temp != NULL)
      str += temp;
  }
  EEPROM.end();
  return str;
}
