/*
Welcome to JP Learning
*/
void setup () {
  Serial.begin(115200);
  int x = 0;
  do {
    delay(50);          // wait for sensors to stabilize
    x = readSensors();  // check the sensors
    Serial.println("x = " + String(x));
  } while (x < 3);
}
void loop() {
  Serial.println("Loop");
  delay(1000); 
}
int a = 0;
int readSensors() {
  return a++;
}
