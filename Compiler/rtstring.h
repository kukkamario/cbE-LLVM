#ifndef COMPILER_STRING_H
#define COMPILER_STRING_H
#include "precompiled.h"
#include <llvm/Type.h>
namespace String {
	extern StructType *mStructType;

	#define RT_EXTERN extern
	#define RT_COMPILER_HEADER
	#include "../RuntimeLibrary/string.h"
	#undef RT_EXTERN
	void init(Module *module);
}
#endif // STRING_H
