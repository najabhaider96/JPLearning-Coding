/*
Welcome to JP Learning
*/
#include <LiquidCrystal_I2C.h>

//int lcdAddr = 0x3F; // 63 in decimal
int lcdAddr = 0x27; // 39 in decimal

LiquidCrystal_I2C lcd(lcdAddr, 16, 2);
void setup() {
  Serial.begin(115200);
  Serial.println("\n\nESP Start\n");

  // initialize the lcd
  lcd.init();
  // Turn on the backlight.
  lcd.backlight();
  lcd.clear();
  // Move the cursor characters to the right and
  // zero characters down (line 1).
  lcd.setCursor(0, 0);
  lcd.print("LCD With ESP8266");

  // Move the cursor to the next line and print
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Assalam-o-Alikum");
  lcd.setCursor(2, 1);
  lcd.print("Hello World");
  Serial.println("\n\nSetup End\n");
  delay(5000);
}

void loop() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Welcome To");
  lcd.setCursor(2, 1);
  lcd.print("JP  Learning");
  delay(1000);
}
