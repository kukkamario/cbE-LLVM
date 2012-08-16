#include "runtimefunction.h"

RuntimeFunction::RuntimeFunction(const string &name, Type * ret, ArrayRef<Type*> args) {
	mFunctionType = FunctionType::get(ret, args, false);
	mName = name;
	mFunction = 0;
}

void RuntimeFunction::appendToModule(Module *mod) {
	mFunction = mod->getFunction(mName);
	if (!mFunction) {
		mFunction = Function::Create(mFunctionType, GlobalValue::ExternalLinkage, mName, mod);
		mFunction->setCallingConv(CallingConv::C);
	}
}
