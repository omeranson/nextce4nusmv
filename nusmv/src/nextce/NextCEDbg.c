#include "NextCEDbg.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int is_nextce_debug(int level) {
	char * nextce_debug = NULL;
	int nce_level = -1;
// #ifdef NUSMV_HAVE_GETENV
	nextce_debug = getenv("NEXTCE_DEBUG");
	if (nextce_debug) {
		nce_level = atoi(nextce_debug);
	}
// #endif
	if ((nce_level < 0) || (nce_level < level)) {
		return 0;
	}
	return 1;
}

void nextce_debug(int level, const char * format, ...) {
	if (!is_nextce_debug(level)) {
		return;
	}
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	printf("\n");
	va_end(args);
}

