#include <Time.h>
#include <TimeLib.h>
#include <LiquidCrystal.h>

#define TIME_MSG_LEN  11   // time sync to PC is HEADER and unix time_t as ten ascii digits
#define TIME_HEADER  255   // Header tag for serial time sync message

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int buttonBluePin = 7;
const int buttonOrangePin = 1;
const int redPin = 10;
const int yellowPin = 9;
const int whitePin = 6;
const long interval = 1000;

  // Buttons are named after the color of the wire connecting them to digital ports
bool buttonOrangeState;
bool buttonBlueState;

int incomingTime;   // stores time sent through USB
int clockMode;    // used to switch between clock modes
int elapsed;  // current time saved in stopwatch
unsigned long currentMillis;
unsigned long previousMillis;
bool running;
 
void setup() {
  // initialize the pushbutton pin as an input:
  pinMode(buttonBluePin, INPUT);
  pinMode(buttonOrangePin, INPUT);
  lcd.begin(16, 2);
  Serial.begin(9600);

  buttonOrangeState = 0;
  buttonBlueState = 0;
  incomingTime = 0;
  clockMode = 0;
  elapsed = 0;
  running = 0;
  currentMillis = 0;
  previousMillis = 0;

  setTime(12,22,32,12,11,2012);   //hardcoded for testing purposes

  //final setTime and getting time from USB should probably be here in setup
}
 
void loop() {
  // Clear LCD for new data
  lcd.clear();
  lcd.setCursor(0, 0);

  // Get current state of the two buttons
  buttonOrangeState = digitalRead(buttonOrangePin);
  buttonBlueState = digitalRead(buttonBluePin);

  if(digitalRead(buttonBluePin) == LOW)
    clockMode++;
  if(clockMode > 4)
    clockMode = 0;

  switch(clockMode)
  {
    case 0: // military
    {
      lcd.print(hour());
      lcd.print(':');
      lcd.print(minute());
      lcd.print(':');
      lcd.print(second());
      break;
    }
    case 1: // AM/PM
    {
      lcd.print(hourFormat12());
      lcd.print(':');
      lcd.print(minute());
      lcd.print(':');
      lcd.print(second());
      if(isAM())
        lcd.print(" AM");
      else
        lcd.print(" PM");
      break;
    }
    case 2: // binary
    {
      lcd.print(hour(), BIN);
      lcd.print(':');
      lcd.print(minute(), BIN);
      lcd.print(':');
      lcd.print(second(), BIN);
      break;
    }
    case 3: // hex
    {
      lcd.print(hour(), HEX);
      lcd.print(':');
      lcd.print(minute(), HEX);
      lcd.print(':');
      lcd.print(second(), HEX);
      break;
    }
    case 4: // stopwatch
    {
      stopwatch();
    }
  }

  lcd.setCursor(0, 1);
  lcd.print(digitalRead(buttonOrangePin));
  lcd.print("  ");
  lcd.print(digitalRead(buttonBluePin));
  lcd.print("  ");
  lcd.print(running);
  
  delay(1000);
}

// Sets LED colors based on current hour
void setLedColors()
{
  if(0 < hour() && hour() < 5)
  {
    analogWrite(redPin, 255); 
    analogWrite(yellowPin, 130); 
    analogWrite(whitePin, 0);
  }
  else if(20 < hour() && hour() < 24)
  {
    analogWrite(redPin, 255); 
    analogWrite(yellowPin, 130); 
    analogWrite(whitePin, 0);
  }
  else if(5 < hour() && hour() < 10)
  {
    analogWrite(redPin, 130); 
    analogWrite(yellowPin, 130); 
    analogWrite(whitePin, 130);
  }
  else if(14 < hour() && hour() < 20)
  {
    analogWrite(redPin, 130); 
    analogWrite(yellowPin, 130); 
    analogWrite(whitePin, 130);
  }
  else if(10 < hour() && hour () < 14)
  {
    analogWrite(redPin, 0); 
    analogWrite(yellowPin, 100); 
    analogWrite(whitePin, 255);
  }
}

void stopwatch()
{
  currentMillis = millis();

  if(running && digitalRead(buttonOrangePin) == LOW) {
    running = false;
  } 
  else if(!running && digitalRead(buttonOrangePin) == LOW) {
    running = true;
  }

  if(running && (currentMillis - previousMillis >= interval)) {
    previousMillis = currentMillis;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(elapsed, DEC);
    elapsed++;
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(elapsed, DEC);
  }
    
}
