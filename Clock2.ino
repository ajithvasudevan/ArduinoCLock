/*
 * Clock with light leak bugs fixed, and with time setting feature
 */


#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;
int colonPin = 9;
int modePin = 14;
int setPin = 15;
int MODE_BUTTON_PREVSTATE, MODE_BUTTON_STATE;
int SET_BUTTON_PREVSTATE, SET_BUTTON_STATE;
int MODE = 0;
int hr, mt;
int SET_DELAY = 50;
int setDelayCtr = 0;
int timeChanged = 0;
int baseDelay = 1000;
int bias = 300;

/*
* Arduino 4x7 LED Display
* Displays numbers ranging from 0 through 1023
* Test Code displays the value of a sensor connected to the analog input A0
* Inspired by a code found in the Arduino Cookbook
* Tested at TechNode Protolabz/July 2014
*/
// bits representing segments A through G (and decimal point) for numerals 0-9
const int numeral[10] = {
   //ABCDEFG /dp
   B11111100, // 0
   B01100000, // 1
   B11011010, // 2
   B11110010, // 3
   B01100110, // 4
   B10110110, // 5
   B10111110, // 6
   B11100000, // 7
   B11111110, // 8
   B11110110, // 9
};
// pins for decimal point and each segment
// DP,G,F,E,D,C,B,A
const int segmentPins[] = { 5,5,4,6,7,8,2,3 };
const int nbrDigits= 4; // the number of digits in the LED display
//dig 0 1 2 3
const int digitPins[nbrDigits] = { 13,12,11,10 };

void setup()
{
  Serial.begin(115200);

  delay(3000); // wait for console opening

  if (! rtc.begin()) {
//    Serial.println("Couldn't find RTC");
    while (1);
  }

  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //rtc.adjust(DateTime(2017, 12, 27, 21, 57, 10));

  if (rtc.lostPower()) {
    //Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    //rtc.adjust(DateTime(2017, 12, 27, 22, 6, 10));
  }
  
   for(int i=0; i < 8; i++) {
      pinMode(segmentPins[i], OUTPUT); // set segment and DP pins to output
   }
   for(int i=0; i < nbrDigits; i++) {
      pinMode(digitPins[i], OUTPUT);
   }
   pinMode(colonPin, OUTPUT);
   pinMode(modePin, INPUT);
   pinMode(setPin, INPUT);
} 

 
void loop()
{
   

    DateTime now = rtc.now();

    int rem = now.second() % 2;
    if(MODE == 0) {
      if(rem == 0) digitalWrite(colonPin, HIGH);
      else digitalWrite(colonPin, LOW);
    } else digitalWrite(colonPin, LOW);
    
    MODE_BUTTON_STATE = digitalRead(modePin);
    if(MODE_BUTTON_STATE == HIGH && MODE_BUTTON_PREVSTATE == LOW) {  
      MODE++; 
      if(MODE > 2) { 
        MODE = 0;
        if(timeChanged == 1) {
          rtc.adjust(DateTime(now.year(), now.month(), now.day(), hr, mt, 0));
          timeChanged = 0; 
        }
      }
    } 
    MODE_BUTTON_PREVSTATE = MODE_BUTTON_STATE;
 
    SET_BUTTON_STATE = digitalRead(setPin);
    
    /*Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(now.dayOfTheWeek());
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    */

    if(MODE == 0) { 
      hr = now.hour();
      mt = now.minute();
    } else if(MODE == 1) {
      if(SET_BUTTON_STATE == HIGH && setDelayCtr == 0) { mt++; if(mt > 59) mt = 0; timeChanged = 1; }
    } else if(MODE == 2) {
      if(SET_BUTTON_STATE == HIGH && setDelayCtr == 0) { hr++; if(hr > 23) hr = 0;  timeChanged = 1; }
    }

/*   if(SET_BUTTON_STATE == HIGH && setDelayCtr == 0) {
      baseDelay = 100 +  baseDelay;
      if (baseDelay > 13000) baseDelay = 100;
      Serial.print("BaseDelay = "); Serial.println(baseDelay); 
   }
*/   
   if(hr > 12) hr = hr - 12;
   if(hr == 0) hr = 12;
   int value = hr * 100 + mt;
   
   showNumber(value);
   setDelayCtr++; if(setDelayCtr == SET_DELAY) setDelayCtr = 0;
} 
 
void showNumber( int number)
{
//   if(number == 0) {
//      showDigit( 0, nbrDigits-1) ; // display 0 in the rightmost digit
//   } else {
      // display the value corresponding to each digit
      // leftmost digit is 0, rightmost is one less than the number of places
      for( int digit = 0; digit <nbrDigits; digit++)  {
         if(number > 0)  {
            showDigit( number % 10, digit) ;
            number = number / 10;
         }
      }
//   }
} 
 
// Displays given number on a 7-segment display at the given digit position
void showDigit( int number, int digit)
{
    digitalWrite( digitPins[digit], LOW );

   for(int segment = 1; segment < 8; segment++)  {
      boolean isBitSet = bitRead(numeral[number], segment);
      // isBitSet will be true if given bit is 1
      // isBitSet = ! isBitSet; // Code Option*
      // uncomment the above Code Option line for common anode display
      digitalWrite( segmentPins[segment], isBitSet);
   }
   
   if(MODE == 1 && ((digit == 0) || (digit == 1))) {  
      if(setDelayCtr < (SET_DELAY / 2)) digitalWrite( digitPins[digit], HIGH );
      else digitalWrite( digitPins[digit], LOW );
   } 
   else if(MODE == 2 && ((digit == 2) || (digit == 3))) {  
      if(setDelayCtr < (SET_DELAY / 2)) digitalWrite( digitPins[digit], HIGH );
      else digitalWrite( digitPins[digit], LOW );
   } else digitalWrite( digitPins[digit], HIGH );
   
   
   
   if(number == 1) delayMicroseconds(baseDelay); 
   else if(number == 7)  delayMicroseconds(baseDelay + bias);
   else if(number == 4)  delayMicroseconds(baseDelay + 2*bias);
   else if(number == 3)  delayMicroseconds(baseDelay + 3*bias);
   else if(number == 2)  delayMicroseconds(baseDelay + 3*bias);
   else if(number == 5)  delayMicroseconds(baseDelay + 3*bias);
   else if(number == 6)  delayMicroseconds(baseDelay + 4*bias);
   else if(number == 9)  delayMicroseconds(baseDelay + 4*bias);
   else if(number == 0)  delayMicroseconds(baseDelay + 4*bias);
   else delayMicroseconds(baseDelay + 5*bias);
   digitalWrite( digitPins[digit], LOW );
}
