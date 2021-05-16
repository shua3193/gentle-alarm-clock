

#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

// ---------- alarm vars ------------
int Alarm1_hour = 6;
int Alarm1_min = 30;
DateTime Alarm1;
int trigger_alarm = 0;
// ---------------- Pins ---------------
const int MP3_Tx = 11;
const int MP3_Rx = 10; 
// not using pin   2
const int minPin = 3;
const int hourPin = 4;
const int timePin = 5;
const int alarmPin = 6;
const int lightSnoozePin = 7;

// ---------- button vars ------------
int minLast = 0;
int hourLast = 0;
int timeLast = 0;
int alarmLast = 0;
int lightLast = 0;
int newIn;

// ---------- time vars ------------
int Time_hour;
int Time_min;
DateTime newTime;

// ---------- runtime vars ------------
const int totalGentleTime = 30; // 30 mins for total wake time
const int music_runtime = totalGentleTime/10*60*1000; //ms -> 30/10=3 mins
const int backlight_runtime = 3000; //ms = 3 seconds
int music_startTime;
int backlight_startTime;

// ---------- music vars ------------
int audioLevel = 3;
const int vol_increment = 3;
const int MaxVolume = 30;
int iteration = 0;

const int birds = 1;
const int rooster = 2;

// ---------- extra vars ------------
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
int BacklightStatus = 0;


// --------------------- SETUP ------------------------
void setup(){
  // --------- setting up the pins -----------
  pinMode(minPin, INPUT);
  pinMode(hourPin, INPUT);
  pinMode(timePin, INPUT);
  pinMode(alarmPin, INPUT);
  pinMode(lightSnoozePin, INPUT);
  
  // will probably have to increase baud rate
  // Serial.begin(9600);
  
  // --------- lcd setup -----------
  lcd.init();
  lcd.backlight();
  lcd.print("Welcome");
  delay(3000);
  lcd.clear();
  lcd.print(millis());
  delay(1000);
  lcd.clear();
  lcd.println("setup complete");
  delay(1000);
  lcd.clear();
  

  // --------- rtc setup -----------
  rtc.disable32K();
  
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
  if (! rtc.begin()) {
    lcd.print("Couldn't find RTC");
    delay(10*1000);
    abort();
  }

  if (rtc.lostPower()) {
    lcd.print("RTC lost power, let's set the time!");
    delay(1*1000);
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }



  printDate(rtc.now());
  
  lcd.noBacklight();


  // --------- MP3 setup -----------
  mySoftwareSerial.begin(9600);
  // Serial.begin(115200);

  myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms
  myDFPlayer.volume(audioLevel);  //Set volume value (0~30).
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);

  
}

