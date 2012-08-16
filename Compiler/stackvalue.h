#ifndef STACKVALUE_H
#define STACKVALUE_H
#include "precomp.h"
struct StackValue {
		enum Type {
			Int,
			Float,
			String,
			Any
		};
		Type mType;
		Value *mValue;
		bool mConstant;
		union {
				int32_t mInt;
				float mFloat;
				string *mString;
		};
};

#endif // STACKVALUE_H
