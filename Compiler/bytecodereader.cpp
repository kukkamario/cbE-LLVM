#include "bytecodereader.h"


ByteCodeReader::ByteCodeReader()
{
}

bool ByteCodeReader::readCBExecutable(const QString &s) {
	mByteCode.clear();
	qint32 startPos; // Beginning of the CoolBasic data
	qint32 endPos; // End of the executable

	quint32 nStrings; // Number of strings
	quint32 size; // Length of CoolBasic data

	// Open file for reading
	ifstream input(qPrintable(s), ios::binary);

	if (!input.is_open()) {
		printf("Can't open exe! %s", qPrintable(s));
		return false;
	}

	input.seekg(-4, ios::end);
	endPos = input.tellg();
	input.read((char *)(&startPos), 4);
	input.seekg(24 - startPos, ios::end);
	input.read((char *)(&nStrings), 4);

	// Read and decrypt strings
	mByteCode.stringPool().reserve(nStrings);
	string key = "Mark Sibly is my idol!";
	for (quint32 i = 1; i <= nStrings; i++) {
		quint32 len;
		input.read((char *)(&len), 4);
		string s;
		char c;
		for (quint32 j = 0; j < len; j++) {
			input >> c;
			s += char(c - key[j % key.length()]);
		}
		mByteCode.stringPool().append(s);
	}

	// Skip useless data and directly to beginning of the bytecode
	input.seekg(32, ios::cur);
	startPos = input.tellg();

	// Read code to memory and close the file
	size = endPos - startPos;
	char *code = new char [size];

	input.read(code, size);
	input.close();
	quint32 ncmd = 0;
	quint32 i = 0;
	CBInstruction inst;
	while (i < size) {
		quint8 cmd = *(quint8 *)(code + i);
		ncmd++;
		i++;
		inst.mIndex = ncmd;
		switch (cmd) {
			case 65: inst.mOpCode = OCSetInt; break;
			case 66: inst.mOpCode = OCSetFloat; break;
			case 67: inst.mOpCode = OCCommand; break;
			case 68: inst.mOpCode = OCData; break;
			case 73: inst.mOpCode = OCPushInt; break;
			case 74: inst.mOpCode = OCPushSomething; break;
			case 78: inst.mOpCode = OCJump; break;
			case 79: inst.mOpCode = OCOperation; break;
			case 80: inst.mOpCode = OCIncVar; break;
			case 81: inst.mOpCode = OCIncGlobalVar; break;
			case 84: inst.mOpCode = OCPushTypeMemberVariable; break;
			case 85: inst.mOpCode = OCPushFuncPtr; break;
			case 86: inst.mOpCode = OCPushVariable; break;
			case 90: inst.mOpCode = OCFunction; break;
			case 97:
			case 98:
			case 99: inst.mOpCode = OCUnknown; break;
			default: printf("[%i] Unhandled preparsing1: %i \n",i, (quint32) cmd);
		}
		if (cmd == 79 || cmd == 68) {
			inst.mData = *(qint8 *)(code + i);
			mByteCode.append(inst);
			i ++;
		}
		else {
			inst.mData = *(qint32 *)(code + i);
			mByteCode.append(inst);
			i += 4;
		}
	}
	return true;
}
