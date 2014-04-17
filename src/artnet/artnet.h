/*----------------------------------------------------------------------------
 Copyright:      Stefan Krupop  mailto: mail@stefankrupop.de
 Author:         Stefan Krupop, Ulrich Radig
 Remarks:        
 known Problems: none
 Version:        30.12.2013
 Description:    Implementation des ArtNet-Protokolls für DMX-Übertragung über Ethernet

 Dieses Programm ist freie Software. Sie können es unter den Bedingungen der 
 GNU General Public License, wie von der Free Software Foundation veröffentlicht, 
 weitergeben und/oder modifizieren, entweder gemäß Version 2 der Lizenz oder 
 (nach Ihrer Option) jeder späteren Version. 

 Die Veröffentlichung dieses Programms erfolgt in der Hoffnung, 
 daß es Ihnen von Nutzen sein wird, aber OHNE IRGENDEINE GARANTIE, 
 sogar ohne die implizite Garantie der MARKTREIFE oder der VERWENDBARKEIT 
 FÜR EINEN BESTIMMTEN ZWECK. Details finden Sie in der GNU General Public License. 

 Sie sollten eine Kopie der GNU General Public License zusammen mit diesem 
 Programm erhalten haben. 
 Falls nicht, schreiben Sie an die Free Software Foundation, 
 Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA. 
------------------------------------------------------------------------------*/
#include "../config.h"

#ifndef _ARTNETCLIENT_H
	#define _ARTNETCLIENT_H
	
	//Hardware Konfiguration
	#define DMXIOPullupPort		PORTC   //PULLUP DMX IN/OUT
	#define DMXIOPullupPin		4      //PULLUP DMX IN/OUT
	#define DMXIOPinPort		PINC   //DMX IN/OUT
	#define DMXIOPin			4      //DMX IN/OUT

	#define DMXPort				PORTD   //DMX Kommunikations Port
	#define DMXPinPort			PIND   //DMX Kommunikations PIN
	#define DMXDirectionPort	DDRD   //DIR_IO and TXD Output
	#define DMXRXD				0      //RXD Input
	#define DMXTXD				1      //TXD Output
	#define DMXDIR_IO			2      //DIR_IO Output

	#define ARTNET_DEBUG usart_write
	//#define ARTNET_DEBUG(...)

	#include <avr/io.h>
	#include <avr/pgmspace.h>
	#include "../net/stack.h"
	#include "../output/usart.h"

	#define ARTNET_SUBNET_EEPROM_STORE    	60
	#define ARTNET_INUNIVERSE_EEPROM_STORE	61
	#define ARTNET_OUTUNIVERSE_EEPROM_STORE	62
	#define ARTNET_PORT_EEPROM_STORE    	64
	#define ARTNET_NETCONFIG_EEPROM_STORE   66
	#define ARTNET_SHORTNAME_EEPROM_STORE	70
	#define ARTNET_LONGNAME_EEPROM_STORE    88

	#define TRUE					1
	#define FALSE					0

	// ----------------------------------------------------------------------------
	// op-codes
	#define OP_POLL					0x2000
	#define OP_POLLREPLY			0x2100
	#define OP_OUTPUT				0x5000
	#define OP_ADDRESS				0x6000
	#define OP_IPPROG				0xf800
	#define OP_IPPROGREPLY			0xf900

	// ----------------------------------------------------------------------------
	// status
	#define RC_POWER_OK				0x01
	#define RC_PARSE_FAIL			0x04
	#define RC_SH_NAME_OK			0x06
	#define RC_LO_NAME_OK			0x07

	// ----------------------------------------------------------------------------
	// default values
	#define SUBNET_DEFAULT			0
	#define INUNIVERSE_DEFAULT		1
	#define OUTUNIVERSE_DEFAULT		0
	#define PORT_DEFAULT			0x1936
	#define NETCONFIG_DEFAULT		1

	// ----------------------------------------------------------------------------
	// other defines
	#define MAX_NUM_PORTS			4
	#define SHORT_NAME_LENGTH		18
	#define LONG_NAME_LENGTH		64
	#define PORT_NAME_LENGTH		32
	#define MAX_DATA_LENGTH			511

	#define PROTOCOL_VERSION 		14 		// DMX-Hub protocol version.
	#define FIRMWARE_VERSION 		0x0208	// DMX-Hub firmware version.
	#define OEM_ID 					0xB108  // OEM Code (Registered to DMXControl, must be changed in own implementation!)
	#define STYLE_NODE 				0    	// Responder is a Node (DMX <-> Ethernet Device)

	#define PORT_TYPE_DMX_OUTPUT	0x80
	#define PORT_TYPE_DMX_INPUT 	0x40

	#define MAX_CHANNELS 			512
	#define IBG   					10		// interbyte gap [us]

	#define REFRESH_INTERVAL		4		// [s]

	void artnet_init(void);
	void artnet_sendPollReply(void);
	void artnet_main(void);
	void artnet_get(unsigned char);
	void artnet_tick(void);
	
#endif
