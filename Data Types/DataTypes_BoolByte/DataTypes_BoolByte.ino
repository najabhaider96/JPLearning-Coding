byte LED_PIN = 5;
bool running = true;
byte a = 257;
void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  if (running == true) {
    Serial.println("LED ON");
    digitalWrite(LED_PIN, HIGH);
  } else if (running == false) {
    Serial.println("LED OFF");
    digitalWrite(LED_PIN, LOW);
  }
  Serial.print("a = ");
  Serial.println(a);
  delay(1000);
}
