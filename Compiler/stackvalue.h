#ifndef STACKVALUE_H
#define STACKVALUE_H
#include "precompiled.h"

struct StackValue {
		enum Type {
			Int = 0,
			Float,
			String,
			Bool,
			TypePtr,
			Void
		};
		StackValue() : mType(Void), mConstant(true), mValue(0), mInt(0) {}
		StackValue(int32_t i, Value *v);
		StackValue(float f, Value *v);
		StackValue(Type t, Value *v);
		Type mType;
		Value *mValue;
		bool mConstant;
		union {
				int32_t mInt;
				float mFloat;
				const string *mString;
		};
};

inline StackValue::StackValue(int32_t i, Value *v) :
	mType(Int),
	mConstant(true),
	mInt(i),
	mValue(v)
{
}

inline StackValue::StackValue(float f, Value *v) :
	mType(Float),
	mConstant(true),
	mFloat(f),
	mValue(v)
{
}

inline StackValue::StackValue(Type t, Value *v) :
	mType(t),
	mConstant(false),
	mValue(v)
{
}

#endif // STACKVALUE_H
