#include "rtstring.h"

namespace String {

StructType *mStructType;
#define RT_EXTERN
#include "../RuntimeLibrary/string.h"

#undef RT_COMPILER_HEADER
void init(Module *module) {
	vector<Type*> elements;
	elements.push_back(Type::getInt32PtrTy(module->getContext()));
	mStructType = StructType::create(module->getContext(), elements, "ISString");

#include "../RuntimeLibrary/string.h"
}

}
