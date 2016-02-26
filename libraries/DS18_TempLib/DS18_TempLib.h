#ifndef DS18_TempLib_h
#define DS18_TempLib_h

	#if ARDUINO >= 100
	 #include "Arduino.h"
	#else
	 #include "WProgram.h"
	#endif

	float getDS18Temperature(int pin_, bool convertToF) ;

#endif
