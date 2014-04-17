//
//  driver.h
//  header for all drivers
//
//
//

#ifndef drviver_eeprom_h
#define drviver_eeprom_h

#include <stdint.h>
#include "driver.h"

static uint8_t LOCK = 0;

drv_open(eeprom);
drv_close(eeprom);
drv_read(eeprom, char *, uint8_t);
drv_write(eeprom, char *);

#endif
