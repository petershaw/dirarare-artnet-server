//
//  menu.h
//  
//
//  Created by Peter Shaw on 3/9/13.
// - modularised at 09.03.2014
//

#ifndef _menu_h
#define _menu_h

#include <stdbool.h>

/**
 * Represents a single element in the menu.
 */
typedef struct menuentry {
    int position;
    struct menuentry *next;     // pointer to the next entry
    struct menuentry *parent;   // pointer to the parent entry
    char *message;              // the raw data string to show
    void (*fn)(int, void*);     // function to execute at startup
    void *data					// data to handle with
} menuentry_t;

/**
 * Represents a menu.
 */
typedef struct menustruc {
    menuentry_t *firstentry;
    menuentry_t *lastentry;
} menu_t;

menu_t *menu;

menuentry_t *active;

void ui_menu_init(void);
menuentry_t *ui_menu_add(const char*, void*, void*);
void ui_menu_next(void);
void ui_menu_previous(void);
char* ui_menu_show(void);
void* ui_menu_data(void);
void ui_menu_run(int);

#endif
