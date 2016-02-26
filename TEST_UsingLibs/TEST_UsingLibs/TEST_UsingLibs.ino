//  UNO Sketch
#include <stdlib.h>
#include <avr/power.h>
#include <string.h>

#include <SPI.h>
#include <RF24.h>

#include <OneWire.h>
#include <DS18_TempLib.h>

#include <printfEx.h>

#include <Narcoleptic.h>

// DS18B20 Temperature Sensor 
#define ONEWIRE_PIN 6

// Set up nRF24L01 radio on SPI pin for CE, CSN
#define PAYLOAD_SIZE 21
#define NODE_ID 0xC1
const uint64_t pipes[2] = { 0xF0F0F0F0D1LL, 0xF0F0F0F0E1LL };
RF24 radio(8,9);

void setup(void)
{
  /*
    Bit 7 - PRTWI: Power Reduction TWI
   Bit 6 - PRTIM2: Power Reduction Timer/Counter2
   Bit 5 - PRTIM0: Power Reduction Timer/Counter0
   Bit 4 - Res: Reserved bit
   Bit 3 - PRTIM1: Power Reduction Timer/Counter1
   Bit 2 - PRSPI: Power Reduction Serial Peripheral Interface
   Bit 1 - PRUSART0: Power Reduction USART0
   Bit 0 - PRADC: Power Reduction ADC
   Commonly unneeded peripherals are USART0, TWI, SPI and ADC. 
   To disabled those peripherals, set these in the setup...
   ADCSRA = 0;
   PRR = B10010111;
   */
  // PRR = B10010001;
  //ADCSRA = ADCSRA & B01111111;
  ADCSRA = 0;
  power_twi_disable();
  power_adc_disable();
  
  Serial.begin(57600);
  printfEx_begin();

  radio.begin();

  // Enable this seems to work better
  //  radio.enableDynamicPayloads();

  //radio.setDataRate(RF24_1MBPS);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(76);
  radio.setRetries(15,15);
  radio.setAutoAck( 0 ) ;   
  radio.setPayloadSize(PAYLOAD_SIZE) ;

  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);

  // Dump the configuration of the rf unit for debugging
  radio.printDetails();

  radio.stopListening();
}


void loop(void)
{
  static uint8_t counter=0;
  uint8_t motionSense = 0;

  uint16_t _seq, _t, _empty;
      _seq = _t = _empty = 9999;

  char temp[5];
  char outBuffer[32]=""; // Clear the outBuffer before every loop

  float t = getDS18Temperature(ONEWIRE_PIN, true) ;

  //  printf( "DS Temp=[" ); printFloat(t2,1); printf("]\n\r") ;

  _seq = counter++;
    if ( counter > 999 ) counter = 0;

  _t = (int16_t) (t * 10);

  // 0 - Append the hex nodeID to the beginning of the payload
  sprintf(outBuffer, "%2X", NODE_ID & 0xff);   // Send the NodeID as the first part of the payload

  // 1
  strcat(outBuffer,",");
  sprintf(temp,"%03d",_seq); strcat(outBuffer,temp); // Append sequence number to the test payload

  // 2 
  strcat(outBuffer,",");
  sprintf(temp,"%04d",_t); strcat(outBuffer,temp);

  // 3 
  strcat(outBuffer,",");
  sprintf(temp,"%04d",_empty); strcat(outBuffer,temp);

  // 4 
  strcat(outBuffer,",");
  sprintf(temp,"%04d",_empty); strcat(outBuffer,temp); // Append the Temp from DS18B20

  printf("outBuffer: %s len: %d ---  ",outBuffer, PAYLOAD_SIZE);

  radio.powerUp() ;
  if( radio.write( outBuffer, PAYLOAD_SIZE) ) {
    printf("Send Successful\n\r") ;
  } 
  else {
    printf("Send FAILED!\n\r") ;
  }
  radio.powerDown() ;

  Serial.flush() ;

  Narcoleptic.delay( 5*60*1000ul ) ;  //  Uses Power_Down mode
}

