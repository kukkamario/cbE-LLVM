#ifndef BYTECODE_H
#define BYTECODE_H
#include <QSharedData>
#include <QLinkedList>
#include "instruction.h"
#include "stringpool.h"
#include <string>


class ByteCode {
	public:
		typedef QLinkedList<CBInstruction>::const_iterator const_iterator;
		typedef QLinkedList<CBInstruction>::iterator iterator;
		ByteCode();
		ByteCode(const ByteCode &c);
		const_iterator begin() const { return mInstructions.begin(); }
		iterator begin() { return mInstructions.begin(); }
		const_iterator end() const { return mInstructions.end(); }
		iterator end() { return mInstructions.end(); }
		void append(const CBInstruction &ins) { mInstructions.append(ins); }
		StringPool &stringPool() {return mStringPool;}
		void clear();
		void print();
	private:
		QLinkedList<CBInstruction> mInstructions;
		StringPool mStringPool;
};

#endif // BYTECODE_H
