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
#include "artnet.h"

#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "../global.h"
#include "../../src-lib/lcd/lcd.h"

// ----------------------------------------------------------------------------
// packet formats
struct artnet_packet_addr {
	unsigned char  ip[4];
	unsigned short port;
};

struct artnet_header {
	unsigned char  id[8];
	unsigned short opcode;
};

struct artnet_poll {
	unsigned char  id[8];
	unsigned short opcode;
	unsigned char  versionH;
	unsigned char  version;
	unsigned char  talkToMe;
	unsigned char  pad;
};

struct artnet_pollreply {
	unsigned char  id[8];
	unsigned short opcode;
	struct artnet_packet_addr addr;
	unsigned char  versionInfoH;
	unsigned char  versionInfo;
	unsigned char  subSwitchH;
	unsigned char  subSwitch;
	unsigned short oem;
	unsigned char  ubeaVersion;
	unsigned char  status;
	unsigned short estaMan;
	char           shortName[SHORT_NAME_LENGTH];
	char           longName[LONG_NAME_LENGTH];
	char           nodeReport[LONG_NAME_LENGTH];
	unsigned char  numPortsH;
	unsigned char  numPorts;
	unsigned char  portTypes[MAX_NUM_PORTS];
	unsigned char  goodInput[MAX_NUM_PORTS];
	unsigned char  goodOutput[MAX_NUM_PORTS];
	unsigned char  swin[MAX_NUM_PORTS];
	unsigned char  swout[MAX_NUM_PORTS];
	unsigned char  swVideo;
	unsigned char  swMacro;
	unsigned char  swRemote;
	unsigned char  spare1;
	unsigned char  spare2;
	unsigned char  spare3;
	unsigned char  style;
	unsigned char  mac[6];
	unsigned char  filler[32];
};

struct artnet_ipprog {
	unsigned char  id[8];
	unsigned short opcode;
	unsigned char  versionH;
	unsigned char  version;
	unsigned char  filler1;
	unsigned char  filler2;
	unsigned char  command;
	unsigned char  filler3;
	unsigned char  progIp[4];
	unsigned char  progSm[4];
	unsigned char  progPort[2];
	unsigned char  spare[8];
};

struct artnet_ipprogreply {
	unsigned char  id[8];
	unsigned short opcode;
	unsigned char  versionH;
	unsigned char  version;
	unsigned char  filler1;
	unsigned char  filler2;
	unsigned char  filler3;
	unsigned char  filler4;
	unsigned char  progIp[4];
	unsigned char  progSm[4];
	unsigned char  progPort[2];
	unsigned char  spare[8];
};

struct artnet_address {
	unsigned char  id[8];
	unsigned short opcode;
	unsigned char  versionH;
	unsigned char  version;
	unsigned char  filler1;
	unsigned char  filler2;
	char           shortName[SHORT_NAME_LENGTH];
	char           longName[LONG_NAME_LENGTH];
	unsigned char  swin[MAX_NUM_PORTS];
	unsigned char  swout[MAX_NUM_PORTS];
	unsigned char  subSwitch;
	unsigned char  swVideo;
	unsigned char  command;
};

struct artnet_dmx {
	unsigned char  id[8];
	unsigned short opcode;
	unsigned char  versionH;
	unsigned char  version;
	unsigned char  sequence;
	unsigned char  physical;
	unsigned short universe;
	unsigned char  lengthHi;
	unsigned char  length;
	unsigned char  dataStart;
};

// ----------------------------------------------------------------------------
//global variables
enum {BREAK, STARTB, DATA, STOPPED};

unsigned char  artnet_subNet = SUBNET_DEFAULT;
unsigned char  artnet_outputUniverse1 = OUTUNIVERSE_DEFAULT;
unsigned char  artnet_inputUniverse1 = INUNIVERSE_DEFAULT;
unsigned char  artnet_sendPollReplyOnChange = FALSE;
unsigned long  artnet_pollReplyTarget = (unsigned long)0xffffffff;
volatile unsigned int   artnet_pollReplyCounter = 0;
unsigned char  artnet_status = RC_POWER_OK;
char           artnet_shortName[18];
char           artnet_longName[64];
unsigned short artnet_port = PORT_DEFAULT;
unsigned char  artnet_netConfig = NETCONFIG_DEFAULT;

