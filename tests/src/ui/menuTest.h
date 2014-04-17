#include "../cu/cu.h"

// Declarations of tests
TEST(test1);
TEST(test_init_menu);
TEST(test_add_elements);
TEST(test_move_around);
TEST(test_add_submenu);
TEST(test_move_around_submenu);
TEST(test_next_and_enter_submenu);
TEST(test_next_and_leave_submenu);
TEST(test_has_submenu);

// Collect tests into test suite
TEST_SUITE(UI_MENUTEST) {
    TEST_ADD(test1),
    TEST_ADD(test_init_menu),
    TEST_ADD(test_add_elements),
    TEST_ADD(test_move_around),
    TEST_ADD(test_add_submenu),
    TEST_ADD(test_move_around_submenu),
    TEST_ADD(test_next_and_enter_submenu),
    TEST_ADD(test_next_and_leave_submenu),
    TEST_ADD(test_has_submenu),
    TEST_SUITE_CLOSURE
};

static void *dummy(void);
