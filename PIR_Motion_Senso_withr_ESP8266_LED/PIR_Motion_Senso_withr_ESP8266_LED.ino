byte PIR_SENSOR_PIN = 4, LED_PIN = 5;
void setup () {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(PIR_SENSOR_PIN, INPUT);
}
void loop() {
  byte data = digitalRead(PIR_SENSOR_PIN);  // Read motion sensor
  Serial.println("\ndata = " + String(data));
  if (data == 1) {
    Serial.println("LED ON");
    digitalWrite(LED_PIN, HIGH);
  } else if (data == 0) {
    Serial.println("LED OFF");
    digitalWrite(LED_PIN, LOW);
  }
  delay(1000);
}
