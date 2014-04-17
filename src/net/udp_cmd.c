/*----------------------------------------------------------------------------
 Copyright:      Radig Ulrich  mailto: mail@ulrichradig.de
 Author:         Radig Ulrich
 Remarks:        
 known Problems: none
 Version:        09.01.2012
 Description:    empfangene UDP Daten auf Port 7600

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
#include "udp_cmd.h"
#include "stack.h"
#include "../global.h"

// #include "usart.h"

unsigned long  udp_cmd_pollReplyTarget = (unsigned long)0xffffffff;
	
//----------------------------------------------------------------------------
//Initialisierung des NTP Ports (für Daten empfang)
void udp_cmd_init (void)
{
	kill_udp_app(UDP_CMD_PORT_RX);
	
	//Port in Anwendungstabelle eintragen für eingehende UDP Daten!
	add_udp_app (UDP_CMD_PORT_RX, (void(*)(unsigned char))udp_cmd_get);
	add_udp_app (UDP_BOOTLOADER_RESET, (void(*)(unsigned char))udp_bootloader_reset);
	return;
}

//----------------------------------------------------------------------------
//Empfang der IP
void udp_cmd_get (unsigned char index)
{
	//UDP_CMD_DEBUG("** CMD DATA GET Bytes: %i **\r\n",((UDP_DATA_END_VAR)-(UDP_DATA_START)));

	struct IP_Header *ip;
	ip = (struct IP_Header *)&eth_buffer[IP_OFFSET];
	udp_cmd_pollReplyTarget = ip->IP_Srcaddr;
	
	if(strcasestr_P((char*)&eth_buffer[UDP_DATA_START],PSTR("CMD IP ")) != 0){	
		if ((eth_buffer[UDP_DATA_START+11] == myip[0] &&
			eth_buffer[UDP_DATA_START+12] == myip[1] &&
			eth_buffer[UDP_DATA_START+13] == myip[2] &&
			eth_buffer[UDP_DATA_START+14] == myip[3])||
			(eth_buffer[UDP_DATA_START+11] == 255 &&
			eth_buffer[UDP_DATA_START+12] == 255 &&
			eth_buffer[UDP_DATA_START+13] == 255 &&
			eth_buffer[UDP_DATA_START+14] == 255))
		{
			//write default port
			unsigned short tmp = 0x1936;
			eeprom_write_block(&tmp,(void *)64, 2);

			//write_eeprom_ip(IP_EEPROM_STORE);
			if (*((unsigned int*)&eth_buffer[UDP_DATA_START+7]) != 0x00000000)
			{	
				//value ins EEPROM schreiben
				for (unsigned char count = 0; count<4;count++)
				{
					eeprom_busy_wait ();
					eeprom_write_byte((unsigned char *)(IP_EEPROM_STORE + count),eth_buffer[UDP_DATA_START+7+count]);
				}
			}

			//write_eeprom_netmask(NETMASK_EEPROM_STORE);
			if (*((unsigned int*)&eth_buffer[UDP_DATA_START+15]) != 0x00000000)
			{	
				//value ins EEPROM schreiben
				for (unsigned char count = 0; count<4;count++)
				{
					eeprom_busy_wait ();
					eeprom_write_byte((unsigned char *)(NETMASK_EEPROM_STORE + count),eth_buffer[UDP_DATA_START+15+count]);
				}
			}
			
			(*((unsigned long*)&myip[0])) = MYIP;
			(*((unsigned long*)&netmask[0])) = NETMASK;

			//UDP_CMD_DEBUG("My IP: %1i.%1i.%1i.%1i\r\n",myip[0],myip[1],myip[2],myip[3]);
			//UDP_CMD_DEBUG("MASK %1i.%1i.%1i.%1i\r\n", netmask[0]  , netmask[1]  , netmask[2]  , netmask[3]);
			
			//send OK
			eth_buffer[UDP_DATA_START] = 'O';
			eth_buffer[UDP_DATA_START+1] = 'K';
			//create_new_udp_packet(2,UDP_CMD_PORT_TX+1392,UDP_CMD_PORT_TX,udp_cmd_pollReplyTarget);
			create_new_udp_packet(2,UDP_CMD_PORT_TX+3456,UDP_CMD_PORT_TX,(unsigned long)0xffffffff);
		}
	}
}

//----------------------------------------------------------------------------
//UDP Update Reset go Bootloader
void udp_bootloader_reset (unsigned char index)
{	
	struct IP_Header *ip;
		
	//Ethernet Interrupt deaktivieren
	ETH_INT_DISABLE;
		
	if ((eth_buffer[UDP_DATA_START] == myip[0] &&
	eth_buffer[UDP_DATA_START+1] == myip[1] &&
	eth_buffer[UDP_DATA_START+2] == myip[2] &&
	eth_buffer[UDP_DATA_START+3] == myip[3]))
	{
			WDTCSR = (1<<WDCE) | (1<<WDE);
			WDTCSR = (1<<WDCE) | (1<<WDE) | (1<<WDP0) | (1<<WDP1) | (1<<WDP2);
			while(1);
	}
}

	





