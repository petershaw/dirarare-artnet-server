//
//  menu.c
//  Dirarare
//
//  Created by Peter Shaw on 3/9/13.
//
// @CompileTest
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "menu.h"
#ifndef IgnoreTest
#include "../output/usart.h"
#else
#include "../../tests/src/mocks/usart.h"
#endif

/**
 * initialize the menu
 */
void ui_menu_init(void){
	menu = calloc(1, sizeof(menu_t));
    
	menuentry_t *entry = malloc(sizeof (menuentry_t));
	entry->position = -1; 
	entry->message = '\0';
	entry->data = NULL;
	
	// loop around
	entry->next = entry;
	entry->parent = NULL;
	entry->fn = NULL;

	menu->firstentry = entry;
	menu->lastentry = entry;

    active = entry;
}

/**
 * add a item to the bottom of the menu
 */
menuentry_t *ui_menu_add(const char* message, void* fn, void* data){
    // get the last entry number
    menuentry_t *last = menu->lastentry;
    int lastposition = last->position;
    if( lastposition > -1){
        // if this should be a sub item
        // create a new item
        menuentry_t *entry = malloc(sizeof (menuentry_t));
        entry->parent = NULL;
        
        entry->position = ++lastposition;
        entry->message = message;
        entry->data = data;
        entry->fn = fn;
        entry->next = menu->firstentry;
        
        last->next = entry;
        menu->lastentry = entry;
        
        return entry;
    } else {
        last->message = message;
        last->data = data;
        last->fn = fn;
        last->position = 0;
        return last;
    }
}


/**
 * Move down the menu and set the next element active
 */
void ui_menu_next(void){
    active = active->next;
}

/**
 * Move up the menu and set the next element active
 */
void ui_menu_previous(void){
	menuentry_t *curActive = active;
	while(active->next != curActive){
		active = active->next;
	}
}

/**
 * Return the current active item string
 */
char* ui_menu_show(void){
    return(active->message);
}

void *ui_menu_data(void){
    return active->data;
}

void ui_menu_run(int a){
    active->fn(a,  active->data);
}


