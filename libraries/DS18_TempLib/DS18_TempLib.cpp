#include "DS18_TempLib.h"
#include <OneWire.h>

float getDS18Temperature(int pin_, bool convertToF) {
  const byte id_DS18S20 = 0x10 ;
  const byte id_DS18B20 = 0x28 ;

  byte i;
  byte data[12];
  byte addr[8];
  float temp;

  OneWire ds(pin_);

  //find a device
  if (!ds.search(addr)) {
    ds.reset_search();
    return 999;
  }
  if (OneWire::crc8( addr, 7) != addr[7]) {
    return 998;
  }
  if (addr[0] != id_DS18S20 && addr[0] != id_DS18B20) {
    return 997;
  }
  //   printf("OneWire Device Address: [") ;
  //   for (i = 0; i < sizeof(addr); i++)
  //   {
  //       printf("%02X", addr[i]);
  //   }
  //   printf("]\n\r");

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
  return temp;
}


