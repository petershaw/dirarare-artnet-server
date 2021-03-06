
#ifndef IgnoreTest
#define IgnoreTest 1
#endif

#include "input/inputbufferTest.h"
#include "ui/menuTest.h"

void *dummy(void);

// This struct contains all test suites
TEST_SUITES {
	TEST_SUITE_ADD(INPUTBUFFER_TEST),
    TEST_SUITE_ADD(UI_MENUTEST),
    TEST_SUITES_CLOSURE
};

int main(int argc, char *argv[]) {
    // Set up directory where are stored files with outputs from test
    // suites
    CU_SET_OUT_PREFIX("regressions/");
    
    // Run all test suites
    CU_RUN(argc, argv);
    
    return 0;
}
