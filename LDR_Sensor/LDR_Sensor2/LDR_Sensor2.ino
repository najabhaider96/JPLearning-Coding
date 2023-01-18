/*
  Welcome to JP Learning
*/
byte Sensor_Pin = 4, LED_Pin1 = 12, LED_Pin2 = 13, LED_Pin3 = 14;
void setup() {
  Serial.begin(115200);
  pinMode(Sensor_Pin, INPUT);
  pinMode(LED_Pin1, OUTPUT);
  pinMode(LED_Pin2, OUTPUT);
  pinMode(LED_Pin3, OUTPUT);
  Serial.println("\n\nWelcome to JP Learning\n");
}
void loop() {
  bool Sensor_State = digitalRead(Sensor_Pin);
  int Senor_Value = analogRead(A0);
  Serial.println("\nSensor_State: " + String(Sensor_State));
  Serial.println("Senor_Value: " + String(Senor_Value));

  // When output is Analog
  if (Senor_Value < 500) {
    digitalWrite(LED_Pin1, HIGH);
    digitalWrite(LED_Pin2, LOW);
    digitalWrite(LED_Pin3, LOW);
    Serial.println("Brightness is HIGH");
  } else if (Senor_Value >= 500 && Senor_Value < 800) {
    digitalWrite(LED_Pin1, LOW);
    digitalWrite(LED_Pin2, HIGH);
    digitalWrite(LED_Pin3, LOW);
    Serial.println("Brightness is Medium");
  } else if (Senor_Value >= 800) {
    digitalWrite(LED_Pin1, LOW);
    digitalWrite(LED_Pin2, LOW);
    digitalWrite(LED_Pin3, HIGH);
    Serial.println("Brightness is LOW");
  }

  delay(100);
}
