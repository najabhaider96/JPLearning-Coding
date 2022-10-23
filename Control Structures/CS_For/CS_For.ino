void setup () {
  Serial.begin(115200);
  for (int i = 0; i < 7; i++) {
    Serial.print("i is : ");
    Serial.println(i);
  }
}
void loop() {
}
