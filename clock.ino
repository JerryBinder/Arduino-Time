#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
 
const int buttonPin = 7;
 
int buttonState = 0;
int elapsed = 0;
int binary = 0;
int incomingTime = 0;
 
unsigned int toBinary(unsigned int k) {
    return (k == 0 || k == 1 ? k : ((k % 2) + 10 * toBinary(k / 2)));
}
 
void setup() {
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  lcd.begin(16, 2);
  Serial.begin(9600);
}
 
void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);
  
//  buttonState = digitalRead(buttonPin);
// 
//  binary = toBinary(elapsed);
// 
//  if (buttonState == LOW) {
//    lcd.print(binary);
//  }
//  else {
//    lcd.print(elapsed);
//  }
// 
//  elapsed++;

  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingTime = Serial.parseInt();
    // say what you got:
    lcd.print("I received: ");
    lcd.print(incomingTime, DEC);
  }
  else {
    lcd.print("...");
  }
  delay(1000);
}
