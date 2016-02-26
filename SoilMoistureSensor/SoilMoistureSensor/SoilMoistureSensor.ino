/* ------------------------------------------------------------------------------- */
// Must include sub-libs to ensure the Include path is set up right...
#include <arduino.h>
//---------------------------------------
#include <LiquidCrystal.h>
#include <avr/power.h>
#include <Narcoleptic.h>
#include <printfEx.h>

#include <CheckVoltage.h>
//---------------------------------------

// Connections:
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

LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2);

int backLight =  9;    
int enable    = 10;    

int soilSensor = 0;

#define SLEEP_INTERVAL .06 // in Minutes
//----------------------------------------------

int adcsra = 0;

void setup(void)
{
  Serial.begin(57600);
  printfEx_begin();

  setPowerBits() ;

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  
  pinMode(A0, INPUT);
  
  pinMode(backLight, OUTPUT);
  digitalWrite(backLight, HIGH); // turn backlight on. Replace 'HIGH' with 'LOW' to turn it off.

  lcd.begin(16,2);              // columns, rows.  use 16,2 for a 16x2 LCD, etc.
  lcd.clear();                  // start with a blank screen
  lcd.setCursor(0,0);           // set cursor to column 0, row 0 (the first row)
  lcd.print("Soil Sensor");    // change this text to whatever you like. keep it clean.

//  delay(2000) ;
}

void loop(void)
{
  float v1, v2, v3 ;
    v1 = v2 = v3 = 999 ;
  int voltage = 0;

//  voltage = checkVoltage( adcsra ) ;
//  printf("Voltage= %d\n", voltage ) ;
  int reading  ;
  reading = analogRead(soilSensor);
  printf("Soil Reading = %d\n", reading) ;
  
  lcd.setCursor(0,1);           // set cursor to column 0, row 1
  lcd.print("          ");    // change this text to whatever you like. keep it clean.
  lcd.setCursor(0,1);           // set cursor to column 0, row 1
  lcd.print(reading);

  Serial.flush() ;
  Narcoleptic.delay( SLEEP_INTERVAL*60*1000ul ) ;  //  Uses Power_Down mode
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

  adcsra = ADCSRA;
//  ADCSRA = 0;
//  power_adc_disable();
//  power_usart0_disable();
  power_twi_disable();
  power_spi_disable();  // Stops communication with RF24 module
}  

