#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//set the LCD address to 0x27 for a 20 chars and 4 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup(){ 
  lcd.begin(5, 4);// initialize the lcd with SDA 5 and SCL 4 pins
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0, 0);
  // 20 char max!
  lcd.print("Hello world line 0 !");
  lcd.setCursor(0, 1);
  lcd.print("Hello world line 1 !");
  lcd.setCursor(0, 2);
  lcd.print("Hello world line 2 !");
  lcd.setCursor(0, 3);
  lcd.print("Hello world line 3 !");
}
void loop(){
}
