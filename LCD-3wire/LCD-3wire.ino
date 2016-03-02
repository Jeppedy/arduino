/* ------------------------------------------------------------------------------- */
// character LCD example code
// www.hacktronics.com

#include <LiquidCrystal.h>

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

void setup()
{
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  
  pinMode(backLight, OUTPUT);
  digitalWrite(backLight, HIGH); // turn backlight on. Replace 'HIGH' with 'LOW' to turn it off.
  lcd.begin(16,2);              // columns, rows.  use 16,2 for a 16x2 LCD, etc.
  lcd.clear();                  // start with a blank screen
  lcd.setCursor(0,0);           // set cursor to column 0, row 0 (the first row)
  lcd.print("Hello, Nate");    // change this text to whatever you like. keep it clean.
  lcd.setCursor(0,1);           // set cursor to column 0, row 1
  lcd.print("theherrs.com");
  
  // if you have a 4 row LCD, uncomment these lines to write to the bottom rows
  // and change the lcd.begin() statement above.
  //lcd.setCursor(0,2);         // set cursor to column 0, row 2
  //lcd.print("Row 3");
  //lcd.setCursor(0,3);         // set cursor to column 0, row 3
  //lcd.print("Row 4");
}

void loop()
{
//  digitalWrite(backLight, HIGH); // turn backlight on. Replace 'HIGH' with 'LOW' to turn it off.
//  lcd.noDisplay() ;
//  delay(2000) ;
//  digitalWrite(backLight, LOW); // turn backlight on. Replace 'HIGH' with 'LOW' to turn it off.
//  lcd.display() ;
//  delay(2000) ;
  delay(1000) ;

  for( int i = 0; i<5; i++) {
    delay(200) ;
    lcd.scrollDisplayRight() ;
  }
  for( int i = 0; i<5; i++) {
    delay(200) ;
    lcd.scrollDisplayLeft() ;
  }
  for( int i = 0; i<5; i++) {
    delay(200) ;
    lcd.scrollDisplayLeft() ;
  }
  for( int i = 0; i<5; i++) {
    delay(200) ;
    lcd.scrollDisplayRight() ;
  }
  delay(1000) ;
}
/* ------------------------------------------------------------------------------- */
