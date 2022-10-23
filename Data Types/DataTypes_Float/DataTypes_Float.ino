/*
Welcome to JP Learning
*/
float myfloat;
float sensorCalbrate = 1.117;
int x;
int y;
double z;
void setup() {
  Serial.begin(115200); 
  x = 1;
  y = x / 2;          // y now contains 0, ints can't hold fractions
  z = (float)x / 2.0; // z now contains .5 (you have to use 2.0, not 2)
}

void loop() {
  Serial.print("\nx = ");
  Serial.println(x);
  Serial.print("y = ");
  Serial.println(y);
  Serial.print("z = ");
  Serial.println(z);
  delay(1000);
}
