//  UNO Sketch
#include <stdlib.h>
#include <avr/power.h>
#include <string.h>

#include <SPI.h>
#include <RF24.h>
#include <OneWire.h>
#include <DHT.h>
#include "printfEx.h"

#include <Narcoleptic.h>

// DS18B20 Temperature Sensor 
#define ONEWIRE_PIN 6

// DHT Temperature/Humidity Sensor
#define DHTPIN 7
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Motion Sensor
#define MOTION_PIN 8

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

  dht.begin() ;
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
  
  pinMode(MOTION_PIN, INPUT);

}

float getTemperature(int pin_, bool convertToF) {
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


void loop(void)
{
  static uint8_t counter=0;
  uint8_t motionSense = 0;

  uint16_t _seq, _t, _h, _t2, _m;
      _seq = _t = _h = _t2 = _m = 9999;

  char temp[5];
  bool timeout=0;

  char outBuffer[32]=""; // Clear the outBuffer before every loop

  float h = dht.readHumidity();
  float t = dht.readTemperature(true);
  float t2 = getTemperature(ONEWIRE_PIN, true) ;

  motionSense = digitalRead(MOTION_PIN); //getting values from IR sensor
  motionSense = (motionSense>0?1:0) ;

  //  printf( "DS Temp=[" ); printFloat(t2,1); printf("]\n\r") ;

  // Get readings from sensors
  _seq = counter++;
  _t = (int16_t) ( t * 10);
  _h = (int16_t) ( h * 10);
  _t2 = (int16_t) (t2 * 10);
  _m = (int16_t) (motionSense * 10);

  if ( counter > 999 ) counter = 0;

  // Append the hex nodeID to the beginning of the payload
  // 0
  sprintf(outBuffer, "%2X", NODE_ID & 0xff);   // Send the NodeID as the first part of the payload

  // 1 
  strcat(outBuffer,",");
  sprintf(temp,"%03d",_seq);  // Append sequence number to the test payload
  strcat(outBuffer,temp);

  // 2 
  strcat(outBuffer,",");
  sprintf(temp,"%04d",_t2);  // Append the Temp from DHT11
  strcat(outBuffer,temp);

  // 3 
  strcat(outBuffer,",");
  sprintf(temp,"%04d",_h);  // Append the Humidity from DHT11
  strcat(outBuffer,temp);

  // 4 
  strcat(outBuffer,",");
  sprintf(temp,"%04d",_t);  // Append the Temp from DS18B20
  // sprintf(temp,"%04d",_m);  // Append Motion Sensor
  strcat(outBuffer,temp);

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


