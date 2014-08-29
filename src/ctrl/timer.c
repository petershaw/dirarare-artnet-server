
#include "timer.h"
#include "../global.h"


volatile unsigned long time = 0;
volatile unsigned long next_reset = 0;


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

	return;
};

