/*
Welcome to JP Learning
*/
#define TRIGGER_PIN 12
#define ECHO_PIN 14

float distanceCentimeter, distanceInch;

void setup() {
  Serial.begin(115200);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  delay(5000);
  Serial.println("\n\nWelcome to JP Learning\n");
}
void loop() {
  // Clears the trigPin condition
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  float duration = pulseIn(ECHO_PIN, HIGH);
  
  /*
     Speed of sound v = 340 m/s or 0.034 cm/µs
     Formula = (high level time × velocity of sound (0.034 cm/µs) / 2
  */
  
  // Calculating the distance in cm
  distanceCentimeter = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Calculating the distance in inches (in = cm * 0.393701)
  distanceInch = distanceCentimeter * 0.393701;

  Serial.println("Distance = " + String(distanceCentimeter) + " cm, " + String(distanceInch) + " in");
  delay(1000);
}
