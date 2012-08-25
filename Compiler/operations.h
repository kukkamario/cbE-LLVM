#ifndef OPERATIONS_H
#define OPERATIONS_H
#include "precompiled.h"
#include "stackvalue.h"
namespace Operation {
StackValue add(IRBuilder<> *b, const StackValue &left, const StackValue &right);
StackValue subtract(IRBuilder<> *b, const StackValue &left, const StackValue &right);
StackValue divide(IRBuilder<> *b, const StackValue &left, const StackValue &right);
StackValue multiply(IRBuilder<> *b, const StackValue &left, const StackValue &right);
StackValue unaryMinus(IRBuilder<> *b, const StackValue &value);
StackValue equal(IRBuilder<> *b, const StackValue &left, const StackValue &right);
StackValue notEqual(IRBuilder<> *b, const StackValue &left, const StackValue &right);
StackValue greater(IRBuilder<> *b, const StackValue &left, const StackValue &right);
StackValue less(IRBuilder<> *b, const StackValue &left, const StackValue &right);
StackValue greaterOrEqual(IRBuilder<> *b, const StackValue &left, const StackValue &right);
StackValue lessOrEqual(IRBuilder<> *b, const StackValue &left, const StackValue &right);
}

#endif // OPERATIONS_H
