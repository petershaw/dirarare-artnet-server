//
//  Function ip.c
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <util/delay.h>
#include "save.h"
#include "../global.h"
#include "../drv/eeprom.h"
#include "../../src-lib/lcd/lcd.h"

static char lcd_buf[17];

int firnament_edit_save(int direction, void *datafn() ){
	
	// write to driver
	uint8_t written = 0;
	eeprom_open();	
		written  = eeprom_write(&ip);
		written += eeprom_write(&net);
		written += eeprom_write(&gw);
	eeprom_close();
	
	lcd_clrscr();
	lcd_gotoxy(0,0);
	sprintf(lcd_buf,  "wrote %i bytes  ", written);
	lcd_puts(lcd_buf);
	
	_delay_ms(2000);
	
	// reboot
	reboot();
	
	return 0;	
}

void *fn_firnament_save(int init, void *data){
	on_init {
 		strcpy(currentFunction->currentMessage, "sure?");
		currentFunction->jump_edit = &firnament_edit_save;
		currentFunction->jump_loop = &dummy;
 		currentFunction->highlightPosition = 0;
	}
	
	on_exit {
		strcpy(currentFunction->currentMessage, '\0');
		currentFunction->jump_edit = &Int_dummyEditFn_Int;
		currentFunction->jump_loop = dummy;
		currentFunction->highlightPosition = 0;		
	}
	

}

