// Aquarium Monitor with Warning LED
// Must include sub-libs to ensure the Include path is set up right...
  #include <SPI.h>
  #include <RF24.h>
  #include <OneWire.h>
//---------------------------------------
#include <arduino.h>
#include <avr/power.h>
#include <Narcoleptic.h>
#include <printfEx.h>
#include <RF24Ex.h>
#include <RF24SendCommon.h>

#include <DS18_TempLib.h>
//---------------------------------------
#define TEMP1_PIN 6   // Aquarium
#define TEMP2_PIN 7   // Room
#define TEMP3_PIN 999 // 

// Set up nRF24L01 radio on SPI pin for CE, CSN
#define CE_PIN  9
#define CS_PIN 10
#define NODE_ID 0xC2

#define SLEEP_INTERVAL 15 // in Seconds
//----------------------------------------------

RF24SendCommon newradio(CE_PIN, CS_PIN, NODE_ID);

void setup(void)
{
  setPowerBits() ;
  Serial.begin(57600);
  printfEx_begin();
}

void loop(void)
{
  float v1, v2, v3 ;
    v1 = v2 = v3 = 999 ;

  if( TEMP1_PIN != 999 )
    v1 = getDS18Temperature(TEMP1_PIN, true) ;
  if( TEMP2_PIN != 999 )
    v2 = getDS18Temperature(TEMP2_PIN, true) ;
  if( TEMP3_PIN != 999 )
    v3 = getDS18Temperature(TEMP3_PIN, true) ;

  if( newradio.sendCommonMsg( v1, v2, v3 ) )
    { printf("Send Successful\n\r") ; }
  else { printf("Send FAILED!\n\r") ;  }

  Serial.flush() ;
  Narcoleptic.delay( SLEEP_INTERVAL*60*1000ul ) ;  
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

