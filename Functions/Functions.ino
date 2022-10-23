byte LED_PIN1 = 5, LED_PIN2 = 4;
byte SENSOR_PIN1 = 2, SENSOR_PIN2 = 3;

//// No Argument Passed and No Return Value
//void readSensors(void);

//// No Arguments Passed But Returns a Value
//bool readSensors(void);

//// Argument Passed But No Return Value
//void readSensors(byte);

//// Argument Passed and Returns a Value
//bool readSensors(byte);

// Argument Passed and Returns a Value
bool readSensors(byte, byte);

void setup () {
  Serial.begin(115200);
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  pinMode(SENSOR_PIN1, INPUT);
  pinMode(SENSOR_PIN2, INPUT);
}
void loop() {
  //  readSensors();

  //  bool data1 = readSensors();
  //  Serial.println("\ndata1 = " + String(data1));
  //  if (data1 == 1) {
  //    Serial.println("LED1 ON");
  //    digitalWrite(LED_PIN1, HIGH);
  //  } else if (data1 != 1) {
  //    Serial.println("LED1 OFF");
  //    digitalWrite(LED_PIN1, LOW);
  //  }

  //  readSensors(SENSOR_PIN1);
  //  delay(100);
  //  readSensors(SENSOR_PIN2);

//  bool data1 = readSensors(SENSOR_PIN1);
//  Serial.println("\ndata1 = " + String(data1));
//  if (data1 == 1) {
//    Serial.println("LED1 ON");
//    digitalWrite(LED_PIN1, HIGH);
//  } else if (data1 != 1) {
//    Serial.println("LED1 OFF");
//    digitalWrite(LED_PIN1, LOW);
//  }
//
//  bool data2 = readSensors(SENSOR_PIN2);
//  Serial.println("\ndata2 = " + String(data2));
//  if (data2 == 1) {
//    Serial.println("LED2 ON");
//    digitalWrite(LED_PIN2, HIGH);
//  } else if (data2 != 1) {
//    Serial.println("LED2 OFF");
//    digitalWrite(LED_PIN2, LOW);
//  }
  
  bool data1 = readSensors(SENSOR_PIN1, LED_PIN1);
  Serial.println("\ndata1 = " + String(data1));
  
  bool data2 = readSensors(SENSOR_PIN2, LED_PIN2);
  Serial.println("\ndata2 = " + String(data2));

  delay(1000);
}

//// No Argument Passed and No Return Value
//void readSensors(void) {
//  Serial.println("\nvoid readSensors()");
//
//  bool data1 = digitalRead(SENSOR_PIN1);  // Read sensor
//  Serial.println("\ndata1 = " + String(data1));
//
//  if (data1 == 1) {
//    Serial.println("LED1 ON");
//    digitalWrite(LED_PIN1, HIGH);
//  } else if (data1 != 1) {
//    Serial.println("LED1 OFF");
//    digitalWrite(LED_PIN1, LOW);
//  }
//}

//// No Arguments Passed But Returns a Value
//bool readSensors(void) {
//  Serial.println("\nbool readSensors()");
//
//  bool data = digitalRead(SENSOR_PIN1);  // Read sensor
//  Serial.println("\ndata = " + String(data));
//  return data;
//}

//// Argument Passed But No Return Value
//void readSensors(byte SENSOR_PIN) {
//  Serial.println("\nvoid readSensors(byte SENSOR_PIN)");
//
//  byte data = digitalRead(SENSOR_PIN);  // Read sensor
//  Serial.println("\ndata = " + String(data));
//}

//// Argument Passed and Returns a Value
//bool readSensors(byte SENSOR_PIN) {
//  Serial.println("\nvoid readSensors(byte SENSOR_PIN)");
//
//  bool data = digitalRead(SENSOR_PIN);  // Read sensor
//  Serial.println("\ndata = " + String(data));
//  return data;
//}

// Argument Passed and Returns a Value
bool readSensors(byte SENSOR_PIN, byte LED_PIN) {
  Serial.println("\nvoid readSensors(byte SENSOR_PIN)");

  bool data = digitalRead(SENSOR_PIN);  // Read sensor
  Serial.println("\ndata = " + String(data));
  if (data == 1) {
    Serial.println("LED ON");
    digitalWrite(LED_PIN, HIGH);
  } else if (data != 1) {
    Serial.println("LED OFF");
    digitalWrite(LED_PIN, LOW);
  }
  return data;
}
