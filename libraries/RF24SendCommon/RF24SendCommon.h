#ifndef __RF24SENDCOMMON_H__
#define __RF24SENDCOMMON_H__

#include <RF24Ex.h>
#include <RF24.h>
#include <SPI.h>

#define DEFAULTBUFLEN 21

class RF24SendCommon {
	private:
		RF24Ex _radioEx ;
		int _nodeID ;
		int _bufLen ;
		bool _isInit ;

	public:
		RF24SendCommon(uint8_t _cepin, uint8_t _cspin, int _node, int buflen=DEFAULTBUFLEN );
		void init( uint64_t writePipe=0, uint64_t readPipe=0 ) ;
		bool sendCommonMsg( float t1, float t2=999, float t3=999);
		void nodeID( int newNodeID );
		int nodeID( );

} ;



#endif
