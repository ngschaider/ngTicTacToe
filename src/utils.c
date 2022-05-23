#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <stdbool.h>

#include "utils.h"


void flush_stdin(void) {
	while (getchar() != '\n');
}