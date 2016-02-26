// Must include sub-libs to ensure the Include path is set up right...
#include <arduino.h>
//---------------------------------------
#include <avr/power.h>

#include <CheckVoltage.h>
//---------------------------------------
// LEDs used to show the status of the module
#define NOTICE_LED   7

#define SLEEP_INTERVAL .25 // in Minutes
//----------------------------------------------

void setup(void)
{
  Serial.begin(57600*2);

  pinMode(NOTICE_LED, OUTPUT);
  
  setPowerBits() ;
 }

void loop(void)
{
  int voltage = 0;

  digitalWrite( NOTICE_LED, HIGH) ;

  voltage = checkVoltage( 0 ) ;
  Serial.print("Voltage=");
  Serial.println( voltage );
  Serial.flush() ;

  digitalWrite( NOTICE_LED, LOW) ;

  delay( 750 ) ;
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
  //int serialoutput_bit = 0b00000010 ;
  //PRR = PRR | serialoutput_bit ;  /*  Comment out for DEBUG  */

//  ADCSRA = 0;
//  power_adc_disable();
//  power_usart0_disable();
  power_twi_disable();
  power_spi_disable();  // Stops communication with RF24 module
}  

