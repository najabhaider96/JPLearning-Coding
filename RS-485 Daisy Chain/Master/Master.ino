/*
  Welcome to JP Learning
*/
#include <SoftwareSerial.h>
#include <ModbusMaster.h>

// GPIO Pins
byte TX_PIN = 4, RX_PIN = 5;
byte DE_RE_PIN = 16;

SoftwareSerial Soft_Serial(RX_PIN, TX_PIN);

ModbusMaster node;

// Variables
#define Reg_Address 0x0000
byte SlaveIDs[] = {0x01, 0x02, 0x03};
long Values[3];

void preTransmission() {
  digitalWrite(DE_RE_PIN, HIGH);
}
void postTransmission() {
  digitalWrite(DE_RE_PIN, LOW);
}

void setup() {
  Serial.begin(115200);
  Soft_Serial.begin(9600);

  pinMode(DE_RE_PIN, OUTPUT);  //DE/RE Controling pin of RS-485

  //  node.preTransmission(preTransmission);
  //  node.postTransmission(postTransmission);

  Serial.println("\n\nWelcome to JP Learning\n");
  Serial.println("Master Start\n");
}

void loop() {
  for (byte i = 0; i < 3; i++) {
    Values[i] = Read_Data(SlaveIDs[i], Reg_Address);
    Serial.println("\nslaveID: 0x" + String(SlaveIDs[i], HEX) + " and Value: 0x" + String(Values[i], HEX));
    delay(100);
  }
  delay(5000);
}
long Read_Data(byte SlaveID, int Reg_Addr)
{
  node.begin(SlaveID, Soft_Serial);
  uint8_t result, j;
  long value = 0;
  byte dataReadLength = 1;
  uint16_t data[dataReadLength];

  //  // Disable watchdog reset only for ESP8266
  //  ESP.wdtDisable();

  result = node.readHoldingRegisters(Reg_Addr, dataReadLength);
  // result = node.readInputRegisters(Reg_Addr, dataReadLength);
  // Serial.println("result: " + String(result, HEX) + ", " + String(result));

  //  // Enable watchdog reset only for ESP8266
  //  ESP.wdtEnable(1);

  if (result == node.ku8MBSuccess) {
    for (j = 0; j < dataReadLength; j++)
      data[j] = (node.getResponseBuffer(j));

    // Serial.println("data[0]: " + String(data[0], HEX) + " " + String(data[0]));

    String dataTemp = String(data[0], HEX);
     Serial.println("\n\ndataTemp = " + dataTemp);
    long value = hexToDec(dataTemp);
    Serial.println("value = " + String(value));
    return value;
  } else {
    Serial.println("Connect modbus fail. SlaveID: 0x" + String(SlaveID, HEX) + " and Register: 0x" + String(Reg_Addr, HEX));
    delay(100);
    return 0;
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
