
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
	#define ENCODER_REGISTER	DDRB
	#define ENCODER_PORT		PORTB
	#define ENCODER_PIN_PUSH	PB0
	#define ENCODER_PIN_TURN0	PB1		
	#define ENCODER_PIN_TURN1	PB2
	
	
	
	// *** ================================================
	// *** EEPROM
	// *** ================================================
	#define EEPROM_STARTADDR				0x8		// 8
	#define EEPROM_MAX_BYTES				0x3ff	//1023
    
#endif //_CONFIG_H


