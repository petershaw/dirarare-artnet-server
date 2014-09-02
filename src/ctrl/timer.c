
#include "timer.h"
#include "../input/inputs.h"
#include "../global.h"


volatile unsigned long time = 0;
volatile unsigned long next_reset = 0;

ISR( TIMER2_OVF_vect ) {
  
	int8_t new, diff;
	new = 0;
	//static uint32_t tmp=0;
	
	/*tmp++;
	if (tmp >=  1000) {
		PORTD ^= (1 << PD6);  //PD6 Toggle
		tmp = 0;
	}
	*/

  if(PINB & 1<<PB1 )
    new = 3;
  if(PINB & 1<<PB2 )
    new ^= 1;                   // convert gray to binary
  diff = last - new;            // difference last - new
  
  if( diff & 1 ){               // bit 0 = value (1)
    last = new;                 // store new as next last
    enc_delta += (diff & 2) - 1;        // bit 1 = direction (+/-)
  }
}


ISR (TIMER1_COMPA_vect){
	//tick 1 second
	time++;
	
	if(next_reset == time)
	{
		ETH_INIT();
		next_reset = time + RESET_TIME;
	}
	
    eth.timer = 1;
//	artnet_tick();
}

 
ISR (TIMER0_OVF_vect) { 
 	static int16_t cnt = 0;
 	static int lastIsActive = 0;
 	++cnt;
 	if(cnt >= 256){
 		if(isActive != lastIsActive){
 			// set information if ArtNet is connected or device is pinged.
	  		ui_menucontroller_defaultoff(isActive);
	  		lastIsActive = isActive;
		}
		isActive = 0;
 		cnt = 0;
 	} 
 }

//----------------------------------------------------------------------------

void timer_init (void)
{
	// Timer 1 - ArtNet
	TCCR1B |= (1<<WGM12) | (1<<CS10 | 0<<CS11 | 1<<CS12);
	TCNT1 = 0;
	OCR1A = (F_CPU / 1024) - 1;
	TIMSK1 |= (1 << OCIE1A);
	next_reset = time + RESET_TIME;

	// Timer 0 - show active or not
	TCCR0B |= (1<<CS02) | (1<<CS00);        // Prescaler 1024
	TIMSK0 |= (1<<TOIE0); //(1<<OCIE0A);
	
	// Timer 2	- check encoder rotation
	TCCR2B |= (1<<CS21) | (1<<CS20); 		//Prescaler 1024
	TIMSK2 |= (1<<TOIE2);
	
	//Disable Timer
	ENCODER_TIMER2_OFF;
	
	return;
};

