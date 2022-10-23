/*
Welcome to JP Learning
*/
long countUp = 0;            //creates a variable integer called 'countUp'

void setup() {
  Serial.begin(115200);       // use the serial port to print the number
}

void loop() {
  countUp++;                //Adds 1 to the countUp int on every loop
  Serial.println(countUp);  // prints out the current state of countUp
  delay(1000);
}
