byte LED_PIN = 5, SENSOR_PIN = 2;
void setup () {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(SENSOR_PIN, INPUT);
}
void loop() {
  byte data = digitalRead(SENSOR_PIN);  // Read motion sensor
  Serial.println("\ndata = " + String(data));
  if (data == 1) {
    Serial.println("LED ON");
    digitalWrite(LED_PIN, HIGH);
  } else if (data != 1) {
    Serial.println("LED OFF");
    digitalWrite(LED_PIN, LOW);
  }
  delay(1000);
}
