#ifndef RTMATH_H
#define RTMATH_H
#include "precompiled.h"

namespace Math {
#define RT_EXTERN extern
#define RT_COMPILER_HEADER
#include "../RuntimeLibrary/math.h"
#undef RT_EXTERN
void init(Module *module);
}
#endif // RTMATH_H
