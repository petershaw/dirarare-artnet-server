
#include "inputs.h"
#include "../global.h"
#include "../config.h"
#include <avr/io.h>
#include <avr/interrupt.h>


//volatile uint8_t portbhistory = 0xFF;

void initialize_inputs(void){
	next_action = 0;
	
	//ROTARY ENCODER SETUP
	//------------------------------------------------------------------
	
	// PB0,PB1,PB2 as inputs
	/*ENCODER_REGISTER &= ~(1 << ENCODER_PIN_PUSH) | 
						~(1 << ENCODER_PIN_TURN0) |
						~(1 << ENCODER_PIN_TURN1);
    
    //Enable pull-up
    ENCODER_PORT |= (1 << ENCODER_PIN_PUSH) |
					(1 << ENCODER_PIN_TURN0) |
					(1 << ENCODER_PIN_TURN1);

	int8_t new;
 
	new = 0;
	if( PINB & 1<<PB1 )
		new = 3;
	if( PINB & 1<<PB2 )
		new ^= 1;                   // convert gray to binary
	last = new;                   // power on state
	enc_delta = 0;*/
	
    //Debug LED
    DDRD |= (1 << PD6);
    
	DDRB &= ~(1 << DDB0);
    PORTB |= (1 << PORTB0);
    
    PCICR |= (1 << PCIE0);    // set PCIE0 to enable PCMSK0 scan
    PCMSK0 |= (1 << PCINT0);  // set PCINT0 to trigger an interrupt on state change 

    sei();
    
}
	
	//------------------------------------------------------------------
	//END ROTARY ENCODER SETUP
	

/* INTERRUPT
   -----------------------------------------
   Inputs are not listening until should_read
   is true
   -----------------------------------------
*/
ISR (PCINT0_vect)
{
    /* interrupt code here */
    ENCODER_TIMER2_ON;
}


int8_t encode_read1( void )         // read single step encoders
{
  int8_t val;
 
  cli();
  val = enc_delta;
  enc_delta = 0;
  sei();
  return val;                   // counts since last call
}
 
