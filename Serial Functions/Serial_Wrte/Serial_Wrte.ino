/*
Welcome to JP Learning
*/
void setup() {
  Serial.begin(9600);
  Serial.write(45);     // write a byte with the value 45 => '-' character     
  Serial.write("ArduinoGetStarted.com\n");
  byte buf[] = {'A', 'r', 'd', 'u', 'i', 'n', 'o'};
  Serial.write(buf, 7); // write an array
}
void loop() {
}
