/*
  Welcome to JP Learning
*/

/*
   EEPROM Clear
   Sets all of the bytes of the EEPROM to 0.
   This example code is in the public domain.
*/
#include <EEPROM.h>

void setup() {
  EEPROM.begin(512);
  // write a 0 to all 512 bytes of the EEPROM
  for (int i = 0; i < 512; i++) {
    EEPROM.write(i, 0);
  }

  // turn the LED on when we're done
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  EEPROM.commit();
  EEPROM.end();
}

void loop() {
}
