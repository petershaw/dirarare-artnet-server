
#include "inputs.h"
//#include "map.h"
#include "../config.h"
#include <avr/io.h>
#include <avr/interrupt.h>


//volatile uint8_t portbhistory = 0xFF;

void initialize_inputs(void){
	next_action = 0;
	
	//ROTARY ENCODER SETUP
	//------------------------------------------------------------------
	
	// PB0,PB1,PB2 as inputs
	ENCODER_REGISTER &= ~(1 << ENCODER_PIN_PUSH) | 
						~(1 << ENCODER_PIN_TURN0) |
						~(1 << ENCODER_PIN_TURN1);
    
    // Enable pull-up
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
	enc_delta = 0;
	
	ICR1 = 0x30D;

    TCCR1B |= (1 << WGM12);
    // Mode 4, CTC on OCR1A

    TIMSK1 |= (1 << ICIE1);
    //Set interrupt on compare match

    TCCR1B |= (1 << CS12);
    // set prescaler to 256 and starts the timer

	
	//------------------------------------------------------------------
	//END ROTARY ENCODER SETUP
	
	
}

/* INTERRUPT
   -----------------------------------------
   Inputs are not listening until should_read
   is true
   -----------------------------------------
*/

ISR( TIMER1_COMP_vect ) {             // 1ms for manual movement
  
  int8_t new, diff;
  new = 0;

  if( 1<<PINB0)
	next_action = PRESSED_ENTER;
  if(1<<PINB1 )
    new = 3;
  if(1<<PINB2 )
    new ^= 1;                   // convert gray to binary
  diff = last - new;                // difference last - new
  
  if( diff & 1 ){               // bit 0 = value (1)
    last = new;                 // store new as next last
    enc_delta += (diff & 2) - 1;        // bit 1 = direction (+/-)
  }
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
 
