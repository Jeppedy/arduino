// Must include sub-libs to ensure the Include path is set up right...
#include <arduino.h>
  #include <SPI.h>
  #include <RF24.h>
  #include <OneWire.h>
//---------------------------------------
#include <avr/power.h>
#include <Narcoleptic.h>
#include <printfEx.h>

#include <CheckVoltage.h>

#include <EEPROM.h>
#include <ReadEEPROMDeviceID.h>

#include <RF24Ex.h>
#include <RF24SendCommon.h>

#include <DS18_TempLib.h>
//---------------------------------------
#define TEMP1_PIN 8   // 
#define TEMP2_PIN 7   // 
#define TEMP3_PIN 999 // 

#define BATTERY_VOLTAGE  A0

// Set up nRF24L01 radio on SPI pin for CE, CSN
#define CE_PIN  9
#define CS_PIN 10
#define DEFAULT_NODE_ID 0xFF

// LEDs used to show the status of the module
#define NOTICE_LED   5
#define ERROR_LED    6

// Default const int refVoltageOffset = 1100 ;
const int refVoltageOffset  = 1100 ;
const int battVoltageOffset =    0 ;  // Every ~30 points is a 0.10 volt offset
//  HOME Batt Offset = -100

#define SLEEP_INTERVAL  5 // in Minutes
//----------------------------------------------

RF24SendCommon newradio(CE_PIN, CS_PIN, 0x00);

void setup(void)
{
  Serial.begin(57600);
  printfEx_begin();

  setPowerBits() ;
 
  digitalWrite( NOTICE_LED, LOW) ;
  pinMode(NOTICE_LED,OUTPUT);
  digitalWrite( ERROR_LED, LOW) ;
  pinMode(ERROR_LED,OUTPUT);

  pinMode(BATTERY_VOLTAGE,INPUT);

  newradio.init() ;

  // Use the device ID from EEPROM, if present.
  int nodeID = readEEPROMDeviceID();
  if( !nodeID ) {
    Serial.println("Invalid EEPROM address found. Using default.") ;
    nodeID = DEFAULT_NODE_ID ;
  }   
  newradio.nodeID( nodeID ) ;
  Serial.print("Device ID is now: " ) ;
  Serial.println( newradio.nodeID(), HEX ) ;

}

void loop(void)
{
  float v1, v2, v3 ;
    v1 = v2 = v3 = 999 ;
  int voltage = 0;
  int batteryVoltage = 0;

  digitalWrite( NOTICE_LED, HIGH) ;

  batteryVoltage = analogRead(BATTERY_VOLTAGE) ;
//  Serial.print("Raw Battery Charge: ") ;
//  Serial.println( batteryVoltage ) ;
  batteryVoltage += battVoltageOffset ;
  batteryVoltage = map(batteryVoltage, 0, 1023, 0, 330);
//  Serial.print("Mapped Battery Charge: ") ;
//  Serial.println( batteryVoltage ) ;

  if( batteryVoltage > 0 ) {
    voltage = batteryVoltage ;
  } else {
    voltage = checkVoltage( refVoltageOffset ) ;
//    printf("Internal voltage= %d\n", voltage ) ;
  }
//  printf("Voltage= %d\n", voltage ) ;

  if( TEMP1_PIN != 999 )
    v1 = getDS18Temperature(TEMP1_PIN, true) ;
  if( TEMP2_PIN != 999 )
    v2 = getDS18Temperature(TEMP2_PIN, true) ;

  if( TEMP3_PIN != 999 )
    v3 = getDS18Temperature(TEMP3_PIN, true) ;
  else
    v3 = voltage ;

  digitalWrite( NOTICE_LED, LOW) ;

  if( newradio.sendCommonMsg( v1, v2, v3 ) ) { 
    printf("Send Successful\n\r") ; 
  }
  else { 
    printf("Send FAILED!\n\r") ;  
    blinkErrorLED() ;
  }

  Serial.flush() ;
  Narcoleptic.delay( SLEEP_INTERVAL*60*1000ul ) ;  //  Uses Power_Down mode
}

void blinkStatusLED( void ) {
  digitalWrite( NOTICE_LED, true) ;
  delay(40) ;
  digitalWrite( NOTICE_LED, false) ;
  return ;
}
void blinkErrorLED( void ) {
  digitalWrite( ERROR_LED, true) ;
  delay(90) ;
  digitalWrite( ERROR_LED, false) ;
  delay(30) ;
  digitalWrite( ERROR_LED, true) ;
  delay(90) ;
  digitalWrite( ERROR_LED, false) ;
  delay(30) ;
  digitalWrite( ERROR_LED, true) ;
  delay(90) ;
  digitalWrite( ERROR_LED, false) ;
  return ;
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
//  power_usart0_disable();
//  power_adc_disable();
  power_twi_disable();
  // power_spi_disable();  // Stops communication with RF24 module
}  

