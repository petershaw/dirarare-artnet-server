

#ifndef inputs_h
#define inputs_h

#include <stdint.h>
#include "../config.h"

/** 
 * Flow and Value variables 
 */
typedef struct Inputs {
	volatile uint8_t inputs_should_read;
	volatile uint8_t inputs_present;
	volatile uint8_t inputs_value;
} Inputs_t;

Inputs_t *inputmem;

/**
 * Initialize the PISO chip to the right ports
 */
void initialize_inputs(void);

/**
 * 
 */
void inputs_read_next(void);

void set_inputs_should_read(uint8_t);

/**
 * is one of the input is loaded?
 * same as global variable inputs_present
 */
uint8_t is_inputs_present(void);

uint8_t is_inputs_should_read(void);

/**
 * Get the last value from the piso and reset 
 */
int inputs_get_value(void);


#endif