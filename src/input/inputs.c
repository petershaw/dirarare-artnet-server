
#include "inputs.h"
#include "map.h"
#include "../config.h"
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t portbhistory = 0xFF;

void initialize_inputs(void){
	next_action = 0;
	
	//ROTARY ENCODER SETUP
	//------------------------------------------------------------------
	
	// PB0,PB1,PB2 as inputs
	ENCODER_REGISTER &= ~(1 << ENCODER_PIN_PUSH) | 
						~( 1 << ENCODER_PIN_TURN0 ) |
						~( 1 << ENCODER_PIN_TURN1 );
    
    // Enable pull-up
    ENCODER_PORT |= (1 << ENCODER_PIN_PUSH) |
					( 1 << ENCODER_PIN_TURN1 ) |
					( 1 << ENCODER_PIN_TURN1 );
	
	// Set PCIE0 to enable PCMSK0 scan
    PCICR |= (1 << PCIE0);
    
    // Set PCINT0 to trigger an interrupt on state change 
    PCMSK0 |= (1 << PCINT0);

	//------------------------------------------------------------------
	//END ROTARY ENCODER SETUP
}






/* INTERRUPT
   -----------------------------------------
   Inputs are not listening until should_read
   is true
   -----------------------------------------
*/

//@todo
//REchts links drehen funktioniert nicht.
ISR(PCINT0_vect) {
	
	 uint8_t changedbits;


    changedbits = PINB ^ portbhistory;
    portbhistory = PINB;

    
    if(changedbits & (1 << PINB0))
    {
        /* PCINT0 changed */
        next_action = PRESSED_ENTER;
    }
	
	   if(changedbits & (1 << PINB1) && (1 << PINB2 ))
    {
        /* PCINT1 changed */
         next_action = PRESSED_ENTER;
    }
    
     else
    {
        /* PCINT2 changed */
         lcd_clrscr();
    }

}
	

 /*ISR (INT0_vect) { 
 	if( INPUT_PRESS_PORT & (1 << INPUT_PRESS_DOWN) ){
 		next_action = PRESSED_ENTER;
 	} else if(INPUT_TURN_PORT & (1 << INPUT_TURN_RIGHT)){
 		next_action = PRESSED_RIGHT;
 	} else if(INPUT_TURN_PORT & (1 << INPUT_TURN_LEFT)){
 		next_action = PRESSED_LEFT;
 	}

 }*/

