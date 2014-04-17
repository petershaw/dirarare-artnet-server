//
//  eeprom driver
//  internal eeprom
//
//

#include <stdio.h>
#include <stdint.h>
#ifndef IgnoreTest
#include <avr/eeprom.h>
#include <string.h>
#endif
#include "../global.h"
#include "../config.h"
#include "eeprom.h"

#include "../output/usart.h"

uint8_t cursor = 0;

drv_open(eeprom){
	if(LOCK == 1){
		return 0;
	}
	LOCK = 1;
	eeprom_busy_wait();
	cursor = EEPROM_STARTADDR;
	return eeprom_is_ready();
}

drv_close(eeprom){
	eeprom_busy_wait();
	LOCK = 0;
	return 1;
}

drv_read(eeprom, char *dest, uint8_t readout){
	if( EEPROM_MAX_BYTES - ( cursor + readout ) < 0 ){
		readout =  EEPROM_MAX_BYTES - ( cursor + readout );
	}
	//debug_uart("readout-max: (%i) \n", readout);
	
	eeprom_read_block((void*)dest ,(const void*)cursor, readout);
	uint8_t read = (sizeof(char) * strlen(dest));
	cursor = cursor + read;
	return read;
}

drv_write(eeprom, char *src) {
	if( EEPROM_MAX_BYTES - ( cursor + (sizeof(char) *  strlen(src)) ) < 0 ){
		return -1;
	}
	//debug_uart("write to (%i) \n", cursor);
	eeprom_write_block((void*) src, (const void*)cursor, (sizeof(char) * strlen(src)));
	uint8_t written = (sizeof(char) * strlen(src));
	cursor = cursor + written;
	//debug_uart("end at to (%i) \n", cursor);
	return written;
}



