#ifndef FUNCTION_H
#define FUNCTION_H
#include "precompiled.h"
class RuntimeFunction {
	public:
		RuntimeFunction(const string &name, Type *ret, ArrayRef<Type *> args);
		void appendToModule(Module *mod);
		Function *function() { return mFunction; }
	private:
		FunctionType *mFunctionType;
		Function *mFunction;
		string mName;
};

#endif // FUNCTION_H
