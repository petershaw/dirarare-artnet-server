
#include "inputs.h"
#include "map.h"
#include "../../src-lib/lcd/lcd.h"
#include "../output/usart.h"

void initialize_inputs(void){
	next_action = 0;
}

/* INTERRUPT
   -----------------------------------------
   Inputs are not listening until should_read
   is true
   -----------------------------------------
*/
 ISR (PCINT0_vect) { 
 	if( INPUT_PRESS_PORT & (1 << INPUT_PRESS) ){
 		next_action = PRESSED_ENTER;
 	} else if(INPUT_TURN_PORT & (1 << INPUT_TURN_RIGHT)){
 		next_action = PRESSED_RIGHT;
 	} else if(INPUT_TURN_PORT & (1 << INPUT_TURN_LEFT)){
 		next_action = PRESSED_LEFT;
 	}

 }

