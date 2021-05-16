//DFRobot.com
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display


unsigned long t = 0;
int val = 0;
void setup(){
  lcd.init();                      // initialize the lcd 
 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.print("Welcome");
  delay(3000);
  lcd.clear();
  lcd.print(millis());
  delay(1000);
  lcd.noBacklight();
  pinMode(8, INPUT);
  Serial.begin(9600);
  Serial.println("setup complete");
}

void loop(){
  val = digitalRead(8);
  if (val == LOW){
    Serial.println("High value");
//    lcd.backlight();
    lcd.clear();
    lcd.print("Start");
    t = millis();
    lcd.print(t);
  }
  if (t){
    lcd.clear();
    lcd.print(millis());
//    delay(100);
    if ((millis()-t) >= 5000){
      t = 0;
      Serial.println("got to end");
      lcd.print("End");
      lcd.noBacklight();
    } 
  }
}
