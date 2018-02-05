#ifndef __DEBUG_H
#define __DEBUG_H

#define devel_debug 1
#define console_debug 0

#if devel_debug
static char cmd_bufferx[200];
static unsigned int cmd_counter = 0;
#endif

#if console_debug
#define ASSERT(x)  if (!(x)) fprintf(stderr, "Assert fail at %s:%d\n", __FUNCTION__, __LINE__)
#define DEBUG(...) fprintf(stderr, __VA_ARGS__)
#else
#define ASSERT(x)
#define DEBUG(...)
#endif

#endif /* __DEBUG_H */