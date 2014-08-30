//
//  main.c
//  Dirarare-ArtNet-Server
//  Revision 2.0
//	

#include <stdlib.h>
#include <stdbool.h>

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "config.h"
#include "global.h"
#include "output/usart.h"
#include "../src-lib/lcd/lcd.h"
#include "drv/eeprom.h"
#include "input/inputs.h"
#include "functions/abstractFunction.h"
#include "functions/ip.h"
#include "functions/save.h"
#include "ui/menu.h"
#include "ui/menuController.h"
#include "net/stack.h"
#include "net/udp_cmd.h"
#include "artnet/artnet.h"
#include "ctrl/timer.h"
	
static char lcd_buf[16];

//@todo: ETH_CS auf Port PD7 legen!



int main(void) {	

	int32_t val = 0;
	int32_t lastval = 0;
	
	isActive = 0;
    // BOOT DISPLAY
    // -----------------------------------------
 	lcd_init(LCD_DISP_ON_CURSOR);
	lcd_clrscr();
	lcd_home();
	lcd_puts_p(PSTR("Dirarare.\nbooting..."));
	
	// READ EEPROM
    // -----------------------------------------
	eeprom_open();
	int r = eeprom_read(&ip, 	15 );
	r += 	eeprom_read(&net, 	15 );
	r += 	eeprom_read(&gw, 	15 );
	eeprom_close();
	
	// check if first position is a number // otherwise reset!
	if( ip[0]  < 48 ||  ip[0] > 57){   ip[0] = '\0'; }
	if(net[0]  < 48 || net[0] > 57){  net[0] = '\0'; }
	if( gw[0]  < 48 ||  gw[0] > 57){   gw[0] = '\0'; }
		

	// CONFIG INPUTS & MENU
    // -----------------------------------------
	initialize_inputs();
	sei(); 
	fn_init();
	ui_menu_init();
	ui_menucontroller_init();
	
	ui_menu_add("IP",		fn_firnament_ip, 	&ip );
 	ui_menu_add("Netmask",	fn_firnament_ip, 	&net);
 	ui_menu_add("Gateway",	fn_firnament_ip, 	&gw );
	ui_menu_add("Save",		fn_firnament_save, 	NULL);
	
	//------------------------------------------------------------------
	//END CONFIG INPUTS & MENU


    // STARTUP APPLICATION
    // -----------------------------------------
 	/*usart_init(BAUDRATE);
 	stack_init();
 	udp_cmd_init();
 	artnet_init();
	timer_init();
	
	//Ethernetcard Interrupt enable
	ETH_INT_ENABLE;*/
	
	//------------------------------------------------------------------
	//END STARTUP APPLICATION

    // STARTUP MESSAGES
    //------------------------------------------------------------------
	lcd_clrscr();
	lcd_home();
	lcd_puts_p(PSTR("Dirarare Rev.2\n"));	// welcome message
	lcd_puts(BUILD_DATE_SHORT);
 
	_delay_ms(1000);	

	lcd_clrscr();
	lcd_gotoxy(0,0);
	lcd_puts_p(PSTR("IP:\n"));
    lcd_puts(ip);
	_delay_ms(1000);	

	lcd_clrscr();	
	lcd_gotoxy(0,0);
	lcd_puts_p(PSTR("Netmask:\n"));
    lcd_puts(net);
	_delay_ms(1000);	
	
	lcd_clrscr();	
	lcd_gotoxy(0,0);
	lcd_puts_p(PSTR("Gateway:\n"));
    lcd_puts(gw);
	_delay_ms(1000);	

    
	lcd_clrscr();
	lcd_home();
	lcd_puts(ip);
	lcd_puts_p(PSTR("\n"));
	
	ui_menucontroller_defaultoff(0);
	
	//------------------------------------------------------------------
	//END STARTUP MESSAGES
	

    // ENABLE GLOBAL INTERRUPTS
    //------------------------------------------------------------------
    sei(); 
	// END GLOBLA INTERRUPTS


    // MAINLOOP
	//------------------------------------------------------------------
	while(1){

		val += encode_read1();
		
		if (val > lastval) {
			//turned right
			lastval = val;
			//Do something
		}
		else {
			//turned right
			lastval = val;
			//Do something
		}
		
		if(next_action != 0){
			ui_menucontroller_show(); 			
			next_action = 0;
			
 			_delay_ms(300);
 			ui_menucontroller_hide();
 		}	
 		
 		
		eth_get_data();
		artnet_main(); 
	}
	
	//------------------------------------------------------------------
	//END MAINLOOP
	
}


