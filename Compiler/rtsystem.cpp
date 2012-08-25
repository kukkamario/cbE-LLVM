#include "rtsystem.h"
#include "rtstring.h"
namespace System {

#define RT_EXTERN
#include "../RuntimeLibrary/system.h"

#undef RT_COMPILER_HEADER
void init(Module *module) {
#include "../RuntimeLibrary/system.h"
}

}

