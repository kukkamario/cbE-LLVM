#include "bytecodereader.h"


ByteCodeReader::ByteCodeReader()
{
}

bool ByteCodeReader::readCBExecutable(const string &s) {
	mByteCode.clear();
	int32_t startPos; // Beginning of the CoolBasic data
	int32_t endPos; // End of the executable

	uint32_t nStrings; // Number of strings
	uint32_t size; // Length of CoolBasic data

	// Open file for reading
	ifstream input(s.c_str(), ios::binary);

	if (!input.is_open()) {
		printf("Can't open exe! %s", s.c_str());
		return false;
	}

	// Find out size of the file by seeking to it's end
	input.seekg(-4, ios::end);
	endPos = input.tellg();

	// Read the offset for CB-bytecode and go there
	input.read((char *)(&startPos), 4);
	input.seekg(24 - startPos, ios::end);

	// Number of strings
	input.read((char *)(&nStrings), 4);

	// Read and decrypt strings
	mByteCode.stringPool().reserve(nStrings);
	const char key[] = "Mark Sibly is my idol!";
	for (uint32_t i = 1; i <= nStrings; i++) {
		uint32_t len;
		input.read((char *)(&len), 4);
		string s;
		unsigned char c;
		for (uint32_t j = 0; j < len; j++) {
			input >> c;
			c = c - key[j % 22];
			if (c > 178 || c == 0) {
				if (c > 161) {
					c--;
				}
				if (c == 0) {
					c = 255;
				}
			}
			s += c;
		}
		mByteCode.stringPool().append(s);
	}

	// Skip useless data and go to beginning of the bytecode
	input.seekg(31, ios::cur);
	char pushIntCheck;
	input.read(&pushIntCheck, 1);
	if (pushIntCheck == 73) {//PushInt
		input.seekg(-1, ios_base::cur);
	}
	startPos = input.tellg();

	// Read code to memory and close the file
	size = endPos - startPos;
	char *code = new char [size];

	input.read(code, size);
	input.close();
	uint32_t ncmd = 0;
	uint32_t i = 0;
	CBInstruction inst;
	while (i < size) {
		uint8_t cmd = *(uint8_t *)(code + i);
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
			default: printf("[%i] Unhandled preparsing1: %i \n",i, (uint32_t) cmd);
		}
		if (cmd == 79 || cmd == 68) {
			inst.mData = *(uint8_t *)(code + i);
			mByteCode.append(inst);
			i ++;
		}
		else {
			inst.mData = *(int32_t *)(code + i);
			mByteCode.append(inst);
			i += 4;
		}
	}
	return true;
}
