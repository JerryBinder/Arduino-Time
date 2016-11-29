#include <Time.h>
#include <TimeLib.h>
#include <LiquidCrystal.h>

#define TIME_MSG_LEN  11   // time sync to PC is HEADER and unix time_t as ten ascii digits
#define TIME_HEADER  255   // Header tag for serial time sync message

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int buttonBluePin = 7;
const int buttonOrangePin = 1;
const int buttonRedPin = 8;
const int redPin = 10;
const int yellowPin = 9;
const int whitePin = 6;
const long interval = 1000;

const bool debug = true;  // displays button high/low states on second line

  // Buttons are named after the color of the wire connecting them to digital ports
bool buttonOrangeState;
bool buttonBlueState;
bool buttonRedState;
int clockMode;    // used to switch between clock modes
int elapsed;  // current time saved in stopwatch
unsigned long currentMillis;
unsigned long previousMillis;
bool running;
int selectedTime; // determines whether hour (0), minute (1), or second (2) will be changed
 
void setup() {
  // initialize the pushbutton pin as an input:
  pinMode(buttonBluePin, INPUT);
  pinMode(buttonOrangePin, INPUT);
  pinMode(buttonRedPin, INPUT);
  lcd.begin(16, 2);
  Serial.begin(9600);

  buttonOrangeState = 0;
  buttonBlueState = 0;
  buttonRedState = 0;
  clockMode = 0;
  elapsed = 0;
  currentMillis = 0;
  previousMillis = 0;
  running = 0;
  selectedTime = 0;

  setTime(12,00,00,12,11,2012);   //can be set by clockMode 5

  //final setTime and getting time from USB should probably be here in setup
}
 
void loop() {
  // Clear LCD for new data
  lcd.setCursor(0,1);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.clear();

  setLedColors();
  
  // Get current state of the buttons
  buttonOrangeState = digitalRead(buttonOrangePin);
  buttonBlueState = digitalRead(buttonBluePin);
  buttonRedState = digitalRead(buttonRedPin);

  if(buttonBlueState == LOW)
    clockMode++;
  if(clockMode > 5)
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
      lcd.setCursor(0,1);
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
      break;
    }
    case 5: // clock set
    {
      clockSet();
      break;
    }
  }

    // Increment stopwatch even if it's not being displayed, as long as running=true
  if(running && (millis() - previousMillis >= interval))
  {
    previousMillis = millis();
    elapsed++;
  }

  if(debug && clockMode < 2)
  {
    lcd.setCursor(0, 1);
    lcd.print(buttonBlueState);
    lcd.print("  ");
    lcd.print(buttonOrangeState);
    lcd.print("  ");
    lcd.print(buttonRedState);
    lcd.print("  ");
    lcd.print(running);
  }

  if(running && (millis() - previousMillis >= interval))
  {
    previousMillis = millis();
    elapsed++;
  }
  
  if(clockMode == 5)
    delay(250);
  else
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
  if(running && buttonOrangeState == LOW) {
    running = false;
  } 
  else if(!running && buttonOrangeState == LOW) {
    running = true;
  }

  if(elapsed == 0)
    lcd.print("0");
  else
    lcd.print(elapsed, DEC);

  if(elapsed == 0 && !running)
  {
    lcd.setCursor(0,1);
    lcd.print("Press to start.");
  }

  if(elapsed > 0 && running)
  {
    lcd.setCursor(0,1);
    lcd.print("Running");
  }

  if(elapsed > 0 && !running)
  {
    if(second() % 2 == 0)
    {
      lcd.setCursor(0,1);
      lcd.print("Paused");
    }
  }
}

void clockSet()
{
  adjustTime(-.75);

  if(buttonOrangeState == LOW)
    selectedTime++;
  if(selectedTime > 2)
    selectedTime = 0;
  
  switch(selectedTime)
  {
    case 0: // set hour
    {
      if(buttonRedState == LOW){
        setTime((hour()+1),minute(),second(),1,1,1970);
      }
      lcd.print(hour());
      lcd.print(':');
      lcd.print(minute());
      lcd.print(':');
      lcd.print(second());
      lcd.setCursor(0,1);
      lcd.print("^hour");
      break;
    }
    case 1: // set minute
    {
      lcd.print(hour());
      lcd.print(':');
      if(buttonRedState == LOW){
        setTime(hour(),(minute()+1),second(),1,1,1970);
      }
      lcd.print(minute());
      lcd.print(':');
      lcd.print(second());
      lcd.setCursor(0,1);
      lcd.print("   ^minute");
      break;
    }
    case 2: // set second
    {
      lcd.print(hour());
      lcd.print(':');
      lcd.print(minute());
      lcd.print(':');
      if(buttonRedState == LOW){
        setTime(hour(),minute(),(second()+1),1,1,1970);
      }
      lcd.print(second());
      lcd.setCursor(0,1);
      lcd.print("      ^second");
      break;
    }
  }
}
