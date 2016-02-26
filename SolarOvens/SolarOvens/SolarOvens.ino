//  Solar Oven Sketch
#include <stdlib.h>
#include <avr/power.h>
#include <string.h>

#include <SPI.h>
#include <RF24.h>
#include <OneWire.h>
#include "printfEx.h"

#include <Narcoleptic.h>

// DS18B20 Temperature Sensor 
#define ONEWIRE_PIN 7

// Light Sensor
#define LIGHT_PIN A1

// Set up nRF24L01 radio on SPI pin for CE, CSN
#define RF_CHANNEL 76
#define PAYLOAD_SIZE 21
#define NODE_ID 0xD1
const uint64_t pipes[2] = { 0xF0F0F0F0D1LL, 0xF0F0F0F0E1LL };
RF24 radio(8,9);

void setup(void)
{
//  ADCSRA = 0;
//  power_adc_disable();
  power_twi_disable();
  
  Serial.begin(57600);
  printfEx_begin();

  radio.begin();

  // Enable this seems to work better
  //  radio.enableDynamicPayloads();

  //radio.setDataRate(RF24_1MBPS);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);
//  radio.setPALevel(RF24_PA_MIN);
  radio.setChannel(RF_CHANNEL);
  radio.setRetries(15,15);
  radio.setAutoAck( 0 ) ;   
  radio.setPayloadSize(PAYLOAD_SIZE) ;

  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);

  // Dump the configuration of the rf unit for debugging
  radio.printDetails();

  radio.stopListening();
  
}

float getTemperature(OneWire& ds, bool convertToF) {
  const byte id_DS18S20 = 0x10 ;
  const byte id_DS18B20 = 0x28 ;

  byte i;
  byte data[12];
  byte addr[8];
  float temp;

  int foundOne = 0;

  //find a device
  if (!ds.search(addr)) {  // No devices remain; reset the search
    ds.reset_search();
    return 999;
  }

  if (OneWire::crc8( addr, 7) != addr[7]) {
    printf("Temp CRC invalid\n\r");
    return 998;
  }
  if (addr[0] != id_DS18S20 && addr[0] != id_DS18B20) {
    printf("Not DS Device\n\r");
    return 997;
  }
//  printf("OneWire Device Address: [") ;
//  for (i = 0; i < sizeof(addr); i++)
//  {
//    printf("%02X", addr[i]);
//  }
//  printf("]\n\r");

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
  
  //  Temp calibrations
  if( addr[7] == 0x5D ) {
//    printf( "Adjusting temp -0.5C\n\r") ;
//    temp = temp - 0.5;
  }    
  
  if( convertToF )
    temp = temp * 1.8 + 32 ;
  
//  printf( "Temp [" ) ;
//  printFloat( temp, 1 ) ;
//  printf("]\n\r");

  return temp;
}

int getLight(int whichPin)
{
  int rtn = 0;

  // Get the light value and return it
//  rtn = analogRead(A1);   
//  rtn = map( rtn, 0, 1023, 0, 10) ;
//  printf("Pin1: %d  ", rtn);
//
//  rtn = analogRead(A2);   
//  rtn = map( rtn, 0, 1023, 0, 10) ;
//  printf("Pin2: %d  ", rtn);
//  
//  rtn = analogRead(A3);   
//  rtn = map( rtn, 0, 1023, 0, 10) ;
//  printf("Pin3: %d  ", rtn);
//
//  rtn = analogRead(A4);   
//  rtn = map( rtn, 0, 1023, 0, 10) ;
//  printf("Pin4: %d\n\r", rtn);

  // Get the light value and return it
  rtn = analogRead(whichPin);   
  rtn = map( rtn, 0, 1023, 0, 100) ;
 
  return(rtn) ;
}


void loop(void)
{
  static uint8_t counter=0;

  int whichTempUnit = 0;
  uint16_t light = 999 ;
  
  uint16_t _seq, _t2, _l;
      _seq = _t2 = _l = 9999;

  char temp[5];
  bool timeout=0;

  char outBuffer[32]=""; // Clear the outBuffer before every loop

  static OneWire ds(ONEWIRE_PIN);

  if ( counter > 999 ) counter = 0;
  _seq = counter++;
  
  float t2 ;
  while( (t2 = getTemperature(ds, true)) != 999 ) {
    whichTempUnit++ ;    
    
    //  printf( "DS Temp=[" ); printFloat(t2,1); printf("]\n\r") ;
    light = getLight(whichTempUnit) ;
  
    _t2 = (int16_t) (t2 * 10);
    _l = (int16_t) ( light ) ;  // Already multiplied by 10 in source function
  
    // Append the hex nodeID to the beginning of the payload
    // 0
    sprintf(outBuffer, "D%d", whichTempUnit);   // Send the NodeID as the first part of the payload
  
    // 1 
    strcat(outBuffer,",");
    sprintf(temp,"%03d",_seq);  // Append sequence number to the test payload
    strcat(outBuffer,temp);
  
    // 2 
    strcat(outBuffer,",");
    sprintf(temp,"%04d",_t2);  // Append the Temp
    strcat(outBuffer,temp);
  
    // 3 
    strcat(outBuffer,",");
    sprintf(temp,"%04d",_l);  // Append the Light Sensor
    strcat(outBuffer,temp);
  
    // 4 
    strcat(outBuffer,",");
    sprintf(temp,"%04d",9990);  // Fill empty
    strcat(outBuffer,temp);
  
    printf("outBuffer: %s len: %d ---  ",outBuffer, PAYLOAD_SIZE);
  
    radio.powerUp() ;
    if( radio.write( outBuffer, PAYLOAD_SIZE) ) {
      printf("Send Successful") ;
    } 
    else {
      printf("Send FAILED!") ;
    }
    printf("\n\r") ;
    radio.powerDown() ;
  }

  printf("\n\r");

  Serial.flush() ;

//  Narcoleptic.delay( 5*1000ul ) ;  //  Uses Power_Down mode
  Narcoleptic.delay( 1*60*1000ul ) ;  //  Uses Power_Down mode
}


