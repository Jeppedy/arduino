#include "CheckVoltage.h"
#include <arduino.h>
#include <avr/power.h>

// results are Vcc * 100: So for example, 5V would be 500.
int checkVoltage( int origADCSRA, long int _intRefVoltage ) {
//   int curr_adcsra = ADCSRA ;
//  printf("Entrance ADCSRA = %d\n", ADCSRA ) ;
//  printf("passed ADCSRA = %d\n", origADCSRA) ;
//   power_adc_enable();
//   ADCSRA=origADCSRA ;

   // REFS0 : Selects AVcc external reference
   // MUX3 MUX2 MUX1 : Selects 1.1V (VBG)
   ADMUX = bit (REFS0) | bit (MUX3) | bit (MUX2) | bit (MUX1);
   ADCSRA |= bit( ADSC );  // start conversion
   while (ADCSRA & bit (ADSC))
     { }  // wait for conversion to complete
   int results = (((_intRefVoltage * 1024) / ADC) + 5) / 10;

//   ADCSRA=curr_adcsra;
//  printf("Exit ADCSRA = %d\n", ADCSRA ) ;

   return results;
}

