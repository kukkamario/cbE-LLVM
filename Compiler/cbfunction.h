#ifndef CBFUNCTION_H
#define CBFUNCTION_H
#include "precomp.h"
#include "variable.h"
#include "instruction.h"
#include "bytecode.h"
class CBFunction
{
	public:
		CBFunction(ByteCode::iterator begin, ByteCode::iterator end, Function *func, Module* mod, bool isMain);
		bool parse();
		void init(int32_t ints, int32_t floats, int32_t strings, int32_t bytes, int32_t shorts, int32_t typePtrs);
	private:
		Variable *mIntVars;
		Module *mModule;
		map<int32_t, BasicBlock*> mBasicBlock;
		ByteCode::iterator mBCBegin;
		ByteCode::iterator mBCEnd;
		Function *mFunction;
		bool mIsMain;
};

#endif // CBFUNCTION_H
