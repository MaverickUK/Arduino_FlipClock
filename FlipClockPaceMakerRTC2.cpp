#include <LiquidCrystal_I2C.h>
#include "LowPower.h" // https://diyi0t.com/arduino-reduce-power-consumption/

// https://create.arduino.cc/projecthub/electropeak/interfacing-ds1307-rtc-module-with-arduino-make-a-reminder-08cb61
#include <Wire.h>
#include "RTClib.h"

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

// Pins
int RELAY_PIN = 8;
int HOURS_PIN = 10;
int MINUTES_PIN = 11;
int GO_PIN = 12;

// Settings
int RELAY_TRIGGER_LENGTH_SECONDS = 4;
bool LIGHT_ACTIVE_WHEN_RUNNING = true;
bool LOW_POWER_MODE = true;

// Run time variables
bool active = false;

RTC_DS1307 rtc;

void setup()
{
  lcd.init();                      // initialize the lcd
  // Set display type as 16 char, 2 rows
  lcd.begin(16, 2);
  lcd.backlight();

  lcdWrite("Uno Pacemaker", 0, 0);
  lcdWrite("Peter Bridger", 1, 0);
  delay(2000);
  lcd.clear();

  while (!Serial); // for Leonardo/Micro/Zero

  // Wait for RTC to become active
  Serial.begin(57600);
  if (! rtc.begin()) {
    lcdWrite("Couldn't find RTC", 0, 0);
    while (1);
  }

  if (!rtc.isrunning()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set to compile datetme
  }

  // I/O setup
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  pinMode(HOURS_PIN, INPUT_PULLUP);
  pinMode(MINUTES_PIN, INPUT_PULLUP );
  pinMode(GO_PIN, INPUT_PULLUP );

  // Disable onboard LED
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  // Low mode mode
  if ( LOW_POWER_MODE ) {
    CLKPR = 0x80; // (1000 0000) enable change in clock frequency
    CLKPR = 0x01; // (0000 0001) use clock division factor 2 to reduce the frequency from 16 MHz to 8 MHz
  }
}

void lcdWrite(String message, char row, char column) {
  lcd.setCursor(column, row);
  lcd.print(message);
}

void loop()
{
  if (active) {
    checkTime();
    showTime();
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  } else {
    setupTime();
    delay(250);
  }
}

void setupTime() {
  lcdWrite("Setup", 1, 0);
  DateTime now = rtc.now();

  int hours = now.hour();
  int minutes = now.minute();

  if ( digitalRead(HOURS_PIN) == LOW ) {
    hours++;
    if ( hours >= 24) {
      hours = 0;
    }
  }

  if ( digitalRead(MINUTES_PIN) == LOW ) {
    minutes++;
    if ( minutes >= 60 ) {
      minutes = 0;
    }
  }

  if ( digitalRead(GO_PIN) == LOW) {
    active = true;

    lcd.clear();
    lcdWrite("Running", 1, 0);

    if ( !LIGHT_ACTIVE_WHEN_RUNNING ) {
      lcd.backlight();
    }
  }

  rtc.adjust(DateTime(now.year(), now.month(), now.day(), hours, minutes, now.second()));

  showTime();
}

int lastTriggerHour = -1;

void checkTime() {
  DateTime now = rtc.now();

  bool canTrigger = lastTriggerHour != now.hour(); // Hasn't already triggered this hour
  bool triggerHour = ( now.hour() == 12 || now.hour() == 0 ); // Midday or midnight
  bool triggerMinute = now.minute() == 0;

  if ( triggerHour && triggerMinute && canTrigger) {
    lastTriggerHour = now.hour();
    triggerFlip();
  }
}

void showTime() {
  char timer [16];
  DateTime now = rtc.now();

  sprintf (timer, "%02u:%02u:%02u", now.hour(), now.minute(), now.second());

  lcdWrite(timer, 0, 0);
}

void triggerFlip()
{
  digitalWrite(RELAY_PIN, HIGH);
  delay(RELAY_TRIGGER_LENGTH_SECONDS * 1000);
  digitalWrite(RELAY_PIN, LOW);
}
