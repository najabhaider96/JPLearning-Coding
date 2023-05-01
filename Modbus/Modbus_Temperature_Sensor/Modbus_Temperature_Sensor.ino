/*
  Welcome to JP Learning
*/
#include <SoftwareSerial.h>

// GPIO Pins
byte TX_PIN = 4, RX_PIN = 5;
byte DE_RE_PIN = 16, LED_PIN = 2;

SoftwareSerial Soft_Serial(RX_PIN, TX_PIN);

// Variables
byte no_Byte, incomingByte[9] = { 0 };

// Modbus Request Bytes
byte type = 2; // (1=Humidity, 2=Temperature, 3=Humidity and Temperature)

// byte sendBuffer[] = { 0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A };  // Valid (Single Read for Humidity)
// byte sendBuffer[] = { 0x01, 0x04, 0x00, 0x00, 0x00, 0x01, 0x31, 0xCA };  // Invalid (Single Read for Humidity)


byte sendBuffer[] = { 0x01, 0x03, 0x00, 0x01, 0x00, 0x01, 0xD5, 0xCA };  // Valid (Single Read for Temperature)
// byte sendBuffer[] = { 0x01, 0x04, 0x00, 0x01, 0x00, 0x01, 0x60, 0x0A };  // Invalid (Single Read for Temperature)

// byte sendBuffer[] = { 0x01, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC4, 0x0B }; // Valid (Read for Humidity and Temperature)
// byte sendBuffer[] = { 0x01, 0x04, 0x00, 0x00, 0x00, 0x02, 0x71, 0xCB }; // Invalid (Read for Humidity and Temperature)
// byte sendBuffer[] = { 0x02, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC4, 0x38 }; // Invalid (Read for Humidity and Temperature)

float humidity, temperature;
void setup() {
  Serial.begin(115200);
  Soft_Serial.begin(9600);

  pinMode(DE_RE_PIN, OUTPUT);  //DE/RE Controling pin of RS-485
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  Serial.println("\n\nWelcome to JP Learning\n");
}

void loop() {
  read_Modbus();

  if (no_Byte >= 7) {
    String incomingBytesInString[no_Byte];
    // Serial.println("\n\nno_Byte = " + String(no_Byte));
    for (int i = 0; i < no_Byte; i++) {
      // Serial.print("incomingByte[" + String(i) + "] = ");
      // Serial.println(String(incomingByte[i]) + " " + String(incomingByte[i], HEX));

      if (String(incomingByte[i]).length() == 1)
        incomingBytesInString[i] = "0" + String(incomingByte[i], HEX);
      else
        incomingBytesInString[i] = String(incomingByte[i], HEX);
    }

    if (type == 1) {
      String dataTemp = incomingBytesInString[3] + incomingBytesInString[4];
      Serial.println("\ndataTemp = " + dataTemp);
      int value = hexToDec(dataTemp);
      Serial.println("value = " + String(value));
      humidity = float(value) / 10;

      Serial.println("\nHumidity = " + String(humidity, 2) + " %\n\n");
    } else if (type == 2) {
      String dataTemp = incomingBytesInString[3] + incomingBytesInString[4];
      Serial.println("\ndataTemp = " + dataTemp);
      int value = hexToDec(dataTemp);
      Serial.println("value = " + String(value));
      temperature = float(value) / 10;

      Serial.println("\nTemperature = " + String(temperature, 2) + " °C\n\n");
    } else if (type == 3) {
      String dataTemp = incomingBytesInString[3] + incomingBytesInString[4];
      Serial.println("\ndataTemp = " + dataTemp);
      int value = hexToDec(dataTemp);
      Serial.println("value = " + String(value));
      humidity = float(value) / 10;

      dataTemp = incomingBytesInString[5] + incomingBytesInString[6];
      Serial.println("dataTemp = " + dataTemp);
      value = hexToDec(dataTemp);
      Serial.println("value = " + String(value));
      temperature = float(value) / 10;

      Serial.println("\nHumidity = " + String(humidity, 2) + " %");
      Serial.println("Temperature = " + String(temperature, 2) + " °C\n\n");
    }

    no_Byte = 0;
  }
  delay(5000);
}

void read_Modbus() {
  // Transmition Enable
  digitalWrite(DE_RE_PIN, HIGH);  //DE/RE=HIGH Transmit Enabled

  // Serial.println("\n\nsizeof(sendBuffer): " + String(sizeof(sendBuffer)));
  // for (byte i = 0; i < sizeof(sendBuffer); i++) {
  //   Serial.println("Request: " + String(sendBuffer[i], DEC) + " " + String(sendBuffer[i], HEX));
  // }
  // Serial.println();
  Soft_Serial.write(sendBuffer, sizeof(sendBuffer));

  // Receiving Enable
  digitalWrite(DE_RE_PIN, LOW);  //DE/RE=LOW Receive Enabled

  while (Soft_Serial.available() > 0) {
    //    Serial.println(Soft_Serial.read(), HEX);
    byte temp = Soft_Serial.read();
    Serial.println("Response: " + String(temp, DEC) + " " + String(temp, HEX));
    incomingByte[no_Byte] = temp;
    no_Byte++;
  }
}

int hexToDec(String hexString) {
  int decValue = 0, nextInt;
  for (int i = 0; i < hexString.length(); i++) {
    nextInt = int(hexString.charAt(i));
    if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
    if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
    if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
    nextInt = constrain(nextInt, 0, 15);
    decValue = (decValue * 16) + nextInt;
  }
  return decValue;
}
