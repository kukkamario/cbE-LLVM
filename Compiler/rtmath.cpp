#include "rtmath.h"

namespace Math {
#define RT_EXTERN
#include "../RuntimeLibrary/math.h"

#undef RT_COMPILER_HEADER
void init(Module *module) {
#include "../RuntimeLibrary/math.h"
}

}
