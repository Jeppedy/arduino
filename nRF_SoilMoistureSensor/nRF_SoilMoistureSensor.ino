/* ------------------------------------------------------------------------------- */
// Must include sub-libs to ensure the Include path is set up right...

//---------------------------------------
#include <arduino.h>
  #include <SPI.h>
  #include <RF24.h>
  #include <EEPROM.h>
#include <avr/power.h>
#include <Narcoleptic.h>
#include <printfEx.h>

#include <CheckVoltage.h>
#include <ReadEEPROMDeviceID.h>

#include <RF24Ex.h>
#include <RF24SendCommon.h>

//#include <LiquidCrystal.h>
//---------------------------------------
#define PIN1 A0   // Plant Moisture
#define PIN2 999 // 
#define PIN3 999 // 
#define SOIL_POWER A1   // Plant Moisture

// Set up nRF24L01 radio on SPI pin for CE, CSN
#define CE_PIN  9
#define CS_PIN 10
#define DEFAULT_NODE_ID 0xC5

// LEDs used to show the status of the module
#define NOTICE_LED   7
#define ERROR_LED    8

const int refVoltageOffset = 1100 ;

#define SLEEP_INTERVAL  15  // in Minutes

// LCD Connections:
// LCD pin  1 - vss- to Arduino Ground
// LCD pin  2 - vdd- to Arduino +5v
// LCD pin  3 - v0 - variable resistance to ground: Contrast Control
// LCD pin  4 - rs - to Arduino pin 12 - Register Select
// LCD pin  5 - rw - to Arduino pin 11 - Read/Write Control
// LCD pin  6 - e  - to Arduino pin 10 - Enable
// LCD pin  7      - none
// LCD pin  8      - none
// LCD pin  9      - none
// LCD pin 10      - none
// LCD 11 to 14    - d4, d5, d6, d7 - to Arduino 5,4,3,2 - Data Pins
// LCD pin 15 - A  - to Arduino pin 9 (with series resistor) - Backlight Power
// LCD pin 16 - K  - to Arduino Ground - Backlight Ground
//#LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2);
//#int backLight =  9;    
//#int enable    = 10;    
//----------------------------------------------

RF24SendCommon radio(CE_PIN, CS_PIN, 0x00);


void setup(void)
{
  Serial.begin(57600);
  printfEx_begin();

  setPowerBits() ;

  pinMode(PIN1, INPUT);
  pinMode(SOIL_POWER, OUTPUT);  // Controls power to sensor

  digitalWrite( NOTICE_LED, LOW) ;
  pinMode(NOTICE_LED,OUTPUT);
  digitalWrite( ERROR_LED, LOW) ;
  pinMode(ERROR_LED,OUTPUT);

  radio.init() ;

  // Use the device ID from EEPROM, if present.
  int nodeID = readEEPROMDeviceID();
  if( !nodeID ) {
    Serial.println("Invalid EEPROM address found. Using default.") ;
    nodeID = DEFAULT_NODE_ID ;
  }   
  radio.nodeID( nodeID ) ;
  Serial.print("Device ID is: " ) ;
  Serial.println( radio.nodeID(), HEX ) ;


//#  pinMode(backLight, OUTPUT);
//#  digitalWrite(backLight, HIGH); // turn backlight on. Replace 'HIGH' with 'LOW' to turn it off.

//#  lcd.begin(16,2);              // columns, rows.  use 16,2 for a 16x2 LCD, etc.
//#  lcd.clear();                  // start with a blank screen
//#  lcd.setCursor(0,0);           // set cursor to column 0, row 0 (the first row)
//#  lcd.print("Soil Sensor");    // change this text to whatever you like. keep it clean.
}

void loop(void)
{
  float v1, v2, v3 ;
    v1 = v2 = v3 = 999 ;
  int soilReading = 0;
  int soilReading1, soilReading2, soilReading3;
  int voltage = 0;

  digitalWrite( NOTICE_LED, HIGH) ;

  digitalWrite( SOIL_POWER, HIGH ) ;
    delay(500) ;  

  soilReading1 = analogRead(PIN1);
  printf("Soil Reading = %d\n", soilReading1) ;
  soilReading2 = analogRead(PIN1);
  printf("Soil Reading = %d\n", soilReading2) ;
  soilReading3 = analogRead(PIN1);
  printf("Soil Reading = %d\n", soilReading3) ;

  digitalWrite( SOIL_POWER, LOW) ;

  soilReading = (soilReading1 + soilReading2 + soilReading3 ) / 3 ;
  printf("Average Soil Reading = %d\n", soilReading) ;
  
  if( soilReading >= 999 )  soilReading = 990 ;  // Greater than 990 is a trigger value

    delay(1000) ;  
  voltage = checkVoltage( refVoltageOffset ) ;
//  printf("Voltage= %d\n", voltage ) ;
  v3 = voltage ;

  digitalWrite( NOTICE_LED, LOW) ;

//#  lcd.setCursor(0,1);           // set cursor to column 0, row 1
//#  lcd.print("          ");    // change this text to whatever you like. keep it clean.
//#  lcd.setCursor(0,1);           // set cursor to column 0, row 1
//#  lcd.print(reading);

  if( radio.sendCommonMsg( soilReading, v1, v2 ) ) { 
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
//  power_adc_disable();
//  power_usart0_disable();
  power_twi_disable();
//  power_spi_disable();  // Stops communication with RF24 module
}  

