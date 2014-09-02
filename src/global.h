
#ifndef firnament_global_h
#define firnament_global_h

#ifndef IgnoreTest
#include "./output/usart.h"
#else
#include "./../tests/src/mocks/usart.h"
#endif


// GLOBAL DEFINES AND MACROS

// DUMMY FUNCTION
// ---------------------------------------------
void *dummy(void);

// Debugging
// ---------------------------------------------
#ifdef DEBUGFN
#define debug_uart(...) do{ DEBUGFN( __VA_ARGS__ ); } while(0)
#else
#define debug_uart(...) do{ } while (0)
#endif

// GLOBAL SYS FUNCTIONS
// ---------------------------------------------
void (*reboot)(void);

// GLOBAL VARIABLES;
// ---------------------------------------------
char *eepromShadow;
//char eepromShadow[64];
char ip[16];
char net[16];
char gw[16];

volatile int isActive;

// DEVICE IP CONFIGURATION
// ---------------------------------------------
int ipintpart(char *, int);

#define MYIP		IP(ipintpart(&ip, 0), ipintpart(&ip, 1), ipintpart(&ip, 2), ipintpart(&ip, 3))
#define ROUTER_IP	IP(ipintpart(&gw, 0), ipintpart(&gw, 1), ipintpart(&gw, 2), ipintpart(&gw, 3))
#define NETMASK		IP(ipintpart(&net, 0), ipintpart(&net, 1), ipintpart(&net, 2), ipintpart(&net, 3))

#define ENCODER_TIMER2_OFF 	TIMSK2 = 0;
#define ENCODER_TIMER2_ON 	TIMSK2 |= (1<<TOIE2);
#endif
