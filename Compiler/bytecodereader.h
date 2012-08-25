#ifndef BYTECODEREADER_H
#define BYTECODEREADER_H
#include "bytecode.h"
#include "precompiled.h"

class ByteCodeReader
{
	public:
		ByteCodeReader();
		bool readCBExecutable(const string &s);
		ByteCode &byteCode() { return mByteCode; }
	private:
		ByteCode mByteCode;
};

#endif // BYTECODEREADER_H
