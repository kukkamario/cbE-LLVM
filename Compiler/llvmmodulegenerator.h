#ifndef LLVMMODULEGENERATOR_H
#define LLVMMODULEGENERATOR_H
#include "precompiled.h"
#include "bytecode.h"
#include "variable.h"
#include "cbfunction.h"
#include "runtimefunction.h"
class LLVMModuleGenerator
{
	public:
		LLVMModuleGenerator();
		static LLVMModuleGenerator *instance();
		Module *generate(ByteCode &bc);
		RuntimeFunction *commandPrintI;
		RuntimeFunction *functionTimer;
		RuntimeFunction *commandEnd;
		Function *cbRuntimeMain;
	private:
		void separateFunctions(const ByteCode &bc);
		vector<CBFunction*> mFunctions;
};

#endif // LLVMMODULEGENERATOR_H
