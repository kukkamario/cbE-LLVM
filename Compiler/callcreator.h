#ifndef CALLCREATOR_H
#define CALLCREATOR_H
#include "precompiled.h"
#include "stackvalue.h"

#define createCall(b, argNum, ...) createCall_(b, argNum, __VA_ARGS__ , 0)
/** Creates call instruction for runtime function.
*/
StackValue createCall_(IRBuilder<> *b, int32_t argNum, ...);

#endif // CALLCREATOR_H
