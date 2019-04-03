#pragma once
#include "common.h"

typedef bool (*TestFn)(void);

// Individual tests and a full test both available; may add tests by section
// in the future
bool test_byte_manip(void);
bool test_next_token(void);
bool test_token_manip(void);
bool test_arithmatic(void);
bool run_tests(void);