volatile unsigned char  artnet_poll = 0;
volatile unsigned char  artnet_dmxUniverse[MAX_CHANNELS];
volatile unsigned short artnet_dmxChannels = 0;
volatile unsigned char  artnet_dmxTransmitting = FALSE;
volatile unsigned char  artnet_dmxInChanged = FALSE;
volatile unsigned char  artnet_dmxInComplete = FALSE;
unsigned char  artnet_dmxDirection = 0;
unsigned char  artnet_dmxRefreshTimer = REFRESH_INTERVAL;
volatile unsigned char  artnet_rs485_safty_counter = 0;


//------------------------------------------------------------------------------
//Read all IP-Datas
void read_ip_addresses (void) {
   
	(*((unsigned long*)&myip[0]))      = MYIP;
	(*((unsigned long*)&netmask[0]))   = NETMASK;
	(*((unsigned long*)&router_ip[0])) = ROUTER_IP;
	//Broadcast-Adresse berechnen
	(*((unsigned long*)&broadcast_ip[0])) = (((*((unsigned long*)&myip[0])) & (*((unsigned long*)&netmask[0]))) | (~(*((unsigned long*)&netmask[0]))));
}

// ----------------------------------------------------------------------------
// initialization of network settings
void artnet_netInit(void) {

	if (artnet_netConfig == 1) {
		if (*((unsigned long*)&myip[0]) == IP(127,127,127,127)) {
			#if USE_DHCP
				//ARTNET_DEBUG("Setting network address: Custom (DHCP)\r\n");
				dhcp_init();
				if (dhcp() != 0) {
					//ARTNET_DEBUG("DHCP fail\r\n");
					//use programmed value
					(*((unsigned long*)&myip[0])) = MYIP;
					(*((unsigned long*)&netmask[0])) = NETMASK;
				}
			#else
				//ARTNET_DEBUG("Setting network address: Custom\r\n");
				(*((unsigned long*)&myip[0])) = MYIP;
				(*((unsigned long*)&netmask[0])) = NETMASK;
			#endif //USE_DHCP
		} else {
			read_ip_addresses();
		}
	} 
	// calculate broadcast adress
	(*((unsigned long*)&broadcast_ip[0])) = (((*((unsigned long*)&myip[0])) & (*((unsigned long*)&netmask[0]))) | (~(*((unsigned long*)&netmask[0]))));

	// remove any existing app from port
	kill_udp_app(artnet_port);
	// add port to stack with callback
	add_udp_app(artnet_port, (void(*)(unsigned char))artnet_get);
}

// ----------------------------------------------------------------------------
// initialization of Art-Net
void artnet_init(void) {
	 // set ports for USART, port direction and dip-switches
	 DMXDirectionPort  |= (1<<DMXTXD)|(1<<DMXDIR_IO); // DIR_IO and TXD Output
	
	 //PULLUP DMX IN/OUT
	 DMXIOPullupPort |= (1<<DMXIOPullupPin);

	 // read Art-Net port
	artnet_port = PORT_DEFAULT;

	// read netconfig
	artnet_netConfig = NETCONFIG_DEFAULT;

	// read subnet
	artnet_subNet = SUBNET_DEFAULT;

	artnet_inputUniverse1 = INUNIVERSE_DEFAULT;

	// read nr. of output universe
	artnet_outputUniverse1 = OUTUNIVERSE_DEFAULT;

	// read short name
	strcpy_P(artnet_shortName, PSTR("AvrArtNode\0"));
	artnet_shortName[SHORT_NAME_LENGTH - 1] = 0;

	// read long name
	strcpy_P(artnet_longName, PSTR("AVR based Art-Net node\0"));
	artnet_longName[LONG_NAME_LENGTH - 1] = 0;

	artnet_netInit();

	// annouce that we are here 
	artnet_sendPollReply();

	// enable PollReply on changes
	artnet_sendPollReplyOnChange = TRUE;

	return;
}

