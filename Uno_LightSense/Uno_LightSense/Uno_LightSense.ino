//  UNO Sketch
#include <stdlib.h>
//#include <avr/power.h>
#include <string.h>

#include <SPI.h>   // WHY DO I NEED TO INCLUDE THIS????

#include "printfEx.h"

#include <Narcoleptic.h>

// Light Sensor
#define LIGHT_PIN A2

void setup(void)
{
//  ADCSRA = 0;
//  power_twi_disable();
//  power_adc_disable();
  
  Serial.begin(57600);
  printfEx_begin();

}

int getLight(int whichPin)
{
  int rtn = 0;

  // Get the light value and return it
  rtn = analogRead(whichPin);   
  printf("%06d",rtn);  // Append the Light from the photosensor
  
  return(rtn) ;
}


void loop(void)
{
  uint16_t _l = 9999;

  char temp[5];
  bool timeout=0;

  char outBuffer[32]=""; // Clear the outBuffer before every loop

  uint16_t light = getLight(LIGHT_PIN) ;

  _l = (int16_t) ( light * 10) ;

  sprintf(temp,"%04d",_l);  // Append the Light from the photosensor
  strcat(outBuffer,temp);

  printf("outBuffer: %s ---  \n\r",outBuffer);

  Serial.flush() ;

//  Narcoleptic.delay( 5*60*1000ul ) ;  //  Uses Power_Down mode
  Narcoleptic.delay( 3*1000ul ) ;  //  Uses Power_Down mode
}


