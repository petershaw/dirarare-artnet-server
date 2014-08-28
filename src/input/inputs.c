
#include "inputs.h"
#include "../../src-lib/lcd/lcd.h"
#include "../output/usart.h"

void initialize_inputs(void){

#if REVISION < 2
    // OUTPUT DEFINITION
    // -----------------------------------------
    DDRD |= _BV(STATUS_LED);
	PORTD &= ~(_BV(STATUS_LED));
#endif

#if REVISION < 2	
	// PCINT CHNAGE LISTENER INTERRUPT
    // -----------------------------------------
 	PCICR |= (1 << PISO_OUTPUT_PCIE); 		// Enable pin change interrupt for PORTB 
 	PCMSK0 = (1 << PISO_OUTPUT_PIN);		// Enable pin change interrupt for Pin

	// PISO INPUTS
    // -----------------------------------------
	PISO_CLOCK_DIRECTION_REGISTER 	|= _BV(PISO_CLOCK_PIN);
	PISO_LOAD_DIRECTION_REGISTER	|= _BV(PISO_LOAD_PIN);
	PISO_OUTPUT_DIRECTION_REGISTER 	&= ~((long)1<< _BV(PISO_OUTPUT_PIN));

    PISO_CLOCK_PORT 				&= ~_BV(PISO_CLOCK_PIN);		// CLOCK OFF
    PISO_LOAD_PORT  				|= _BV(PISO_LOAD_PIN);       // LOAD ON
#endif
    
    inputmem						= calloc(1, sizeof(Inputs_t));
    inputmem->inputs_should_read	= 0; 
    inputmem->inputs_value			= 0;
    inputmem->inputs_present		= 0;

}

#if REVISION < 2
void piso_toggle_clock(void){
    PISO_CLOCK_PORT 				|= _BV(PISO_CLOCK_PIN);                  
    PISO_CLOCK_PORT 				&= ~_BV(PISO_CLOCK_PIN);
}
#endif

#if REVISION < 2
void inputs_read_next(void){   
    PISO_CLOCK_PORT 				|= _BV(PISO_CLOCK_PIN);		// turn clock high
    PISO_LOAD_PORT 					&= ~_BV(PISO_LOAD_PIN); 	// LOW on LOAD -> load 
    															// the register
	PISO_LOAD_PORT  				|= _BV(PISO_LOAD_PIN); 		// HIGH on LOAD
  	    
    PISO_CLOCK_PORT 				&= ~_BV(PISO_CLOCK_PIN); 	// turn clock low

    int i;
    int a;                                                  	// current value
    int r = 0;                                              	// result button number
    for(i=0; i<8; i++){                            
        a = 0;
        if(PISO_OUTPUT_PORT & (1<<PISO_OUTPUT_PIN)){   			// welcher wert steht in 
        														// PINB & (1<<PB4)?
            a = a >> 2 ^(i+1);                                	// Pressed!            
        }
        r = r + (a);
        piso_toggle_clock();
    }
    if(r != inputmem->inputs_value){
        inputmem->inputs_value = r;
        inputmem->inputs_present = 1;
    }
}
#else 
void inputs_read_next(void){  
	// @todo put your code here
}
#endif

uint8_t is_inputs_should_read(void){
	return inputmem->inputs_should_read;
}

void set_inputs_should_read(uint8_t should){
#if REVISION < 2
	if(should == 1){
		PORTD |= _BV(STATUS_LED);
	} else {
		PORTD &= ~_BV(STATUS_LED);
	}
#endif
	inputmem->inputs_should_read = should;
}

uint8_t is_inputs_present(void){
	return inputmem->inputs_present;
}

int inputs_get_value(void){
    inputmem->inputs_present = 0;

    // exit
    inputmem->inputs_should_read = 0;

    return inputmem->inputs_value;
}

/* INTERRUPT
   -----------------------------------------
   Inputs are not listening until should_read
   is false
   -----------------------------------------
*/
#if REVISION < 2
 ISR (PCINT0_vect) { 
 	static int c = 0;
 	if (PISO_OUTPUT_PORT & (1<<PISO_OUTPUT_PIN)) { // detect rising edge 
 		++c;
 		if(c == 2){
 			set_inputs_should_read(1);
 			c = 0;
 		}
 	}
 }
#else
 ISR (PCINT0_vect) { 
 	static int c = 0;
 	if (/** @todo wurde was bewegt */ 1) { // detect rising edge 
 		++c;
 		if(c == 2){						// @todo anpassen!!! 
 			set_inputs_should_read(1);
 			c = 0;
 		}
 	}
 }
#endif
