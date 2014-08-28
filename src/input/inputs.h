

#ifndef inputs_h
#define inputs_h

#include <stdint.h>
#include "../config.h"

#define PRESSED_ENTER 	1
#define PRESSED_RIGHT 	2
#define PRESSED_LEFT	3

volatile uint8_t next_action;

/**
 * Initialize the PISO chip to the right ports
 */
void initialize_inputs(void);


#endif