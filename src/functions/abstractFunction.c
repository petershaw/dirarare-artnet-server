//
//  Function ip.c
//

#include <stdio.h>
#include <stdlib.h>
#include "abstractFunction.h"
#include "../global.h"


int Int_dummyEditFn_Int(int i, void *datafn() ){return 0;};

void fn_init(void){
	currentFunction = malloc(sizeof(Function_t));
	currentFunction->jump_edit 			= Int_dummyEditFn_Int;
	currentFunction->jump_loop 			= dummy;
	currentFunction->highlightPosition  = 0;
	currentFunction->currentMessage 	= calloc(17, sizeof(char));
	
}

