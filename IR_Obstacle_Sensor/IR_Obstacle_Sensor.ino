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
  Serial.println("\nSensor_State: " + String(Sensor_State));
  if (!Sensor_State) {
    digitalWrite(LED_Pin, HIGH);
    Serial.println("Obstacle Detected");
  } else {
    digitalWrite(LED_Pin, LOW);
    Serial.println("No Obstacle Detected");
  }
  delay(100);
}
