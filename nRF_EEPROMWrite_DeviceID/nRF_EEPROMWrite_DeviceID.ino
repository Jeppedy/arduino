#include <EEPROM.h>

char deviceID[] = "C3" ;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);

  writeEEPROMDeviceID( deviceID ) ;

  int myID = readEEPROMDeviceID() ;
  if( myID > 0 ) {
    Serial.print("Found valid device ID: ") ; Serial.println( myID, HEX ) ;        
  }
  else {
    Serial.println("Invalid EEPROM content found; Not a device address") ;
  }

}

void writeEEPROMDeviceID( char* deviceIDIn )
{
  char guardChar1 = '+' ;
  char guardChar2 = '=' ;
  
  EEPROM.write(0, guardChar1);  
  EEPROM.write(1, guardChar2);  
  EEPROM.write(2, deviceIDIn[0]);  
  EEPROM.write(3, deviceIDIn[1]);
  EEPROM.write(4, guardChar2);  
  EEPROM.write(5, guardChar1);  

  return ;
}

int readEEPROMDeviceID( )
{
  // Required format is  +=xx=+ , where xx is in the form 'E3'
  char nodeID[2+1] ;
  int _nodeID = 0 ;
  if( (char)EEPROM.read(0) == '+' and
      (char)EEPROM.read(1) == '=' and
      (char)EEPROM.read(4) == '=' and
      (char)EEPROM.read(5) == '+' ) {
        nodeID[0] = (char)EEPROM.read(2) ;
        nodeID[1] = (char)EEPROM.read(3) ;
        nodeID[2] = '\0' ;

        _nodeID = strtol(nodeID, NULL, 16) ;
  }   
  else {
    Serial.println("ReadEEPROM: Invalid EEPROM content found; Not a device address") ;
  }
  return _nodeID;  
}

void loop() {
  delay(1000) ;
}