// ----------------------------------------------------------------------------
// send an ArtPollReply packet
void artnet_sendPollReply(void) {
	struct artnet_pollreply *msg;
	char buffer[LONG_NAME_LENGTH];

	// clear packet buffer
	for (unsigned int i = 0; i < sizeof(struct artnet_pollreply); i++) { //clear eth_buffer to 0
		eth_buffer[UDP_DATA_START + i] = 0;
	}

	msg = (struct artnet_pollreply *)&eth_buffer[UDP_DATA_START];
	
	strcpy_P((char*)msg->id,PSTR("Art-Net\0"));
	msg->opcode = OP_POLLREPLY;

	msg->addr.ip[0] = myip[0];
	msg->addr.ip[1] = myip[1];
	msg->addr.ip[2] = myip[2];
	msg->addr.ip[3] = myip[3];
	msg->addr.port = artnet_port;
	msg->versionInfoH = (FIRMWARE_VERSION >> 8) & 0xFF;
	msg->versionInfo = FIRMWARE_VERSION & 0xFF;

	msg->subSwitchH = 0;
	msg->subSwitch = artnet_subNet & 15;

	msg->oem = OEM_ID;
	msg->ubeaVersion = 0;
	msg->status = 0;
	msg->estaMan = 'S' * 256 + 'K';
	strcpy(msg->shortName, artnet_shortName);
	strcpy(msg->longName, artnet_longName);
	sprintf(buffer, "#%04X [%04u] AvrArtNode is ready", artnet_status, artnet_pollReplyCounter);

	strcpy(msg->nodeReport, buffer);

	msg->numPortsH = 0;
	msg->numPorts = 1;

	if (artnet_dmxDirection == 1) {
		msg->portTypes[0] = PORT_TYPE_DMX_INPUT;
	} else {
		msg->portTypes[0] = PORT_TYPE_DMX_OUTPUT;
	}

	if (artnet_dmxDirection != 1) {
		msg->goodInput[0] = (1 << 3);
	} else {
		if (artnet_dmxChannels > 0) {
			msg->goodInput[0] |= (1 << 7);
		}
	}

	msg->goodOutput[0] = (1 << 1);
	if (artnet_dmxTransmitting == TRUE) {
		msg->goodOutput[0] |= (1 << 7);
	}

	msg->swin[0] = (artnet_subNet & 15) * 16 | (artnet_inputUniverse1 & 15);
	msg->swout[0] = (artnet_subNet & 15) * 16 | (artnet_outputUniverse1 & 15);

	msg->style = STYLE_NODE;

	msg->mac[0] = MYMAC1;
	msg->mac[1] = MYMAC2;
	msg->mac[2] = MYMAC3;
	msg->mac[3] = MYMAC4;
	msg->mac[4] = MYMAC5;
	msg->mac[5] = MYMAC6;

	create_new_udp_packet(sizeof(struct artnet_pollreply)+20, artnet_port, artnet_port, artnet_pollReplyTarget);
}

// ----------------------------------------------------------------------------
// send an ArtIpProgReply packet
void artnet_sendIpProgReply(unsigned long target) {
	struct artnet_ipprogreply *msg;

	// clear packet buffer
	for (unsigned int i = 0; i < sizeof(struct artnet_ipprogreply); i++) { //clear eth_buffer to 0
		eth_buffer[UDP_DATA_START + i] = 0;
	}

	msg = (struct artnet_ipprogreply *)&eth_buffer[UDP_DATA_START];
	strcpy_P((char*)msg->id,PSTR("Art-Net\0"));
	msg->opcode = OP_IPPROGREPLY;

	msg->versionH = 0;
	msg->version = PROTOCOL_VERSION;

	for (unsigned char i = 0; i < 4; i++) {
		msg->progIp[i] = myip[i];
		msg->progSm[i] = netmask[i];
	}
	msg->progPort[0] = (artnet_port >> 8) & 0xff;
	msg->progPort[1] = artnet_port & 0xff;

	create_new_udp_packet(sizeof(struct artnet_ipprogreply), artnet_port, artnet_port, target);
}

