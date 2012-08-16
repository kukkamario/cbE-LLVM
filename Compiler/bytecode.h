#ifndef BYTECODE_H
#define BYTECODE_H
#include "instruction.h"
#include "stringpool.h"
#include <string>


class ByteCode {
	public:
		typedef list<CBInstruction>::const_iterator const_iterator;
		typedef list<CBInstruction>::iterator iterator;
		ByteCode();
		ByteCode(const ByteCode &c);
		const_iterator begin() const { return mInstructions.begin(); }
		iterator begin() { return mInstructions.begin(); }
		const_iterator end() const { return mInstructions.end(); }
		iterator end() { return mInstructions.end(); }
		void append(const CBInstruction &ins) { mInstructions.push_back(ins); }
		StringPool &stringPool() {return mStringPool;}
		void clear();
		void print();
	private:
		list<CBInstruction> mInstructions;
		StringPool mStringPool;
};

#endif // BYTECODE_H
