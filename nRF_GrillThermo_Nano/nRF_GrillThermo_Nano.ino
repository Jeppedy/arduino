// Grill Monitor
// Must include sub-libs to ensure the Include path is set up right...
  #include <SPI.h>
  #include <RF24.h>
//---------------------------------------
#include <arduino.h>
#include <avr/power.h>
#include <Narcoleptic.h>
#include <printfEx.h>
#include <RF24Ex.h>
#include <RF24SendCommon.h>
//---------------------------------------
#define PROBE_KNOWN_RESISTOR_VAL 22000

#define TEMP1_PIN 0   // Pit
#define TEMP2_PIN 1   // Food 1
#define TEMP3_PIN 999 // Food 2

// Set up nRF24L01 radio on SPI pin for CE, CSN
#define CE_PIN  9
#define CS_PIN 10
#define NODE_ID 0xF1

#define WRITE_PIPE 0xF0F0F0F0D1LL

#define SLEEP_INTERVAL 15 // in Seconds
//----------------------------------------------

RF24SendCommon newradio(CE_PIN, CS_PIN, NODE_ID);

void reportTemperatures() {
  float v1, v2, v3 ;
    v1 = v2 = v3 = 999 ;

  if( TEMP1_PIN != 999 )
    v1 = (int)CToF(thermister_temp(analogRead(TEMP1_PIN)));
  if( TEMP1_PIN != 999 )
    v2 = (int)CToF(thermister_temp(analogRead(TEMP2_PIN)));
  if( TEMP1_PIN != 999 )
    v3 = (int)CToF(thermister_temp(analogRead(TEMP3_PIN)));

  if( newradio.sendCommonMsg( v1, v2, v3 ) )
    { printf("Send Successful\n\r") ; }
  else { printf("Send FAILED!\n\r") ;  }
}

double thermister_temp(int aval) {
  double R, T;

  // These were calculated from the thermister data sheet
  //  A = 2.3067434E-4;
  //  B = 2.3696596E-4;
  //  C = 1.2636414E-7;
  // Rknown is the value of the other half of the voltage divider
  R = log((1 / ((1024 / (double) aval) - 1)) * (double) PROBE_KNOWN_RESISTOR_VAL);
  T = (1 / ((2.3067434E-4) + (2.3696596E-4) * R + (1.2636414E-7) * R * R * R)) - 273.25;  // Compute degrees C

  // return degrees F
  return (T);
}

double CToF(double const tCel) {
  return (tCel * 1.8 + 32);
}


void setup() {
//  setPowerBits() ;
	Serial.begin(57600);
  printfEx_begin();

  newradio.init( WRITE_PIPE ) ;
}

void loop() {
  reportTemperatures();
  Serial.flush() ;
  Narcoleptic.delay( SLEEP_INTERVAL*1000ul ) ;  //  Uses Power_Down mode
}

void setPowerBits() {
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

  int serialoutput_bit = 0b00000010 ;
  /*  Comment out for DEBUG  */
  PRR = PRR | serialoutput_bit ;

  ADCSRA = 0;
  power_twi_disable();
  power_adc_disable();
}  