// ----------------------------------------------------------------------------
// send an ArtDmx packet
void artnet_sendDmxPacket(void) {
	static unsigned char sequence = 1;
	struct artnet_dmx *msg;

	// clear packet buffer
	for (unsigned int i = 0; i < sizeof(struct artnet_dmx) + artnet_dmxChannels; i++) { //clear eth_buffer to 0
		eth_buffer[UDP_DATA_START + i] = 0;
	}

	msg = (struct artnet_dmx *)&eth_buffer[UDP_DATA_START];
	strcpy_P((char*)msg->id,PSTR("Art-Net\0"));
	msg->opcode = OP_OUTPUT;

	msg->versionH = 0;
	msg->version = PROTOCOL_VERSION;

	msg->sequence = sequence++;
	if (sequence == 0) {
		sequence = 1;
	}

	msg->physical = 1;
	msg->universe = ((artnet_subNet << 4) | artnet_inputUniverse1);

	msg->lengthHi = (artnet_dmxChannels >> 8) & 0xFF;
	msg->length = artnet_dmxChannels & 0xFF;

	memcpy(&(msg->dataStart), (unsigned char *)&artnet_dmxUniverse[0], artnet_dmxChannels);

	create_new_udp_packet(sizeof(struct artnet_dmx) + artnet_dmxChannels, artnet_port, artnet_port, (unsigned long)0xffffffff);
}

// ----------------------------------------------------------------------------
// process an ArtPoll packet
void processPollPacket(struct artnet_poll *poll) {
	if ((poll->talkToMe & 2) == 2) {
		artnet_sendPollReplyOnChange = TRUE;
	} else {
		artnet_sendPollReplyOnChange = FALSE;
	}

	if ((poll->talkToMe & 1) == 1) {
		struct IP_Header *ip;
		ip = (struct IP_Header *)&eth_buffer[IP_OFFSET];
		artnet_pollReplyTarget = ip->IP_Srcaddr;
	} else {
		artnet_pollReplyTarget = (unsigned long)0xffffffff;
	}

	artnet_sendPollReply();
}

// ----------------------------------------------------------------------------
// process an ArtAddress packet
// void processAddressPacket(struct artnet_address *address) {
// 	unsigned char changed = 0;
// 
// 	if (address->shortName[0] != 0) {
// 		// set short name
// 		strcpy(artnet_shortName, address->shortName);
// 		eeprom_write_block(&artnet_shortName, (unsigned char *)ARTNET_SHORTNAME_EEPROM_STORE, SHORT_NAME_LENGTH);
// 		artnet_status = RC_SH_NAME_OK;
// 		changed = 1;
// 	}
// 
// 	if (address->longName[0] != 0) {
// 		// set long name
// 		strcpy(artnet_longName, address->longName);
// 		eeprom_write_block(&artnet_longName, (unsigned char *)ARTNET_LONGNAME_EEPROM_STORE, LONG_NAME_LENGTH);
// 		artnet_status = RC_LO_NAME_OK;
// 		changed = 1;
// 	}
// 
// 	if (address->swin[0] == 0) {
// 		// reset input universe nr.
// 		artnet_inputUniverse1 = INUNIVERSE_DEFAULT;
// 		eeprom_busy_wait ();
// 		eeprom_write_byte((unsigned char *)ARTNET_INUNIVERSE_EEPROM_STORE, artnet_inputUniverse1);
// 		changed = 1;
// 	} else if ((address->swin[0] & 128) == 128) {
// 		// set input universe nr.
// 		artnet_inputUniverse1 = address->swin[0] & 0xF;
// 		eeprom_busy_wait ();
// 		eeprom_write_byte((unsigned char *)ARTNET_INUNIVERSE_EEPROM_STORE, artnet_inputUniverse1);
// 		changed = 1;
// 	}
// 
// 	if (address->swout[0] == 0) {
// 		// reset output universe nr.
// 		artnet_outputUniverse1 = OUTUNIVERSE_DEFAULT;
// 		eeprom_busy_wait ();
// 		eeprom_write_byte((unsigned char *)ARTNET_OUTUNIVERSE_EEPROM_STORE, artnet_outputUniverse1);
// 		changed = 1;
// 	} else if ((address->swout[0] & 128) == 128) {
// 		// set output universe nr.
// 		artnet_outputUniverse1 = address->swout[0] & 0xF;
// 		eeprom_busy_wait ();
// 		eeprom_write_byte((unsigned char *)ARTNET_OUTUNIVERSE_EEPROM_STORE, artnet_outputUniverse1);
// 		changed = 1;
// 	}
// 
// 	if (address->subSwitch == 0) {
// 		// reset subnet
// 		artnet_subNet = SUBNET_DEFAULT;
// 		eeprom_busy_wait ();
// 		eeprom_write_byte((unsigned char *)ARTNET_SUBNET_EEPROM_STORE, artnet_subNet);
// 		changed = 1;
// 	} else if ((address->subSwitch & 128) == 128) {
// 		// set subnet
// 		artnet_subNet = address->subSwitch & 0xF;
// 		eeprom_busy_wait ();
// 		eeprom_write_byte((unsigned char *)ARTNET_SUBNET_EEPROM_STORE, artnet_subNet);
// 		changed = 1;
// 	}
// 
//  	// send PollReply when something changed
//  	if (changed == 1 && artnet_sendPollReplyOnChange == TRUE) {
// 		artnet_pollReplyCounter++;
// 		artnet_sendPollReply();
//  	}
// }

