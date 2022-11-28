/*
Welcome to JP Learning
*/
int count = 0;
void setup() {
  Serial.begin(115200);
  Serial.println("\n\nWelcome to JP Learning\n");
  Serial.println("Transmitter Testing");
}
void loop() {
  count++;
//  Serial.write("\nHi, msg from ESP8266");
//  Serial.println("Hi, msg from ESP8266");
  Serial.println(count);
  delay(2000);
}
