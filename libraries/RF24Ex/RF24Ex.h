#ifndef __RF24EX_H__
#define __RF24EX_H__

#include <RF24.h>
#include <SPI.h>

#define DEFAULT_WRITE_PIPE 0xF0F0F0F0D1LL
#define DEFAULT_READ_PIPE  0xF0F0F0F0E2LL

class RF24Ex {
	private:
		RF24 _radio ;

	public:
		RF24Ex(uint8_t _cepin, uint8_t _cspin) ;
		void initRadio( int payloadSize,
			uint64_t writePipe=DEFAULT_WRITE_PIPE,
			uint64_t readPipe=DEFAULT_READ_PIPE,
			bool showDetails=true ) ;
		bool sendMsg( const void* buf, uint8_t len, const bool multicast=false );
} ;



#endif
