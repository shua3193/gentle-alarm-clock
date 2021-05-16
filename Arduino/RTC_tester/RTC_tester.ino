

#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
char monthes[12][12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  rtc.disable32K();
  
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  // put your main code here, to run repeatedly:
    DateTime now = rtc.now();

    Serial.print(monthes[now.month()-1]);
    Serial.print(' ');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.year(), DEC);
    Serial.print("  ");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print("  ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    if (now.minute()<10){
      Serial.print('0');
    }
    Serial.print(now.minute(), DEC);
//    Serial.print(':');
//    if (now.second()<10){
//      Serial.print('0');
//    }
//    Serial.print(now.second(), DEC);
    Serial.println();
    Serial.println();
    delay(500);
}

//-----------Functions-----------------
//void printMonth(int month)
//{
//  switch(month)
//  {
//    case 1: lcd.print(" January ");break;
//    case 2: lcd.print(" February ");break;
//    case 3: lcd.print(" March ");break;
//    case 4: lcd.print(" April ");break;
//    case 5: lcd.print(" May ");break;
//    case 6: lcd.print(" June ");break;
//    case 7: lcd.print(" July ");break;
//    case 8: lcd.print(" August ");break;
//    case 9: lcd.print(" September ");break;
//    case 10: lcd.print(" October ");break;
//    case 11: lcd.print(" November ");break;
//    case 12: lcd.print(" December ");break;
//    default: lcd.print(" Error ");break;
//  } 
//}


// This line sets the RTC with an explicit date & time, for example to set
// January 21, 2014 at 3am you would call:
// rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