// --------------------- MAIN LOOP ------------------------
void loop(){
  DateTime now = rtc.now();
  printTime(now);
  // if(BacklightStatus){
  //   lcd.noBacklight();
  //   BacklightStatus = 0;
  // }

  if(now.minute()==0){ // prints once an hour
    printDate(now);
  }
//  Serial.print(now.hour());
//  Serial.print(":");
//  Serial.print(now.minute());
//  Serial.print(":");
//  Serial.print(now.second());
//  Serial.println();
//  Serial.println();
  
  
  //----------- Alarm changing ----------------
  while(digitalRead(alarmPin)){
    newIn = digitalRead(minPin);
    if (buttonCheck(minLast, newIn)){
      Alarm1_min += 1;
      if (Alarm1_min >= 60){
        Alarm1_min -= 60;
      }
    }
    minLast = newIn;
    newIn = digitalRead(hourPin);
    if (buttonCheck(hourLast, newIn)){
      Alarm1_hour += 1;
      if (Alarm1_hour >= 24){
        Alarm1_hour -= 24;
      }
    }
    hourLast = newIn;
//    DateTime(year, month, day, hour, minute, second)
    Alarm1 = DateTime(2000, 5, 15, Alarm1_hour, Alarm1_min, 0);
    printTime(Alarm1);
  }
  
  //----------- Time changing ----------------
  Time_hour = now.hour();
  Time_min = now.minute();
  
  while(digitalRead(timePin)){
    timeLast = 1;
    newIn = digitalRead(minPin);
    if (buttonCheck(minLast, newIn)){
      Time_min ++;
      if (Time_min >= 60){
        Time_min -= 60;
      }
    }
    minLast = newIn;
    newIn = digitalRead(hourPin);
    if (buttonCheck(hourLast, newIn)){
      Time_hour ++;
      if (Time_hour >= 24){
        Time_hour -= 24;
      }
    }
    hourLast = newIn;
    newTime = DateTime(now.year(), now.month(), now.day(), Time_hour, Time_min, 0);
    printTime(newTime);
  }
  if(releaseCheck(timeLast,digitalRead(timePin))){
    timeLast = 0;
    rtc.adjust(newTime);
  }

  //----------------- alarm comparing/triggering -------------------
  alarm_time_comp(now, Alarm1);
  // ----- triggering alarm -----------
  if(trigger_alarm){
    if(!music_startTime){
      music_startTime = millis();
      iteration = 1;
    }else{
      // check one increment
      if(iteration <= 10){
        if(millis()-music_startTime >= music_runtime){
          // increase the volume
          audioLevel += vol_increment;
          iteration ++;
          if(audioLevel > MaxVolume){
            audioLevel = MaxVolume; // cant have higher audioLevel than max
          }
          myDFPlayer.volume(audioLevel);
          myDFPlayer.play(birds);  //Play the first mp3
        }
      }else{ //Max volume level and put rooster music
        audioLevel = MaxVolume;
        myDFPlayer.volume(audioLevel);
        myDFPlayer.play(rooster);  //Play the first mp3
      }
    }
  }

  //-------------- snoozing and turning on backlight -------------
  if(digitalRead(lightSnoozePin)){
    if (trigger_alarm){
      trigger_alarm = 0;
      myDFPlayer.sleep();     //sleep, turning off the alarm
      audioLevel = 3;
      iteration = 0;
    }
    lcd.backlight();
    BacklightStatus = 1;
    backlight_startTime = millis();
  }

  //-------- turning backlight off -----------------
  if(BacklightStatus){
    if(millis()-backlight_startTime >= backlight_runtime){
      lcd.noBacklight();
      BacklightStatus = 0;
    }
  }

}


//-----------Functions-----------------

void printTime(DateTime t){
  lcd.setCursor(4,1); // 4 indents for spacing, second line
  if(t.hour() < 10){
    lcd.print("0");
  }
  lcd.print(t.hour());
  lcd.print(":");
  if(t.minute() < 10){
    lcd.print("0");
  }
  lcd.print(t.minute());
  lcd.print(":");
  if(t.second() < 10){
    lcd.print("0");
  }
  lcd.print(t.second());
}

void printDate(DateTime t){
  lcd.setCursor(2,0); // 2 indents for spacing, first line
  lcd.print(t.day());
  printMonth(t.month());
  lcd.print(t.year());
}

void printMonth(int month){
  switch(month)
  {
    case 1: lcd.print(" Jan ");break;
    case 2: lcd.print(" Feb ");break;
    case 3: lcd.print(" Mar ");break;
    case 4: lcd.print(" Apr ");break;
    case 5: lcd.print(" May ");break;
    case 6: lcd.print(" Jun ");break;
    case 7: lcd.print(" Jul ");break;
    case 8: lcd.print(" Aug ");break;
    case 9: lcd.print(" Sep ");break;
    case 10: lcd.print(" Oct ");break;
    case 11: lcd.print(" Nov ");break;
    case 12: lcd.print(" Dec ");break;
    default: lcd.print(" Err ");break;
  } 
}

bool buttonCheck(int last,int newIn){
  if (last == 0 && newIn == 1){
        return 1;
  }else{
    return 0;
  }
}

bool releaseCheck(int last,int newIn){
  if (last == 1 & & newIn == 0){
        return 1;
  }else{
    return 0;
  }
}

bool alarm_time_comp(DateTime now, DateTime alarm){
  if (!trigger_alarm){
    if (now.hour()==alarm.hour()){
      if (now.minute()==alarm.minute()){
        trigger_alarm = 1;
      }
    }
  }
}
