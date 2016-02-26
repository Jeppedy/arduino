/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * @file printfEx.h
 *
 * Setup necessary to direct stdout to the Arduino Serial library, which
 * enables 'printf'
 */

#ifndef __PRINTFEX_H__
#define __PRINTFEX_H__

#ifdef ARDUINO

	void printfEx_begin(void) ;
	void printFloatEx(float value, int places) ;


#else
#error This example is only for use on Arduino.
#endif // ARDUINO

#endif // __PRINTF_H__
