
#include <stdio.h>
#include <util/delay.h>
#include "menuController.h"
#include "menu.h"
#include "../output/usart.h"
#include "../input/inputs.h"
#include "../input/map.h"
#include "../../src-lib/lcd/lcd.h"

static char lcd_buf[17];

#define InitAndShowItem_M(v, ce, cs, a)	do {\
											if(ce == 0){\
												v(); \
												ui_menu_run( FUNCTION_FIRST_CALL );\
												ui_menucontroller_print( ui_menu_show() ); \
											} else if(ce == 1){ \
												cs = cs + (a); \
											} \
										} while(0);

void ui_menucontroller_init(void){

}

void ui_menucontroller_defaultoff(int state){
	lcd_clrscr();
	lcd_home();
	lcd_gotoxy(0, 1);
	if(state == 1){
		lcd_puts_p(PSTR("          active"));
	} else {
		lcd_puts_p(PSTR("   not connected"));
	}
}

void ui_menucontroller_print(char *text){
	lcd_home();
	lcd_gotoxy(0, 1);
	sprintf(lcd_buf,  "%-3s %11s ", "#", text);
	lcd_puts(lcd_buf);
	lcd_gotoxy(0, 0);
	sprintf(lcd_buf, "%-16s", currentFunction->currentMessage );
	lcd_puts(lcd_buf);
	lcd_gotoxy(16,1);
}

void ui_menucontroller_info(char *text, int info){
	lcd_home();
	lcd_gotoxy(0, 0);
	sprintf(lcd_buf,  "%s",text);
	lcd_puts(lcd_buf);
	lcd_gotoxy(0, 1);
	sprintf(lcd_buf,  ">%-4i %10s ", info, "info");
	lcd_puts(lcd_buf);
	lcd_gotoxy(16,1);
}

void ui_menucontroller_show(void){
	int active = 1;
	int lastValue = -1;
	int currentValue = 0;
	int cntenter = 0;
	int inEditMode = 0;
	uint8_t cntr_state = 0;
	ui_menu_run( FUNCTION_FIRST_CALL );
	ui_menucontroller_print( ui_menu_show() );
	lcd_gotoxy(15,1);
	while(active){
		inputs_read_next();
		if( cntenter > 0 ){  (*currentFunction->jump_loop)(); }
			
		if( is_inputs_present() == 1 ){
			currentValue = inputs_get_value();
			int menuDirection = 99;
			if(lastValue == INPUT_PIN_8 && currentValue == INPUT_PIN_2){
				menuDirection = 1;
			} else 
			if(lastValue == INPUT_PIN_7 && currentValue == NO_INPUT_PIN){
				menuDirection = 1;
			} else 
			if(lastValue == INPUT_PIN_7 && currentValue == INPUT_PIN_2){
				menuDirection = -1;
			} else 
			if(lastValue == INPUT_PIN_8 && currentValue == NO_INPUT_PIN){
				menuDirection = -1;
			} else 
			if(lastValue == 0){	menuDirection = 0; }
			else {
				lastValue = currentValue;
				continue;
			}
				
			if( inEditMode == 0 ){
				if(menuDirection == 1){
					InitAndShowItem_M(ui_menu_next, cntenter, cntr_state, +1);
					
				} else 
				if(menuDirection == -1){
					InitAndShowItem_M(ui_menu_previous, cntenter, cntr_state, -1);			
				}
			} else {
				inEditMode = (*currentFunction->jump_edit)(menuDirection, &ui_menu_data);
				isEnter(menuDirection){
					ui_menu_run( FUNCTION_IO_ENTER );
				}
				isLeft(menuDirection){
					ui_menu_run( FUNCTION_IO_LEFT );
				}
				isRight(menuDirection){
					ui_menu_run( FUNCTION_IO_RIGHT );
				}
				
				if(inEditMode == 0){
					// edit finished. 
					cntr_state = 0;
					cntenter = 0;
				}
			}
			ui_menu_run( FUNCTION_RE_CALL );
			lcd_gotoxy(0, 0);
			lcd_puts(currentFunction->currentMessage);
			lcd_gotoxy(currentFunction->highlightPosition ,0);
			
			
			if(inEditMode == 0 && (lastValue == 0 || cntenter > 0)){
				if(cntr_state == -1){
					cntr_state = 2;
					ui_menucontroller_print("<exit>");
				} else
				if(cntr_state == 0){
					ui_menucontroller_print("<select>");
				} else if(cntr_state == 1){
					ui_menucontroller_print("<back>");
				} else if(cntr_state == 2){
					ui_menucontroller_print("<exit>");
				} else {
					cntr_state = 0;
					ui_menucontroller_print("<select>");
				}
				if(lastValue == 0){
					++cntenter;
					
					if(cntenter >= 2){
						if(cntr_state == 0){
							ui_menucontroller_print("*edit "); 
							lcd_gotoxy(0, 0);
							inEditMode = 1; 
						} else 
						if(cntr_state == 1){
							ui_menucontroller_print( ui_menu_show() );
							ui_menu_run( FUNCTION_LAST_CALL );
							cntenter = 0;
							cntr_state = 0;
							lcd_gotoxy(15, 1);

						} else 
						if(cntr_state == 2){
							ui_menu_run( FUNCTION_LAST_CALL );
							active = 0;
						}
						cntenter = 0;
					}
				}
			}
			lastValue = currentValue;
		}
	}
	_delay_ms(25);
	inputs_read_next();
	_delay_ms(25);  
}

void ui_menucontroller_hide(void){
	ui_menucontroller_defaultoff(0);
}

