int x = 0;
byte LED_PIN = 5;
void setup () {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
}
void loop() {
  x = readSensors();  // check the sensors
  if (x > 3) {
    Serial.println("LED ON");
    digitalWrite(LED_PIN, HIGH);
  }
  delay(1000);
}
int a = 0;
int readSensors() {
  return a++;
}