// ----------------------------------------------------------------------------
// process an ArtIpProg packet
// void processIpProgPacket(struct artnet_ipprog *ipprog) {
// 
// 	if ((ipprog->command & 128) == 128) {	// enable programming
// 		// program port
// 		if ((ipprog->command & 1) == 1) {
// 			kill_udp_app(artnet_port);
// 			artnet_port = (ipprog->progPort[0] << 8) | ipprog->progPort[1];
// 			//ARTNET_DEBUG("IPPROG: New port: %x (%x, %x)\r\n", artnet_port, ipprog->progPort[0], ipprog->progPort[1]);
// 			eeprom_write_block(&artnet_port, (unsigned char *)ARTNET_PORT_EEPROM_STORE, 2);
// 			add_udp_app(artnet_port, (void(*)(unsigned char))artnet_get);
// 		}
// 
// 		// program subnet
// 		if ((ipprog->command & 2) == 2) {
// 			(*((unsigned long*)&netmask[0])) = (*((unsigned long*)&ipprog->progSm[0]));
// 			for (unsigned char count = 0; count<4; count++) {
// 				eeprom_busy_wait ();
// 				eeprom_write_byte((unsigned char *)(NETMASK_EEPROM_STORE + count),netmask[count]);
// 			}
// 			artnet_netConfig = 1;
// 		}
// 
// 		// program ip
// 		if ((ipprog->command & 4) == 4) {
// 			(*((unsigned long*)&myip[0])) = (*((unsigned long*)&ipprog->progIp[0]));
// 			for (unsigned char count = 0; count<4; count++) {
// 				eeprom_busy_wait ();
// 				eeprom_write_byte((unsigned char *)(IP_EEPROM_STORE + count),myip[count]);
// 			}
// 			artnet_netConfig = 1;
// 		}
// 
// 		// reset to default
// 		if ((ipprog->command & 8) == 8) {
// 			kill_udp_app(artnet_port);
// 			artnet_port = PORT_DEFAULT;
// 			eeprom_write_block(&artnet_port, (unsigned char *)ARTNET_PORT_EEPROM_STORE, 2);
// 			add_udp_app(artnet_port, (void(*)(unsigned char))artnet_get);
// 			artnet_netConfig = 0;
// 		}
// 		eeprom_busy_wait ();
// 		eeprom_write_byte((unsigned char *)ARTNET_NETCONFIG_EEPROM_STORE, artnet_netConfig);
// 	}
// 
// 	struct IP_Header *ip;
// 	ip = (struct IP_Header *)&eth_buffer[IP_OFFSET];
// 	artnet_netInit();
// 	artnet_sendIpProgReply(ip->IP_Srcaddr);
// }

