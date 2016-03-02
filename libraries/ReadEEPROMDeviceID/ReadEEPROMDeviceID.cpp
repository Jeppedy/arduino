#include <stdlib.h>
#include <EEPROM.h>

#include <ReadEEPROMDeviceID.h>

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
  // else return the zero address

  return _nodeID;
}

