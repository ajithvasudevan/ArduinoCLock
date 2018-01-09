#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;
int colonPin = 13;

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
const int segmentPins[] = { 1,8,7,6,5,4,3,2 };
const int nbrDigits= 4; // the number of digits in the LED display
//dig 0 1 2 3
const int digitPins[nbrDigits] = { 9,10,11,12 };

void setup()
{
  Serial.begin(115200);

  delay(3000); // wait for console opening

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    rtc.adjust(DateTime(2017, 12, 19, 17, 42, 0));
  }
  
   for(int i=0; i < 8; i++) {
      pinMode(segmentPins[i], OUTPUT); // set segment and DP pins to output
   }
   for(int i=0; i < nbrDigits; i++) {
      pinMode(digitPins[i], OUTPUT);
   }
   pinMode(colonPin, OUTPUT);
} 

 
void loop()
{
   

    DateTime now = rtc.now();

    int rem = now.second() % 2;
    if(rem == 0) digitalWrite(colonPin, HIGH);
    else digitalWrite(colonPin, LOW);
    
    Serial.print(now.year(), DEC);
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
    
   int value = now.hour() * 100 + now.minute();
   Serial.println(value);
   
   showNumber(value);
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
      }y
//   }
} 
 
// Displays given number on a 7-segment display at the given digit position
void showDigit( int number, int digit)
{
   digitalWrite( digitPins[digit], HIGH );
   for(int segment = 1; segment < 8; segment++)  {
      boolean isBitSet = bitRead(numeral[number], segment);
      // isBitSet will be true if given bit is 1
      // isBitSet = ! isBitSet; // Code Option*
      // uncomment the above Code Option line for common anode display
      digitalWrite( segmentPins[segment], isBitSet);
   }
   delay(4);
   digitalWrite( digitPins[digit], LOW );
}
