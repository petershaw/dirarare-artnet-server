// GLOBAL FUNCTIONS
#include <stdlib.h>
#include "global.h"

// PRAGMA HACK TO PRINT DEFINES
// ---------------------------------------------
#define VALUE_TO_STRING(x) #x
#define VALUE(x) VALUE_TO_STRING(x)
#define VAR_NAME_VALUE(var) #var "="  VALUE(var)


// DUMMY FUNCTION
// ---------------------------------------------
void *dummy(void){return (void *) NULL;};

// GLOBAL SYS FUNCTIONS
// ---------------------------------------------
void (*reboot)(void) = (void *) 0x0000;

// CHECK REVISION
// ---------------------------------------------
#ifndef REVISION
	#define REVISION 0
#endif
#if REVISION < 2
	#warning *-----------------------------------------------------------
	#warning *
	#pragma message(VAR_NAME_VALUE(REVISION))
	#error THIS SOFTWARE IS WRITTEN FOR REVISION 2
	#warning *
	#warning *-----------------------------------------------------------
#endif 

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