/* debug.c */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

int debug_flag;

void
debug(const char *format, ...)
{
		va_list args;

		if (debug_flag) {
				va_start(args, format);
				vfprintf(stderr, format, args);
				va_end(args);
		}
}
