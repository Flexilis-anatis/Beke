#pragma once
#include <stdbool.h>
#include <stddef.h>

typedef char *(*TestFn)(void);

// Individual tests and a full test both available; may add tests by section
// in the future
char *test_byte_manip(void);
char *test_next_token(void);
char *test_token_manip(void);
bool run_tests(void);
