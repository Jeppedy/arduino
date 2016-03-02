/* -----------------------------------------------------------------------------------
 * Adaption of the LiquidCrystal library shipped with Arduino 22
 * for use with 74HC595 shift register adapter board found on:
 * http://www.stephenhobley.com
 
 * Code adaption by Steve Hobley - February 2011

 /*---Shift Register 74HC595---
 * [SR Pin 14 (DS)]    to Arduino pin - Yellow wire [datapin]
 * [SR Pin 12 (ST_CP)] to Arduino pin - Green wire  [latchpin]
 * [SR Pin 11 (SH_CP)] to Arduino pin - White wire  [clockpin]
 * Black wire to Ground
 * Red wire to +5v

 -----Shift Reg to LCD--------
 * SR Pin 15  - ENABLE        10000000
 * SR Pin 1   - D4            00000010 
 * SR Pin 2   - D5			  00000100
 * SR Pin 3   - D6			  00001000
 * SR Pin 4   - D7			  00010000
 * SR Pin 5   - MOSFET / LED1 00100000
 * SR Pin 6   - LED 2         01000000
 * SR Pin 7   - RS            00000001
 *
 * -----------------------------------------------------------------------------------
 */

#ifndef LiquidCrystal595_h
#define LiquidCrystal595_h

#include <inttypes.h>
#include "Print.h"

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

class LiquidCrystal595 : public Print {
public:
  LiquidCrystal595(uint8_t datapin, uint8_t latchpin, uint8_t clockpin);

  void init(uint8_t datapin, uint8_t latchpin, uint8_t clockpin);
    
  void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS);

  void clear();
  void home();

  void noDisplay();
  void display();
  void noBlink();
  void blink();
  void noCursor();
  void cursor();
  void scrollDisplayLeft();
  void scrollDisplayRight();
  void leftToRight();
  void rightToLeft();
  void autoscroll();
  void noAutoscroll();

  void createChar(uint8_t, uint8_t[]);
  void setCursor(uint8_t, uint8_t); 
  virtual void write(uint8_t);
  void command(uint8_t);
  
    // Moved to public - to aid with debugging, and other uses for the library etc...
  void setRSPin(uint8_t pinValue);
  void setEPin(uint8_t pinValue);
  void setD4Pin(uint8_t pinValue);
  void setD5Pin(uint8_t pinValue);
  void setD6Pin(uint8_t pinValue);
  void setD7Pin(uint8_t pinValue);
  void setLED1Pin(uint8_t pinValue);
  void setLED2Pin(uint8_t pinValue);
  void shift595();
  
private:
  void send(uint8_t, uint8_t);
  void write4bits(uint8_t);
  void write8bits(uint8_t);
  void pulseEnable();

  uint8_t _datapin;
  uint8_t _latchpin;
  uint8_t _clockpin;
  char _register; //Stores the current state of the data
 
  uint8_t _displayfunction;
  uint8_t _displaycontrol;
  uint8_t _displaymode;

  uint8_t _initialized;

  uint8_t _numlines,_currline;
};

#endif
