/*  This is simple program: If power is ON, then OUTPUT is LOW for 5 second,
 *  and then UC read INPUT:
 *  a) if INPUT is LOW then OUTPUT is HIGH
 *  b) if INPUT is HIGH then OUTPUT is LOW
 *  
 *  What we need?
 *  a) 1x ATtiny85
 *  b) 1x Resistor 10k ohm
 *  c) ArduinoIDE (tested 1.8.19)
 *  d) library for ArduinoIDE: https://raw.githubusercontent.com/damellis/attiny/ide-1.6.x-boards-manager/package_damellis_attiny_index.json
 *         
 *  Schematic:
 *  
 *        ATtiny85
 *        (1)  (8) 5 VCC
 * OUTPUT (2)  (7)
 *        (3)  (6)
 *    GND (4)  (5) INPUT (10k pulldown)
 *     |--RESISTOR--|
 */
 
#include <avr/wdt.h> // WatchDog library

void setup() 
{
  wdt_enable(WDTO_8S); // WatchDog check UC 8 second
  
  pinMode(0, INPUT); // (5) INPUT (10k pulldown)
  pinMode(3, OUTPUT); // OUTPUT (2)
  
  digitalWrite(3, LOW); // OUTPUT (2) is LOW
  delay(5000); // wait 5 second
}

void loop() 
{  
  wdt_reset(); // WatchDog reset timer
  
  if(digitalRead(0)) // if (5) INPUT is HIGH
  {
    digitalWrite(3, LOW); // OUTPUT (2) is LOW
  }
  else
  {
    digitalWrite(3, HIGH); // OUTPUT (2) is HIGH
  }
}
