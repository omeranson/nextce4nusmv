#include "NextCEDbg.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

/**
 * Is the value in NEXTCE_DEBUG set to level or higher? Retrun 1 if so, 0
 * otherwise
 * @param level The minimum level for NEXTCE_DEBUG
 * @return 1 if the value in NEXTCE_DEBUG set to level or higher
 */
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

/**
 * A printf-like method, that operates only if NEXTCE_DEBUG is set to the given
 * level. This method does nothing if the debug level is not set accordingly.
 * Note that an additional newline is automatically printed.
 * @param level The minimum level for NEXTCE_DEBUG
 * @param format the printf format string
 * @param ... va_list for printf (and its format)
 */
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

