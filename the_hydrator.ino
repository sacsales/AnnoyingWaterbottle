#include <LiquidCrystal_I2C.h>
#include "pitches.h"


// sound
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};


LiquidCrystal_I2C lcd(0x27, 16, 2);


//timer setup
int min = 30;
int sec = 0;
unsigned long oneSecond = 1000UL;
unsigned long startTime;
unsigned long previous2 = 0;


//pins 
const int buttonPin = 2;
const int ledPin = 4;
int bottleIn = 0;
int ledState = LOW;


void setup() {

  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  Serial.print("starting up...");
  sec = sec + 60 * min;
  pinMode(ledPin, OUTPUT);


  lcd.begin();
  lcd.backlight();


}

void loop() {
  
  bottleIn = digitalRead(buttonPin);

  //countdown timer 
  {
    if (millis() - startTime >= oneSecond)
    {
      sec--;
      startTime += oneSecond;
      if (sec < 0) timerReset();
      lcd.clear();
      int displayMin = sec / 60;
      if (displayMin < 10) lcd.print("0");
      lcd.print(displayMin);
      lcd.print(":");
      int displaySec = sec % 60;
      if (displaySec < 10) lcd.print("0");
      lcd.print(sec % 60);

    }
  }


  //led controls
  if (bottleIn == HIGH) {
    digitalWrite(ledPin, HIGH);
    //the bottle is still in here

  } else if (bottleIn == LOW) {
    //the bottle has been removed
    digitalWrite(ledPin, LOW);

  }
}



void timerReset() {

  //while bottle is still inside...
  while (digitalRead(buttonPin) == HIGH) {
    lcd.clear();
    lcd.print("drink!");
    playSound();

  }

  lcd.clear();
  lcd.print("great job! :)");
  delay(1000);
  lcd.clear();

  sec = 0;
  min = 30;
  sec = sec + 60 * min;
  unsigned long oneSecond = 1000UL;

}

//sound and lights
void playSound() {

  for (int thisNote = 0; thisNote < 8; thisNote++) {

    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    digitalWrite(ledPin, ledState);

    
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(8, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing
    noTone(8);
  }
}
