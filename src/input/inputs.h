

#ifndef inputs_h
#define inputs_h

#include <stdint.h>
#include "../config.h"

#define PRESSED_ENTER 	1
#define PRESSED_RIGHT 	2
#define PRESSED_LEFT	3

volatile uint8_t next_action;
volatile int8_t enc_delta;          // -128 ... 127
static int8_t last;

/**
 * Initialize the PISO chip to the right ports
 */
void initialize_inputs(void);
int8_t encode_read1( void );


int32_t val;

#endif