// ----------------------------------------------------------------------------
void artnet_main(void) {
	if (artnet_dmxInComplete == TRUE) {
		if (artnet_dmxInChanged == TRUE) {
			artnet_sendDmxPacket();
			artnet_dmxInChanged = FALSE;
		}
		artnet_dmxInComplete = FALSE;
		artnet_dmxChannels = 0;
	}
	if(artnet_poll == 1)
	{
		artnet_poll = 0;
		artnet_sendPollReply();
	}	
	
	if (DMXIOPinPort & (1 << DMXIOPin))
	{
		artnet_rs485_safty_counter++;
		if(artnet_rs485_safty_counter > 250)
		{
			artnet_dmxChannels = 511;
			// setup USART
			PORTD |= (1 << 2);
			PORTD |= (1 << 1);
			UBRR0   = (F_CPU / (250000* 16L) - 1);
			UCSR0C|= (1<<USBS0); //USBS0 2 Stop bits
			UCSR0B|= (1<<TXEN0)|(1<<TXCIE0); // TXEN0 Transmitter enable / TXCIE0 TX complete interrupt enable
			UDR0    = 0;//start transmitting
			artnet_rs485_safty_counter = 0;
		}
	}
}

// ----------------------------------------------------------------------------
// receive Art-Net packet
void artnet_get(unsigned char index) {
	struct artnet_header *header;

	header = (struct artnet_header *)&eth_buffer[UDP_DATA_START];
	//check the id
	if(strcasestr_P((char*)&header->id,PSTR("Art-Net")) == 0){
		//ARTNET_DEBUG("Wrong ArtNet header, discarded\r\n");
		artnet_status = RC_PARSE_FAIL;
		return;
	}

	switch(header->opcode)
	{
		//OP_POLL
		case (OP_POLL):{
			struct artnet_poll *poll;
			//ARTNET_DEBUG("Received artnet poll packet!\r\n");
			poll = (struct artnet_poll *)&eth_buffer[UDP_DATA_START];
			processPollPacket(poll);
			return;
		}
		//OP_POLLREPLY
		case (OP_POLLREPLY):{
			//ARTNET_DEBUG("Received artnet poll reply packet!\r\n");
			return;
		}
		//OP_OUTPUT
		case (OP_OUTPUT):{
			struct artnet_dmx *dmx;

			//ARTNET_DEBUG("Received artnet output packet!\r\n");
			dmx = (struct artnet_dmx *)&eth_buffer[UDP_DATA_START];

			if (dmx->universe == ((artnet_subNet << 4) | artnet_outputUniverse1)) 
			{
				if (artnet_dmxDirection == 0)
				{
					artnet_dmxChannels = (dmx->lengthHi << 8) | dmx->length;
					memcpy((unsigned char*)&artnet_dmxUniverse[0], &(dmx->dataStart), artnet_dmxChannels);
					
					if (artnet_dmxTransmitting == FALSE)
						{
						// setup USART
						DMXPort |= (1 << DMXDIR_IO);
						DMXPort |= (1 << DMXTXD);
						UBRR0   = (F_CPU / (250000* 16L) - 1);
						UCSR0C|= (1<<USBS0); //USBS0 2 Stop bits
						UCSR0B|= (1<<TXEN0)|(1<<TXCIE0); // TXEN0 Transmitter enable / TXCIE0 TX complete interrupt enable
						UDR0    = 0;//start transmitting

						artnet_dmxTransmitting = TRUE;
						if (artnet_sendPollReplyOnChange == TRUE) {
							artnet_pollReplyCounter++;
							artnet_sendPollReply();
						}
					}
					isActive = 1;

				}
			}
			return;
		}
		//OP_ADDRESS
		case (OP_ADDRESS):{
			struct artnet_address *address;
			//ARTNET_DEBUG("Received artnet address packet!\r\n");
			address = (struct artnet_address *)&eth_buffer[UDP_DATA_START];
			//processAddressPacket(address);
			return;
		}
		//OP_IPPROG
		case (OP_IPPROG):{
			struct artnet_ipprog *ipprog;
			//ARTNET_DEBUG("Received artnet ip prog packet!\r\n");
			ipprog = (struct artnet_ipprog *)&eth_buffer[UDP_DATA_START];
			//processIpProgPacket(ipprog);
			return;		
		}	
	}
}

