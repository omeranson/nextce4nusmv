#ifndef __NEXTCE_DBG_H__
#define __NEXTCE_DBG_H__

/**
 * This file (and the matching .c file) provide some useful debugging APIs.
 * Specifically, they allow defining an environment variable NEXTCE_DEBUG, which
 * defines nextce-specific debug levels.
 */
int is_nextce_debug(int level);
void nextce_debug(int level, const char * format, ...);

#endif /* __NEXTCE_DBG_H__ */
