byte LED_PIN = 5;
void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  Serial.println("Firmware Start"); 
}
void loop() {
  Serial.println("LED ON"); 
  digitalWrite(LED_PIN, HIGH);
  delay(1000);        
  Serial.println("LED OFF");
  digitalWrite(LED_PIN, LOW);
  delay(1000);                  
}
