#ifndef DEBUG_H
#define DEBUG_H
#ifdef HCDEBUG_ENABLED
	#define HCDEBUG(a, ...) printf(a, )
#else
	#define HCDEBUG(a, ...)
#endif
#endif // DEBUG_H
