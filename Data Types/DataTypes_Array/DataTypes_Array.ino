int myInts[6];
int myPins[] = {2, 4, 8, 3, 6};
int mySensVals[5] = {2, 4, -8, 3, 2};
float mySensValsFloat[5] = {3.5, 4.0, -8.1, 3.9, 2.2};
char message[6] = "hello";
String message2[3] = {"Arduino1", "Arduino2", "Arduino3"};
void setup() {
  Serial.begin(115200);
}
void loop() {
  myInts[0] = 5;
  myInts[5] = 16;
  Serial.println(myInts[5]);
  Serial.println(myPins[4]);
  Serial.println(mySensVals[3]);
  Serial.println(mySensValsFloat[1]);
  Serial.println(message[0]);
  Serial.println(message2[2]);
  delay(1000);
}
