//
//  Function: abstractFunction
//

#ifndef firnament_fn_abstract_h
#define firnament_fn_abstract_h

#define FUNCTION_RE_CALL		0
#define FUNCTION_FIRST_CALL		1
#define FUNCTION_LAST_CALL		2
#define FUNCTION_DO_SAVE		3
#define FUNCTION_IO_LEFT		4
#define FUNCTION_IO_RIGHT		5
#define FUNCTION_IO_ENTER		6


#define on_run             		if(init == FUNCTION_RE_CALL)
#define on_init             	if(init == FUNCTION_FIRST_CALL)
#define on_exit             	if(init == FUNCTION_LAST_CALL)
#define on_save             	if(init == FUNCTION_DO_SAVE)

#define on_goto(v)		     	int v; \
								if(init == FUNCTION_IO_LEFT)  { v = -1; } \
								if(init == FUNCTION_IO_RIGHT) { v = 1; } \
								if(init == FUNCTION_IO_ENTER) { v = 0; } \
								if(init == FUNCTION_IO_LEFT || init == FUNCTION_IO_RIGHT || init == FUNCTION_IO_ENTER) 

#define isEnter(v)				if(v == 0)
#define isLeft(v)				if(v == -1)
#define isRight(v)				if(v == 1)

// DUMMY FUNCTIONS
// ---------------------------------------------
int		 Int_dummyEditFn_Int(int, void *datafn() );

// FUNCTION OBJECT DEFINITION
// ---------------------------------------------
typedef struct Function {
	int (*jump_edit)(int, void (*datafn)());
	void(*jump_loop)(void);
	int highlightPosition;
	char* currentMessage;
} Function_t;

// CURRENT ACTIVE FUNCTION PTR
// ---------------------------------------------
Function_t *currentFunction;

// GLOBAL FUNCTION VARIABLES
// ---------------------------------------------
int8_t editPosition;

// INITIALISATION 
// ---------------------------------------------
void fn_init(void);

#endif