// ----------------------------------------------------------------------------
// Called by timer, check changes
void artnet_tick(void) {
	unsigned char changed = 0;

	// set DMX direction
	if (!(DMXIOPinPort & (1 << DMXIOPin))) {
		if (artnet_dmxDirection != 1) {
			artnet_dmxDirection = 1;
			artnet_dmxTransmitting = FALSE;

			// setup USART
			DMXPort &= ~(1 << DMXDIR_IO);
			UBRR0   = (F_CPU / (250000* 16L) - 1);
			UCSR0C|=(1<<USBS0); //USBS0 2 Stop bits
			UCSR0B|=(1<<RXEN0)|(1<<RXCIE0);

			changed = 1;
		}
	} else {
		if (artnet_dmxDirection != 0) {
			artnet_dmxDirection = 0;
			changed = 1;
		}
	}

	// send PollReply when something changed
	if (changed == 1 && artnet_sendPollReplyOnChange == TRUE) {
		artnet_pollReplyCounter++;
		artnet_poll = 1;
	}

	if (artnet_dmxDirection == 1) {
		if (artnet_dmxRefreshTimer > 0) {
			artnet_dmxRefreshTimer--;
		} else {
			if (artnet_dmxChannels > 0) {
			//ARTNET_DEBUG("Refreshing DMX packet\r\n");
			artnet_dmxInChanged = TRUE;
			//artnet_sendDmxPacket(); //Nicht im Interrupt!!! Ulrich
			}
			artnet_dmxRefreshTimer = REFRESH_INTERVAL - 1;
		}
	}
}

// ----------------------------------------------------------------------------
// DMX transmission
ISR (USART_TX_vect) {
	static unsigned char  dmxState = BREAK;
	static unsigned short curDmxCh = 0;
	artnet_rs485_safty_counter = 0;
	switch(dmxState)
	{
		case(STOPPED):{
			if (artnet_dmxTransmitting == TRUE) {
				dmxState = BREAK;
			}
			return;
		}
		case(BREAK):{
			UBRR0 = (F_CPU / (50000 * 16L) - 1);
			UDR      = 0;								//send break
			dmxState = STARTB;
			return;
		}
		case(STARTB):{
			UBRR0 = (F_CPU / (250000 * 16L) - 1);
			UDR      = 0;								//send start byte
			dmxState = DATA;
			curDmxCh = 0;
			return;
		}
	}
	_delay_us(IBG);
	UDR      = artnet_dmxUniverse[curDmxCh++];			//send data
	if (curDmxCh == artnet_dmxChannels){
		if (artnet_dmxTransmitting == TRUE){
			dmxState = BREAK; 						//new break if all ch sent
		} else {
			dmxState = STOPPED;
		}
	}
}

// ----------------------------------------------------------------------------
// DMX reception
ISR (USART_RX_vect) {
	static unsigned char  dmxState = 0;
	static unsigned short dmxFrame = 0;
	unsigned char status = UCSR0A; 	// status register must be read prior to UDR (because of 2 byte fifo buffer)
	unsigned char byte = UDR0; 		// immediately catch data from i/o register to enable reception of the next byte

	if ((byte == 0) && (status & (1<<FE0))) {				// BREAK detected (Framing Error)
		dmxState = 1;
		dmxFrame = 0;
		if (artnet_dmxChannels > 0) {
			artnet_dmxInComplete = TRUE;
		}
	} else if (dmxFrame == 0) {							// Start code test
		if ((byte == 0) && (dmxState == 1)) {					// valid SC detected
			dmxState = 2;
		}
		dmxFrame = 1;
	} else {
		if ((dmxState == 2) && (dmxFrame <= MAX_CHANNELS)) {	// addressed to us
			if (artnet_dmxUniverse[dmxFrame - 1] != byte) {
				artnet_dmxUniverse[dmxFrame - 1] = byte;
				artnet_dmxInChanged = TRUE;
			}
			if (dmxFrame > artnet_dmxChannels) {
				artnet_dmxChannels = dmxFrame;
			}
		}
		dmxFrame++;
	}
}