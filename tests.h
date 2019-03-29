#pragma once
#include <stdbool.h>
#include <stddef.h>

typedef char *(*TestFn)(void);

char *test_byte_manip(void);

const TestFn test_fns[2];
