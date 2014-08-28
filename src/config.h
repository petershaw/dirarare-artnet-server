
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
	// *** BUTTONS & ENCODERS - PISO
	// *** ================================================
#if REVISION < 2
	#define PISO_CLOCK_DIRECTION_REGISTER   DDRB
	#define PISO_CLOCK_PORT                 PORTB
	#define PISO_CLOCK_PIN                  PB2

	#define PISO_LOAD_DIRECTION_REGISTER	DDRD
	#define PISO_LOAD_PORT                  PORTD
	#define PISO_LOAD_PIN                   PD6

	#define PISO_OUTPUT_DIRECTION_REGISTER	DDRD
	#define PISO_OUTPUT_PORT                PINB
	#define PISO_OUTPUT_PIN                 PB1 
	#define PISO_OUTPUT_PCIE				PCIE0
#endif 	

	// *** ================================================
	// *** EEPROM
	// *** ================================================
	#define EEPROM_STARTADDR				0x8		// 8
	#define EEPROM_MAX_BYTES				0x3ff	//1023
    
#endif //_CONFIG_H


