// Aquarium Monitor with LED Warning LED
#include <arduino.h>
#include <stdlib.h>
#include <string.h>

#include <printfEx.h>

// LED Pin
#define LED_PIN 13

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long intervalLED = 700;           // interval at which to blink (milliseconds)
long previousMillisLED = 0;        // will store last time LED was updated
int  ledState = LOW;             // ledState used to set the LED

void setup(void)
{
  pinMode(LED_PIN, OUTPUT);
  printfEx_begin();
}

void loop(void)
{
  static uint8_t counter=0;

    unsigned long currentMillisLED = millis();
    
    if(currentMillisLED - previousMillisLED > intervalLED) {
      previousMillisLED = currentMillisLED;   
      ledState = (ledState == LOW ? HIGH : LOW ) ;
    }  
  digitalWrite(LED_PIN, ledState);  // set the LED with the ledState of the variable

}


