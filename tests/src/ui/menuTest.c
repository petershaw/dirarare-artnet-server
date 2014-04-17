//
//  menuTest.c
//  AnalyserDude
//
//  Created by Peter Shaw on 3/9/13.
//
//

#include <stdio.h>
#include "../cu/cu.h"
#include "../../src/ui/menu.h"


static void dummy(void){}

TEST(test1) {
    assertTrue(1);
    printf("MenuTest test\n");
    assertEquals(1, 1);
}

TEST(test_init_menu) {
    ui_menu_init();
    assertTrue( sizeof(menu->firstentry) == 8);
    assertTrue( menu->firstentry == menu->lastentry );
}

TEST(test_add_elements) {
    ui_menu_init();
    
    char *text_one;
    asprintf(&text_one, "A Test Entry");
    ui_menu_add(text_one, &dummy);
    
    char *text_two;
    asprintf(&text_two, "A second Test");
    ui_menu_add(text_two, &dummy);

    
    printf("Data first: %s\n", menu->firstentry->data);
    printf("Data last: %s\n", menu->lastentry->data);
    
    assertFalse( menu->firstentry == menu->lastentry );
    assertTrue( menu->firstentry->next == menu->lastentry);

    
}

TEST(test_move_around){
    ui_menu_init();
    
    char *text_one;
    asprintf(&text_one, "one");
    ui_menu_add(text_one, &dummy);
    
    char *text_two;
    asprintf(&text_two, "two");
    ui_menu_add(text_two, &dummy);
    
    char *text_three;
    asprintf(&text_three, "three");
    ui_menu_add(text_three, &dummy);
    
    menuentry_t *current = menu->firstentry;
    assertEquals(text_one, current->data);
    
    current = current->next;
    assertEquals(text_two, current->data);
    
    current = current->next;
    assertEquals(text_three, current->data);

    // move to top on end of menu
    current = current->next;
    assertEquals(text_one, current->data);  
}

TEST(test_add_submenu) {
    ui_menu_init();
    
    char *text_one;
    asprintf(&text_one, "A Leaf Entry");
    ui_menu_add(text_one, &dummy);
    
    char *text_two;
    char *text_two_one;
    char *text_two_two;
    asprintf(&text_two, "A Node Entry");
    asprintf(&text_two_one, "A Sub Leaf Entry");
    asprintf(&text_two_two, "Another Sub Leaf Entry");
    menuentry_t *e = ui_menu_add(text_two, &dummy);
    ui_menu_add_sub(e, text_two_one, &dummy );
    ui_menu_add_sub(e, text_two_two, &dummy );
    
    
    menuentry_t *current = menu->firstentry;
    assertEquals(text_one, current->data);
    assertTrue(current->submenu == NULL);
    
    current = current->next;
    assertEquals(text_two, current->data);
    
    current = current->submenu;
    assertEquals(text_two_one, current->data);
    
    current = current->next;
    assertEquals(text_two_two, current->data);
    
}

TEST(test_move_around_submenu) {
    ui_menu_init();
    
    char *text_two;
    char *text_two_one;
    char *text_two_two;
    asprintf(&text_two, "A Node Entry");
    asprintf(&text_two_one, "A Sub Leaf Entry");
    asprintf(&text_two_two, "Another Sub Leaf Entry");
    menuentry_t *e = ui_menu_add(text_two, &dummy);
    ui_menu_add_sub(e, text_two_one, &dummy );
    ui_menu_add_sub(e, text_two_two, &dummy );
    
    
    menuentry_t *current = menu->firstentry;
    current = current->submenu;
    assertEquals(text_two_one, current->data);
    
    current = current->next;
    assertEquals(text_two_two, current->data);

    // move around
    current = current->next;
    assertEquals(text_two_one, current->data);
    
}

TEST(test_next_and_enter_submenu) {
    ui_menu_init();
    
    char *text_one;
    asprintf(&text_one, "A Leaf Entry");
    ui_menu_add(text_one, &dummy);
    
    char *text_two;
    char *text_two_one;
    char *text_two_two;
    asprintf(&text_two, "A Node Entry");
    asprintf(&text_two_one, "A Sub Leaf Entry");
    asprintf(&text_two_two, "Another Sub Leaf Entry");
    menuentry_t *e = ui_menu_add(text_two, &dummy);
    ui_menu_add_sub(e, text_two_one, &dummy );
    ui_menu_add_sub(e, text_two_two, &dummy );
    
    
    assertEquals(text_one, active->data);
    assertTrue(active->submenu == NULL);
    
    ui_menu_next();
    assertEquals(text_two, active->data);
    assertEquals(text_two, ui_menu_show());    
    printf("text_two> ", ui_menu_show());

    
    ui_menu_enter_submenu();
    assertEquals(text_two_one, active->data);
    assertEquals(text_two_one, ui_menu_show());    
    printf("text_two_one> ", ui_menu_show());
    
    ui_menu_next();
    assertEquals(text_two_two, active->data);
    assertEquals(text_two_two, ui_menu_show());    
    printf("text_two_two> ", ui_menu_show());
}

TEST(test_next_and_leave_submenu) {
    ui_menu_init();
    
    char *text_one;
    asprintf(&text_one, "A Leaf Entry");
    ui_menu_add(text_one, &dummy);
    
    char *text_two;
    char *text_two_one;
    char *text_two_two;
    asprintf(&text_two, "A Node Entry");
    asprintf(&text_two_one, "A Sub Leaf Entry");
    asprintf(&text_two_two, "Another Sub Leaf Entry");
    menuentry_t *e = ui_menu_add(text_two, &dummy);
    ui_menu_add_sub(e, text_two_one, &dummy );
    ui_menu_add_sub(e, text_two_two, &dummy );
    
    
    assertEquals(text_one, active->data);
    assertTrue(active->submenu == NULL);
    
    ui_menu_next();
    assertEquals(text_two, active->data);
    
    // ENTER
    ui_menu_enter_submenu();
    assertEquals(text_two_one, active->data);
    
    ui_menu_next();
    assertEquals(text_two_two, active->data);

    // LEAVE
    ui_menu_leave_submenu();
    assertEquals(text_two, active->data);
    
}

TEST(test_has_submenu) {
    ui_menu_init();
    
    char *text_one;
    asprintf(&text_one, "A Leaf Entry");
    ui_menu_add(text_one, &dummy);
    
    char *text_two;
    char *text_two_one;
    asprintf(&text_two, "A Node Entry");
    asprintf(&text_two_one, "A Sub Leaf Entry");
    menuentry_t *e = ui_menu_add(text_two, &dummy);
    ui_menu_add_sub(e, text_two_one, &dummy );
    
    
    assertEquals(text_one, active->data);
    assertTrue(active->submenu == NULL);
    assertFalse(ui_menu_has_submenu());
    
    ui_menu_next();
    assertEquals(text_two, active->data);
    assertTrue(ui_menu_has_submenu());
}
