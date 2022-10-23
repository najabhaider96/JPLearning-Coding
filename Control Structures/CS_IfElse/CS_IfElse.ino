/*
Welcome to JP Learning
*/
byte LED_PIN = 5;
void setup () {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
}
void loop() {
  int x = readSensors();
  Serial.println("x = " + String(x));
  if (x > 4) {
    Serial.println("LED ON");
    digitalWrite(LED_PIN, HIGH);
  } else if (x < 2) {
    Serial.println("LED OFF");
    digitalWrite(LED_PIN, LOW);
  } else {
    Serial.println("Last State");
  }
  delay(1000);
}
int readSensors() {
  return random(1, 5);
}
