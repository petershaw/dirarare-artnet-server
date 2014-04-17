//
//  Function ip.c
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "ip.h"
#include "../global.h"

int edit_ip(int direction, void *datafn() ){
	char *data = (char *) datafn();
	
	isEnter(direction){
		// jump to next field
		++editPosition;
		if(editPosition >= 15){
			editPosition = -1;
			return 0;			// exit and mark as done.
		}
		if(editPosition == 3 || editPosition == 7 || editPosition == 11){
			++editPosition;
		}
		currentFunction->highlightPosition = editPosition;
		return 1; 	
	}

	if(editPosition == -1){ return 1; }

	char num = data[editPosition];
 	int digitInEdit = (atoi(&num));

 	isLeft(direction){
 		// decrease 
 		 digitInEdit =  digitInEdit -1;
 		 if(digitInEdit < 0){ digitInEdit = 9; }
 	}

 	isRight(direction){
 		// increase
 		digitInEdit = digitInEdit +1;
 		if(digitInEdit > 9){ digitInEdit = 0; }
 	}

 	data[editPosition] = (char) digitInEdit +48;

	return 1;	
}


void *fn_firnament_ip(int init, void *data){
	on_init {
		if(strlen(data) < 15){ 
			strcpy_P(data, PSTR("___.___.___.___")); 
		}
 		strcpy(currentFunction->currentMessage, data);
 		currentFunction->jump_edit = &edit_ip;
 		currentFunction->jump_loop = dummy;
 		currentFunction->highlightPosition = 0;
 		editPosition = 0;
	}
	
	on_exit {
		strcpy(currentFunction->currentMessage, '\0');
		currentFunction->jump_edit = &Int_dummyEditFn_Int;
		currentFunction->jump_loop = dummy;
		currentFunction->highlightPosition = 0;
	}
	
	on_run {
		// update message variable
		strncpy(currentFunction->currentMessage, data, 15);
	}

}

