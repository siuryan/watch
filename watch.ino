#define VERSION 1.0

/* TODO:
 * - Pong
 * - Random number generator
 */

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Time.h>
#include <RTClib.h>

#define OLED_RESET 2 
Adafruit_SSD1306 display(OLED_RESET);

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 

#define pi 3.1416

byte buzzerPin = 6;
#define button1 7
#define button2 8
#define pot A0

byte mode = 8;
#define numModes 7 // look at change

byte setting = 0;
#define numSettings 2
bool powerSave = 0;

byte timeSetting = 0;
byte setHour = 0;
byte setMinute = 0;
byte setMonth = 0;
byte setDay = 0;

#define flashlight 9

byte alarmSetting = 0;
long alarm1 = 0;
byte alarm1Hrs = 0;
byte alarm1Mins = 0;
byte alarm1Secs = 0;

long stopwatch1 = 0;
bool stop1Paused = 0;
long pause1Time = 0;

bool gameStarted = 0;
byte paddleX = 4;
byte paddleY = 24;
byte paddleLen = 16;
byte ballX = 64;
byte ballY = 32;
byte ballVel = 0;
byte ballAngle = 0;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3D);  // initialize with the I2C addr 0x3D (for the 128x64)
  
  display.display();
  pinMode(buzzerPin, OUTPUT);
  analogWrite(buzzerPin, 255);
  delay(300);
  analogWrite(buzzerPin, 0);
  delay(2000);

  pinMode(flashlight, OUTPUT);
  pinMode(pot, INPUT);

  setTime(15,00,00,27,5,2016);
  setHour = hour(now());
  setMinute = minute(now());
  setMonth = month(now());
  setDay = day(now());

  randomSeed(analogRead(2));

  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void loop() {
  display.clearDisplay();
  display.setTextColor(WHITE);
  
  switch (mode) {
    case 0:
      homeScreen();
      break;
    case 1:
      schedule();
      break;
    case 2:
      setAlarm();
      break;
    case 3:
      stopwatch();
      break;
    case 4:
      if (!gameStarted) {
        pongIntro();
        gameStarted = digitalRead(button2) == HIGH;
      } else {
        pong();
      }
      break;
    case 5:
      settings();
      break;
    case 6:
      changeTime();
      break;
    case 7:
      about();
      break;
  }

  if (now() == alarm1) {
    alarmTriggered(F("Alarm 1"), alarm1);
  }

  mode = map(analogRead(pot), 0, 1023, 0, numModes);
  
  delay(100);
}

void printTime(time_t t) {
  display.println(zeroFix(hour(t)) + F(":") + zeroFix(minute(t)) + F(":") + zeroFix(second(t)));
}

void homeScreen() {

  time_t t = now();

  // text display tests
  String w = dayStr(weekday(t));
  String M = monthStr(month(t));
  String d = String(day(t));
  byte h = hour(t);
  byte m = minute(t);
  byte s = second(t);

  display.setCursor(9*display.width()/16, display.height()/2 - 10);
  display.println(w);
  display.setCursor(9*display.width()/16, display.height()/2);
  display.println(M + F(" ") + d);
  display.setCursor(9*display.width()/16, display.height()/2 + 10);
  printTime(t);
  
  drawClock(hour(t), minute(t));
  
  display.display();
}

void drawClock(byte h, byte m) {
  byte minuteHand = 20;
  byte hourHand = 15;
  
  byte centerX = display.width() / 4;
  byte centerY = display.height() / 2;
  byte minuteX = sin(6*m*pi/180) * minuteHand + centerX;
  byte minuteY = -cos(6*m*pi/180) * minuteHand + centerY;
  byte hourX = sin((30*h+0.5*m)*pi/180) * hourHand + centerX;
  byte hourY = -cos(30*h*pi/180) * hourHand + centerY;
  
  display.drawCircle(centerX, centerY, 25, WHITE);
  display.fillCircle(centerX, centerY, 1, WHITE);
  display.drawLine(centerX, centerY, minuteX, minuteY, WHITE);
  display.drawLine(centerX, centerY, hourX, hourY, WHITE);
}

String zeroFix(int num) {
  if (num < 10) { return "0" + String(num); }
  return String(num);
}

