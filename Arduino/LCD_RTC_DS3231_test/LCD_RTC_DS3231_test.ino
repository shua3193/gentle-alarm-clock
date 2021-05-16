// LiquidCrystal I2C - Version: Latest 
/*RTC_LCD_i2c
Original code obtained from:
https://gist.github.com/laurenceHR/c08cd2a0944df78d168c
*/

#include <LiquidCrystal.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2); //0x27 is the LCD Address

int backlightPin = 3;
void setup(){
  Wire.begin();
  Serial.begin(9600);  
  lcd.backlight();  
  lcd.init();
  pinMode(backlightPin,OUTPUT);
  analogWrite(backlightPin,50); // For External Backlight Control 
}

void loop(){
  lcd.clear();
  
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  
  lcd.print("VideotronicMaker");
  lcd.setCursor(0,1);
  if(hour<10) lcd.print("0");
  lcd.print(hour); 
  lcd.print(":"); 
  if(minute<10) lcd.print("0");
  lcd.print(minute); 
  lcd.print(":"); 
  if(second<10) lcd.print("0");
  lcd.print(second); 
  lcd.print(" ");
  if(dayOfMonth<10) lcd.print("0");
  lcd.print(dayOfMonth);
  lcd.print("/");
  if(month<10) lcd.print("0");
  lcd.print(month);  
    
  delay(1000);  
}
