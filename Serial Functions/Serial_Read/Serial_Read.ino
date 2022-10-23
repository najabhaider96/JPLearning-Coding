/*
Welcome to JP Learning
*/
void setup() {
  Serial.begin(9600);
}
void loop() {
  if (Serial.available() > 0) {
    String teststr = Serial.readStringUntil(',');
    Serial.println("I received: " + teststr);
  }
}
