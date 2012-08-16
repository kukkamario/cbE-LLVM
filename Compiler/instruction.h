#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include "precomp.h"
enum OpCode {
	OCUnknown = 0,
	OCPushInt,
	OCSetInt,
	OCSetFloat,
	OCCommand,
	OCData,
	OCPushSomething,
	OCJump,
	OCOperation,
	OCIncVar,
	OCIncGlobalVar,
	OCPushTypeMemberVariable,
	OCPushFuncPtr,
	OCPushVariable,
	OCFunction,
	OpCodeCount
};

extern const char* OpCodeNames[OpCodeCount];

struct CBInstruction {
		CBInstruction() : mOpCode(OCUnknown), mData(0), mBasicBlock(0) {
		}
		CBInstruction(OpCode op, int32_t d) : mOpCode(op), mData(d) {}
		OpCode mOpCode;
		int32_t mData;
		int32_t mIndex;
		llvm::BasicBlock *mBasicBlock;
};

inline ostream &operator << (ostream & s, const CBInstruction &i) {
	s << "<" << OpCodeNames[i.mOpCode] << ">";
	int len = 25 - strlen(OpCodeNames[i.mOpCode]);
	for (int32_t ii = 0; ii < len; ii++) {
		s << " ";
	}
	s << i.mData;
	return s;
}

#endif // INSTRUCTION_H
