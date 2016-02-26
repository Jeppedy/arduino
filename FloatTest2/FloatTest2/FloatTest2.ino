#include <Arduino.h>
#include "printfEx.h"

void setup() {
   Serial.begin(57600); 
   printfEx_begin();

}

void loop() {
  Serial.write( "Starting" ) ;

  uint8_t Data2 = 0;
  float t = 12.3 ;
  Data2 = (int) (t*10) ;
  printFloatEx(t,1) ;
  printf( "i: %i   d: %d", Data2) ;

  

  delay(10000);
  
  Serial.println();

}
