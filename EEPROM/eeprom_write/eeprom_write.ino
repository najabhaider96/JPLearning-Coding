/*
  Welcome to JP Learning
*/

/*
   EEPROM Write
   Stores values read from analog input 0 into the EEPROM.
   These values will stay in the EEPROM when the board is
   turned off and may be retrieved later by another sketch.
*/
#include <EEPROM.h>

// the current address in the EEPROM (i.e. which byte
// we're going to write to next)
int addr = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("\n\nWelcome to JP Learning\n");
  EEPROM.begin(512);
}

void loop() {
  // need to divide by 4 because analog inputs range from
  // 0 to 1023 and each byte of the EEPROM can only hold a
  // value from 0 to 255.
  int val = analogRead(A0) / 4;

  Serial.println("addr: " + String(addr) + ", val: " + String(val));

  // write the value to the appropriate byte of the EEPROM.
  // these values will remain there when the board is
  // turned off.
  EEPROM.write(addr, val);

  // advance to the next address.  there are 512 bytes in
  // the EEPROM, so go back to 0 when we hit 512.
  // save all changes to the flash.
  addr = addr + 1;
  if (addr == 512) {
    addr = 0;
    if (EEPROM.commit()) {
      Serial.println("\nEEPROM successfully committed\n");
    } else {
      Serial.println("\nERROR! EEPROM commit failed\n");
    }
    delay(10000);
  }

  delay(100);
}
