
#ifndef _CONFIG_H_
	#define _CONFIG_H_	

	// *** ================================================
	// *** NETWORK
	// *** ================================================
	//Umrechnung von IP zu unsigned long
	#define IP(a,b,c,d) ((unsigned long)(d)<<24)+((unsigned long)(c)<<16)+((unsigned long)(b)<<8)+a

		
	//MAC Adresse des ART NET NODES	
	#define MYMAC1	0x00
	#define MYMAC2	0x22
	#define MYMAC3	0xF9
	#define MYMAC4	0x01	
	#define MYMAC5	0x1D
	#define MYMAC6	0xA6


	// *** ================================================
	// *** DMX
	// *** ================================================
	//Baudrate der seriellen Schnittstelle
	#define BAUDRATE 57600


	// *** ================================================
	// *** BUTTONS & ENCODERS
	// *** ================================================
	#define INPUT_PRESS_REGISTER			DDRD
	#define INPUT_PRESS_PORT				PORTD
	#define INPUT_PRESS_DOWN				PD6
	#define INPUT_TURN_REGISTER				DDRB
	#define INPUT_TURN_PORT					PORTB
	#define INPUT_TURN_RIGHT				PB1
	#define INPUT_TURN_LEFT					PB2
	
	
	// *** ================================================
	// *** EEPROM
	// *** ================================================
	#define EEPROM_STARTADDR				0x8		// 8
	#define EEPROM_MAX_BYTES				0x3ff	//1023
    
#endif //_CONFIG_H


