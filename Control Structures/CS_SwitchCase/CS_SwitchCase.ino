/*
Welcome to JP Learning
*/
void setup () {
  Serial.begin(115200);
}
void loop() {
  int x = readSensors();
  Serial.println("x = " + String(x));
  switch (x) {
    case 1:
      Serial.println("case 1: x is equal to 1");
      break;
    case 2:
      Serial.println("case 2: x is equal to 2");
      break;
    case 3:
      Serial.println("case 3: x is equal to 3");
      break;
    case 4:
      Serial.println("case 4: x is equal to 4");
      break;
    default:
      Serial.println("default: x is equal to 5");
      break;
  }
  delay(1000);
}
int readSensors() {
  return random(1, 5);
}
