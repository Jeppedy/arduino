#include <RF24Ex.h>

#include <RF24.h>
#include <SPI.h>


RF24Ex::RF24Ex(uint8_t _cepin, uint8_t _cspin) :
	_radio(_cepin, _cspin)
{
}

void RF24Ex::initRadio( int payloadSize, uint64_t writePipe, uint64_t readPipe, bool showDetails ) {
  _radio.begin();
  _radio.setDataRate(RF24_250KBPS);
  _radio.setPALevel(RF24_PA_MAX);
  _radio.setChannel(76);
  _radio.setRetries(15,15);
  _radio.setCRCLength( RF24_CRC_16 ) ;
  _radio.setAutoAck( 0 ) ;
  _radio.setPayloadSize(payloadSize) ;

  _radio.openWritingPipe(writePipe);
  _radio.openReadingPipe(1,readPipe);

  if( showDetails )
    _radio.printDetails();
  _radio.stopListening();

  return ;
}

bool RF24Ex::sendMsg( const void* buf, uint8_t len, const bool multicast ) {
  _radio.powerUp() ;
    _radio.write( buf, len, multicast ) ;
  _radio.powerDown() ;

}

