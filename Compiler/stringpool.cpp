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

	int32_t index = 1;
	for (vector<ConstString>::iterator i = mPool.begin(); i != mPool.end(); i++) {
		ConstString &str = *i;


		str.mConstArray = ConstantDataArray::getString(module->getContext(), str.mString);
		str.mGlobalVariable = new GlobalVariable(*module, ArrayType::get(IntegerType::get(module->getContext(), 8), str.mString.size() + 1),
												 true,
												 GlobalVariable::PrivateLinkage,
												 str.mConstArray, "userString_"+boost::lexical_cast<string>(index));
		index++;
	}
}
