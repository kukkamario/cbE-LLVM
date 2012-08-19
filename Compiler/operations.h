#ifndef OPERATIONS_H
#define OPERATIONS_H
#include "precompiled.h"
#include "stackvalue.h"
namespace Operation {
void add(IRBuilder<> *b, const StackValue &left, const StackValue &right);
void subtract(IRBuilder<> *b, const StackValue &left, const StackValue &right);
void divide(IRBuilder<> *b, const StackValue &left, const StackValue &right);
void multiply(IRBuilder<> *b, const StackValue &left, const StackValue &right);
void unaryMinus(IRBuilder<> *b, const StackValue &value);
}

#endif // OPERATIONS_H
