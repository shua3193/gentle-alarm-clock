// Armuino --- Arduino DS3231 Real Time Clock Module - Basic Sketch ---
//
// DS3231 Library by Henning Karlsen (www.rinkydinkelectronics.com)
//
// Arduino Real Time Clock DS3231 with I2C LCD Display - Simple Sketch:
// https://www.youtube.com/watch?v=v7tWVRJXy8M

#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <DS3231.h>

#define I2C_ADDR 0x27 // Add your I2C address here.
#define Rs_pin 0
#define Rw_pin 1
#define En_pin 2
#define BACKLIGHT_PIN 3
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7

DS3231  rtc(SDA, SCL);

LiquidCrystal_I2C lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);

void setup() { 
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE); // LCD Backlight ON
  lcd.setBacklight(HIGH);
   
  lcd.home (); // Go home on LCD
  rtc.begin(); // Initialize the rtc object
  lcd.begin(16,2); // Initializes the interface to the LCD screen with the dimensions (width and height) of the display
}

void loop() { 
 lcd.setCursor(0,0);
 lcd.print("Time:  ");
 lcd.print(rtc.getTimeStr());
 
 lcd.setCursor(0,1);
 lcd.print("Date: ");
 lcd.print(rtc.getDateStr());
 
 delay(1000); 
}
