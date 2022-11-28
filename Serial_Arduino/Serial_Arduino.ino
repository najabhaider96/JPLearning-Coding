/*
Welcome to JP Learning
*/
void setup() {
  Serial.begin(115200);
  Serial.println("\n\nWelcome to JP Learning\n");
  Serial.println("Receiver Testing");
}
void loop() {
  while (Serial.available() > 0) {
    char data = Serial.read();
    Serial.print(data);
  }
}
