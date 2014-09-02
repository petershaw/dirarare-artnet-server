//
//  menuController.h
//  
//
//  Created by Peter Shaw on 3.9.2014.
//
//

#ifndef _menu_controller_h
#define _menu_controller_h

#include "../functions/abstractFunction.h"

// CONTROLLER FUNCTIONS

void ui_menucontroller_init(void);

void ui_menucontroller_print(char *text);
void ui_menucontroller_info(char *text, int);

void ui_menucontroller_show(void);

void ui_menucontroller_hide(void);

void ui_menucontroller_defaultoff(int);

int32_t val;
int32_t lastval;

#endif
