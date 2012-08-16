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
		void init(qint32 ints, qint32 floats, qint32 strings, qint32 bytes, qint32 shorts, qint32 typePtrs);
	private:
		Variable *mIntVars;
		Module *mModule;
		QMap<int32_t, BasicBlock*> mBasicBlock;
		ByteCode::iterator mBCBegin;
		ByteCode::iterator mBCEnd;
		Function *mFunction;
		bool mIsMain;
};

#endif // CBFUNCTION_H
