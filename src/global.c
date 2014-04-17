// GLOBAL FUNCTIONS
#include <stdlib.h>
#include "global.h"


// DUMMY FUNCTION
// ---------------------------------------------
void *dummy(void){return (void *) NULL;};

// GLOBAL SYS FUNCTIONS
// ---------------------------------------------
void (*reboot)(void) = (void *) 0x0000;

// DEVICE IP CONFIGURATION
// ---------------------------------------------
int ipintpart(char *ip, int pos){
	int value = 0;
	char *token, *string, *tofree;
	tofree = string = strdup(ip);
	if(ip == NULL) return value;
	int counter = 0;
	
    while ((token = strsep(&string, ".")) != NULL){
		if(counter == pos){
			value = atoi(token);
			break;
		}
		++counter;
    }
    free(tofree);
	return value;
}