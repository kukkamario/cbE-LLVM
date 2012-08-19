#ifndef CASTS_H
#define CASTS_H
#include "precompiled.h"
#include "stackvalue.h"
namespace Cast {
void init();

Value *toString(IRBuilder<> *b, const StackValue &val);
Value *toInt(IRBuilder<> *b, const StackValue &val);
Value *toFloat(IRBuilder<> *b, const StackValue &val);
Value *toShort(IRBuilder<> *b, const StackValue &val);
Value *toByte(IRBuilder<> *b, const StackValue &val);
}
#endif // CASTS_H
