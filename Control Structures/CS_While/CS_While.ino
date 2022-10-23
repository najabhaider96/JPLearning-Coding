/*
Welcome to JP Learning
*/
void setup () {
  Serial.begin(115200);
  byte var = 0;
  while (var < 200) {
    // do something repetitive 200 times
    Serial.print("var is : ");
    Serial.println(var);
    var++;
  }
}
void loop() {
}
