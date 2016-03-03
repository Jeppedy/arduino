#include "CheckVoltage.h"
#include <arduino.h>
#include <avr/power.h>

// results are Vcc * 100: So for example, 5V would be 500.
int checkVoltage( long int _intRefVoltage ) {
   // REFS0 : Selects AVcc external reference
   // MUX3 MUX2 MUX1 : Selects 1.1V (VBG)
   ADMUX = bit (REFS0) | bit (MUX3) | bit (MUX2) | bit (MUX1);
	   delayMicroseconds( 250 ) ;  // Some reference suggested this helps
   bitSet( ADCSRA, ADSC ) ;
   while (bit_is_set(ADCSRA, ADSC))
     { }  // wait for conversion to complete
   word x = ADC ;
   int results = ((_intRefVoltage * 1023) / x) / 10;

   return results;
}
