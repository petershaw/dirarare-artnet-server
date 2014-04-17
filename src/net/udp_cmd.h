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
#ifndef _UDP_CMD_CLIENT_H
	#define _UDP_CMD_CLIENT_H

	#include <avr/io.h>
	#include <avr/pgmspace.h>
	#include <string.h>

	//#define UDP_CMD_DEBUG usart_write
	#define UDP_CMD_DEBUG(...)

	#define UDP_CMD_PORT_RX			7600
	#define UDP_CMD_PORT_TX			7601
	#define UDP_BOOTLOADER_RESET	7602

	void udp_cmd_init(void);
	void udp_cmd_get(unsigned char);
	void udp_bootloader_reset (unsigned char);
	
#endif
