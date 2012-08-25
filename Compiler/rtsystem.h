#ifndef RTSYSTEM_H
#define RTSYSTEM_H
#include "precompiled.h"
namespace System {
	#define RT_EXTERN extern
	#define RT_COMPILER_HEADER
	#include "../RuntimeLibrary/system.h"
	#undef RT_EXTERN
	void init(Module *module);
}
#endif // RTSYSTEM_H