void schedule() {

  // Regular
  const float schedule0[] = {
  8.00, 8.41,
  8.45, 9.26,
  9.31, 10.14,
  10.20, 11.01,
  11.06, 11.47,
  11.52, 12.33,
  12.38, 13.19,
  13.24, 14.05,
  14.09, 14.50,
  14.54, 15.35 };

  // Homeroom
  /*const float schedule0[] = {
  8.00, 8.40,
  8.45, 9.25,
  9.29, 10.09,
  10.30, 11.10,
  11.14, 11.54,
  11.58, 12.38,
  12.42, 13.22,
  13.26, 14.06,
  14.10, 14.50,
  14.55, 15.35,
  10.13, 10.25 };*/

  const byte schedule0Len = sizeof(schedule0)/4;

  time_t t = now();

  float adjCurrentTime = hour(t) + minute(t) / 100.0;

  byte i = 0;
  bool found = 0;
  float period = 0;
  while (!found && i < schedule0Len) {
    float timeStart = schedule0[i];
    float timeFinish = schedule0[i+1];
    float timeNext = 0;
    if (i < schedule0Len - 2) {
      timeNext = schedule0[i+2];
    }
    if (timeStart <= adjCurrentTime && adjCurrentTime <= timeFinish) {
      period = i/2 + 1.0;
      found = 1;
    } else if (timeFinish <= adjCurrentTime && adjCurrentTime <= timeNext) {
      period = i/2 + 1.5;
      found = 1;
    } else {
      i += 2;
    }
  }

  if (weekday(t) == 1 or weekday(t) == 7) {
    period == 0;
  }

  display.setCursor(0,0);
  display.println(F("Schedule"));
  display.println();
  printTime(t);

  if (period == 0) {
    display.print(F("Period "));
    display.println(String(round(period)));
    display.println(String(schedule0[schedule0Len - 1]) + "-" + schedule0[0]);
    display.println(F("School's out!"));
  } else if (int(period*10) % 10 == 0) {
    display.print(F("Period "));
    display.println(String(round(period)));
    display.println(String(schedule0[i]) + F("-") + schedule0[i+1]);
    display.println(String(minDiff(schedule0[i+1], adjCurrentTime)) + F(" minutes left"));
  } else {
    display.println(String(schedule0[i+1]) + F("-") + schedule0[i+2]);
    display.println(String(minDiff(schedule0[i+2], adjCurrentTime)) + F(" minutes until startof Period ") + String(round(period)));
  }

  display.display();
}

byte minDiff(float final, float current) {
  byte hourDiff = floor(final) - floor(current);
  if (hourDiff >= 1) {
    return (60 - (int(current*100) % int(floor(current)*100)) + (int(final*100) % int(floor(final)*100)) + (60 * (hourDiff - 1)));
  }
  return (int(final*100) % int(floor(final)*100)) - (int(current*100) % int(floor(current)*100));
}

void settings() {
  
  display.setCursor(0,0);
  display.println(F("Settings"));
  display.println();

  display.println(F("Power Saving Mode"));
  display.println(F("Flashlight"));
  display.setCursor(120, 15 + 8*setting);
  display.print(F("<"));
  
  display.dim(powerSave);
  display.display();

  if (digitalRead(button1) == HIGH) {
    setting = (setting + 1) % numSettings;
  }

  if (digitalRead(button2) == HIGH) {
    switch (setting) {
      case 0:
        powerSave = !powerSave;
        break;
      case 1:
        digitalWrite(flashlight, !digitalRead(flashlight));
        break;
    }
  }
}

void changeTime() {
  display.setCursor(0,0);
  display.println(F("Change Time"));
  display.println();

  display.println(F("Hour"));
  display.println(F("Minute"));
  display.println(F("Month"));
  display.println(F("Day"));
  display.println(F("Set time"));
  display.setCursor(100, 15 + 8*timeSetting);

  switch(timeSetting) {
    case 0:
      display.print(setHour);
      break;
    case 1:
      display.print(setMinute);
      break;
    case 2:
      display.print(setMonth);
      break;
    case 3:
      display.print(setDay);
      break;
    case 4:
      display.print(F("<"));
      break;
    default:
      break;
  }

  display.display();
  
  if (digitalRead(button1) == HIGH) {
    timeSetting = (timeSetting + 1) % 5;
  }

  if (digitalRead(button2) == HIGH) {
    switch (timeSetting) {
      case 0:
        setHour = (setHour + 1) % 24;
        break;
      case 1:
        setMinute = (setMinute + 1) % 60;
        break;
      case 2:
        setMonth = (setMonth + 1) % 12;
        break;
      case 3:
        setDay = (setDay + 1) % 31;
        break;
      case 4:
        setTime(setHour, setMinute, second(), setDay, setMonth, year());
        setHour = hour(now());
        setMinute = minute(now());
        setMonth = month(now());
        setDay = day(now());
        break;
    }
  }
}

