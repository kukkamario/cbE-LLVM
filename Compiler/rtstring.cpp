#include "rtstring.h"

namespace String {

StructType *mStructType;
StructType *mSharedDataStructType;
#define RT_EXTERN
#include "../RuntimeLibrary/string.h"

#undef RT_COMPILER_HEADER
void init(Module *module) {

	/*vector<Type*> elements;
	elements.push_back(Type::getInt32PtrTy(module->getContext()));
	mStructType = StructType::create(module->getContext(), elements, "class.ISString");*/
	mStructType = module->getTypeByName("struct.ISString");
	assert(mStructType);

#include "../RuntimeLibrary/string.h"
}

}
