#include <RF24SendCommon.h>
#include <RF24Ex.h>
#include <RF24.h>
#include <SPI.h>

#define BUF_LEN 21

RF24SendCommon::RF24SendCommon(uint8_t _cepin, uint8_t _cspin, int _node, int buflen) :
	_radioEx(_cepin, _cspin), _nodeID(_node), _bufLen(buflen), _isInit(false)
{
}

void RF24SendCommon::init( uint64_t writePipe, uint64_t readPipe ) {
	_isInit = true;
	if( writePipe == 0 )
	  _radioEx.initRadio( _bufLen ) ;
	else if( readPipe == 0)
	  _radioEx.initRadio( _bufLen, writePipe ) ;
	else
	  _radioEx.initRadio( _bufLen, writePipe, readPipe ) ;
}

bool RF24SendCommon::sendCommonMsg( float temp1, float temp2, float temp3 ) {
  static uint16_t counter=0;

  char outBuffer[32] ; // Clear the outBuffer before every loop

  if( ! _isInit ) {
	  printf("RF24 radio must be initialized before sending messages.\n") ;
	  return false ;
  }
  if ( ++counter > 999 ) counter = 0;
  sprintf(outBuffer, "%2X,%03d,%04u,%04u,%04u",
    _nodeID & 0xff,
    counter,
    (int16_t)(temp1*10),
    (int16_t)(temp2*10),
    (int16_t)(temp3*10)
    );
  printf("outBuffer: %s len: %d ---  ",outBuffer, strlen(outBuffer));

  return _radioEx.sendMsg( outBuffer, _bufLen ) ;
}

void RF24SendCommon::nodeID( int newNodeID ) {
	_nodeID = newNodeID ;
}
int RF24SendCommon::nodeID( ) {
	return _nodeID ;
}

