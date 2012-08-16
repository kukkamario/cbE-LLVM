#ifndef BYTECODEREADER_H
#define BYTECODEREADER_H
#include "bytecode.h"
#include "precomp.h"

class ByteCodeReader
{
	public:
		ByteCodeReader();
		bool readCBExecutable(const string &s);
		ByteCode byteCode() const { return mByteCode; }
	private:
		ByteCode mByteCode;
};

#endif // BYTECODEREADER_H
