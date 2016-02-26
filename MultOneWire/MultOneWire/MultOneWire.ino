//  Solar Oven Sketch
#include <stdlib.h>
#include <avr/power.h>
#include <string.h>

#include <printfEx.h>

#include <SPI.h>
#include <RF24.h>
#include <OneWire.h>

#include <Narcoleptic.h>

// DS18B20 Temperature Sensor 
#define ONEWIRE_PIN 6

void setup(void)
{
  Serial.begin(57600);
  printfEx_begin();

}

void getTemperature(int pin_, bool convertToF) {
  const byte id_DS18S20 = 0x10 ;
  const byte id_DS18B20 = 0x28 ;

  byte i;
  byte data[12];
  byte addr[8];
  float temp;

  OneWire ds(pin_);

  int foundOne = 0;
  
  //find a device
  while (ds.search(addr)) {

    if (OneWire::crc8( addr, 7) != addr[7]) {
      printf("Temp CRC invalid\n\r");
      return ;  // Temp reading not valid
    }
    if (addr[0] != id_DS18S20 && addr[0] != id_DS18B20) {
      printf("Not a valid DS Device\n\r");
      return ;  // Not valid DS temp sensor
    }
    
    foundOne = 1;
    
    printf("OneWire Device Address: [") ;
    for (i = 0; i < sizeof(addr); i++)
    {
      printf("%02X", addr[i]);
    }
    printf("] ");
  
    ds.reset();
    ds.select(addr);
    // Start conversion
    ds.write(0x44, 1);
    // Wait some time...
    delay(850);
    ds.reset();
    ds.select(addr);
    // Issue Read scratchpad command
    ds.write(0xBE);
    // Receive 9 bytes
    for ( i = 0; i < 9; i++) {
      data[i] = ds.read();
    }
    
    // Calculate temperature value
    temp = ( (data[1] << 8) + data[0] )*0.0625;
    if( convertToF )
      temp = temp * 1.8 + 32 ;
      
    printf( "Temp [" ) ;
    printFloatEx( temp, 1 ) ;
    printf("]\n\r");
  }
  
  if( !foundOne ) {
//    ds.reset_search();
    printf("No temp sensors found\n\r");
    return ;
  }
  
  return ;
}

void loop(void)
{
  char temp[5];

  getTemperature(ONEWIRE_PIN, true) ;

  Serial.flush() ;

  Narcoleptic.delay( 10*1000ul ) ;  //  Uses Power_Down mode
}


