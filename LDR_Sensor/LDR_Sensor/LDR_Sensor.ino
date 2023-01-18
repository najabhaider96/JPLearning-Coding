/*
  Welcome to JP Learning
*/
byte Sensor_Pin = 4, LED_Pin = 5;
void setup() {
  Serial.begin(115200);
  pinMode(Sensor_Pin, INPUT);
  pinMode(LED_Pin, OUTPUT);
  Serial.println("\n\nWelcome to JP Learning\n");
}
void loop() {
  bool Sensor_State = digitalRead(Sensor_Pin);
  int Senor_Value = analogRead(A0);
  Serial.println("\nSensor_State: " + String(Sensor_State));
  Serial.println("Senor_Value: " + String(Senor_Value));

  //  // When output is Digital
  //  if (!Sensor_State) {
  //    digitalWrite(LED_Pin, HIGH);
  //    Serial.println("Light Detected");
  //  } else {
  //    digitalWrite(LED_Pin, LOW);
  //    Serial.println("Light Not Detected");
  //  }

  // When output is Analog
  if (Senor_Value < 1000) {
    digitalWrite(LED_Pin, HIGH);
    Serial.println("Light Detected");
  } else {
    digitalWrite(LED_Pin, LOW);
    Serial.println("Light Not Detected");
  }

  delay(100);
}