void setAlarm() {

  display.setCursor(0,0);
  display.println(F("Alarm 1"));
  display.println();
  
  if (alarm1 == 0) {
    display.println(F("Hours"));
    display.println(F("Minutes"));
    display.println(F("Seconds"));
    display.println(F("Set alarm"));
    display.setCursor(100, 15 + 8*alarmSetting);
  
    switch(alarmSetting) {
      case 0:
        display.print(alarm1Hrs);
        break;
      case 1:
        display.print(alarm1Mins);
        break;
      case 2:
        display.print(alarm1Secs);
        break;
      case 3:
        display.print(F("<"));
        break;
      default:
        break;
    }
    
    if (digitalRead(button1) == HIGH) {
      alarmSetting = (alarmSetting + 1) % 4;
    }
  
    if (digitalRead(button2) == HIGH) {
      switch (alarmSetting) {
        case 0:
          alarm1Hrs = (alarm1Hrs + 1) % 24;
          break;
        case 1:
          alarm1Mins = (alarm1Mins + 1) % 60;
          break;
        case 2:
          alarm1Secs = (alarm1Secs + 1) % 60;
          break;
        case 3:
          if (alarm1Hrs + alarm1Mins + alarm1Secs != 0) {
            alarm1 = now() + alarm1Hrs*3600 + alarm1Mins*60 + alarm1Secs;
            alarmSetting = 0;
          }
          break;
      }
    }
  } else if (alarm1 != 0) {
    display.println();
    
    time_t t = alarm1 - now();
    printTime(t);
    display.println(F("Delete           <"));

    if (digitalRead(button2) == HIGH) {
      alarm1 = 0;
    }
  }

  display.display();
}

void alarmTriggered(String alarm, long alarmTime) {
  display.clearDisplay();
  
  display.setCursor(0,0);
  display.println(alarm);
  display.println();
  display.println(F("Alarm triggered!"));
  printTime(alarmTime);
  display.display();

  while (digitalRead(button1) != HIGH and digitalRead(button2) != HIGH) {
    if (!powerSave) {
      analogWrite(buzzerPin, 192);
    }
    delay(100);
  }

  analogWrite(buzzerPin, 0);
  alarm1 = 0;
}

void stopwatch(){
  display.setCursor(0,0);
  display.println(F("Stopwatch"));
  display.println();

  if (stopwatch1 == 0) {
    display.println(F("Start            <"));

    if (digitalRead(button2) == HIGH) {
      stopwatch1 = now();
    }
  } else {
    if (!stop1Paused) {
      time_t t = now() - stopwatch1;
      printTime(t);
      display.println(F("1: Pause"));
    } else {
      time_t t = pause1Time - stopwatch1;
      printTime(t);
      display.println(F("1: Resume"));
    }
    
    display.println(F("2: Delete"));

    if (digitalRead(button1) == HIGH) {
      stop1Paused = !stop1Paused;
      if (stop1Paused) {
        pause1Time = now();
      } else {
        stopwatch1 += now() - pause1Time;
      }
    } else if (digitalRead(button2) == HIGH) {
      stopwatch1 = 0;
      stop1Paused = 0;
      pause1Time = 0;
    }
    
  }
  
  display.display();
}

void about() {
  display.setCursor(0,0);
  display.println(F("About"));
  display.println();
  display.print(F("Watch v"));
  display.println(VERSION);
  display.println(F("Designed by Ryan Siu"));

  display.display();
}

void pongIntro() {
  display.setCursor(0,0);
  display.println(F("Pong"));
  display.println();
  display.println(F("Press select to start"));
  display.println();

  display.display();
}

void ballMove() {
  if (ballVel == 0) {
    ballAngle = random(360);
    ballVel = 5;
  }
  ballX += ballVel*cos(ballAngle*pi/180);
  ballY += ballVel*sin(ballAngle*pi/180);

  delay(500);
}

void pong() {
  display.drawLine(paddleX, paddleY, paddleX, paddleY+paddleLen, WHITE);
  display.drawCircle(ballX, ballY, 3, WHITE);

  if (digitalRead(button1) == HIGH && paddleY >= 3) { paddleY -= 3; } 
  else if (digitalRead(button2) == HIGH && paddleY <= 45) { paddleY += 3; }

  ballMove();

  display.display();
}

