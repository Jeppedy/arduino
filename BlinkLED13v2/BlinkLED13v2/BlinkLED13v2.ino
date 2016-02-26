// Aquarium Monitor with LED Warning LED
#include <arduino.h>
#include <stdlib.h>
#include <avr/power.h>
#include <string.h>

#include "printfEx.h"

#include <Narcoleptic.h>

// LED Pin
#define LED_PIN 13



// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long intervalLED = 700;           // interval at which to blink (milliseconds)
long previousMillisLED = 0;        // will store last time LED was updated
int  ledState = LOW;             // ledState used to set the LED


int getBandgap(void) // Returns actual value of Vcc (x 100)
{
        
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
     // For mega boards
     const long InternalReferenceVoltage = 1115L;  // Adjust this value to your boards specific internal BG voltage x1000
        // REFS1 REFS0          --> 0 1, AVcc internal ref. -Selects AVcc reference
        // MUX4 MUX3 MUX2 MUX1 MUX0  --> 11110 1.1V (VBG)         -Selects channel 30, bandgap voltage, to measure
     ADMUX = (0<<REFS1) | (1<<REFS0) | (0<<ADLAR)| (0<<MUX5) | (1<<MUX4) | (1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (0<<MUX0);
  
#else
     // For 168/328 boards
     const long InternalReferenceVoltage = 1056L;  // Adjust this value to your boards specific internal BG voltage x1000
        // REFS1 REFS0          --> 0 1, AVcc internal ref. -Selects AVcc external reference
        // MUX3 MUX2 MUX1 MUX0  --> 1110 1.1V (VBG)         -Selects channel 14, bandgap voltage, to measure
     ADMUX = (0<<REFS1) | (1<<REFS0) | (0<<ADLAR) | (1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (0<<MUX0);
       
#endif
     delay(50);  // Let mux settle a little to get a more stable A/D conversion
        // Start a conversion  
     ADCSRA |= _BV( ADSC );
        // Wait for it to complete
     while( ( (ADCSRA & (1<<ADSC)) != 0 ) );
        // Scale the value
     int results = (((InternalReferenceVoltage * 1023L) / ADC) + 5L) / 10L; // calculates for straight line value 
     return results;
 
}
    
    
void setup(void)
{
  /*
   Bit 7 - PRTWI: Power Reduction TWI
   Bit 6 - PRTIM2: Power Reduction Timer/Counter2
   Bit 5 - PRTIM0: Power Reduction Timer/Counter0 (If disabled, Sleep never returns!)
   Bit 4 - Res: Reserved bit
   Bit 3 - PRTIM1: Power Reduction Timer/Counter1
   Bit 2 - PRSPI: Power Reduction Serial Peripheral Interface
   Bit 1 - PRUSART0: Power Reduction USART0 (If disabled, no Serial debugging)
   Bit 0 - PRADC: Power Reduction ADC
   Commonly unneeded peripherals are USART0, TWI, SPI and ADC. 
   To disabled those peripherals, set these in the setup...
   ADCSRA = 0;
   PRR = B10010111;
  */
  int serialoutput_bit = 0b00000010 ;
  
//  ADCSRA = 0;
//  power_twi_disable();
//  power_adc_disable();
  PRR = 0B11011101;
  if( 0 )
    PRR = PRR | serialoutput_bit ;

  
  Serial.begin(57600);
  printfEx_begin();

  pinMode(LED_PIN, OUTPUT);
}

void loop(void)
{
  static uint8_t counter=0;
  char temp[5];

  char outBuffer[32]=""; // Clear the outBuffer before every loop

  // check to see if it's time to blink the LED; that is, if the 
  // difference between the current time and last time you blinked 
  // the LED is bigger than the interval at which you want to 
  // blink the LED.
    unsigned long currentMillisLED = millis();
    
    printf( "LED Millis Curr[%ld]  Prev[%ld], Counter [%d]\n\r", currentMillisLED, previousMillisLED, counter++ ) ;

    if(currentMillisLED - previousMillisLED > intervalLED) {
      // save the last time you blinked the LED 
      previousMillisLED = currentMillisLED;   
      printf("Current Millis LED[%ld]\n\r", currentMillisLED) ;
  
      // if the LED is off turn it on and vice-versa:
      ledState = (ledState == LOW ? HIGH : LOW ) ;
    }  
  digitalWrite(LED_PIN, ledState);  // set the LED with the ledState of the variable
  printf( "Set LED to [%d]\n\r", ledState ) ;

   int battVolts = getBandgap();  //Determins what actual Vcc is, (X 100), based on known bandgap voltage
   Serial.print("Battery Vcc volts =  ");
   Serial.println(battVolts);
   Serial.println();    


  Serial.flush() ;

//  NOTE: Narcoleptic goes into power-down which means Millis() isn't updated.
//  Narcoleptic.delay( 5*1000ul ) ;  //  Uses Power_Down mode
  delay(2*1000ul) ;
}


