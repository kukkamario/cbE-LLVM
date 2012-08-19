#include "stringpool.h"
#include <llvm/Constants.h>
#include "string.h"

StringPool::StringPool()
{
}

void StringPool::append(const string &str)
{
	ConstString s;
	s.mString = str;
	mPool.push_back(s);
}

void StringPool::addStringConstants(Module *module)
{
	for (vector<ConstString>::iterator i = mPool.begin(); i != mPool.end(); i++) {
		ConstString &str = *i;
		str.mConstArray = ConstantArray::get(module->getContext(), str.mString);
		str.mGlobalVariable = new GlobalVariable(ArrayType::get(IntegerType::get(module->getContext(), 8), str.mString.size() + 1),
												 true,
												 GlobalVariable::PrivateLinkage,
												 str.mConstArray);
	}
}
