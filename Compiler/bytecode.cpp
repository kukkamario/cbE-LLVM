#include "bytecode.h"

ByteCode::ByteCode()
{
}

ByteCode::ByteCode(const ByteCode &c) : mInstructions(c.mInstructions) {
}

void ByteCode::clear() {
	mInstructions.clear();
	mStringPool.clear();
}

void ByteCode::print() {
	cout << "Constant strings:\n";
	int id = 1;
	for (StringPool::const_iterator i = mStringPool.begin(); i != mStringPool.end(); i++) {
		cout << "[" + id << "] " << i->mString << "\n";
	}
	cout << "\n";
	cout << "CB bytecode:\n";
	id = 0;
	for (const_iterator i = begin(); i != end(); i++) {
		cout << "[" << id << "] ";
		for (int ii = 10; ii < 100000; ii *= 10) {
			if (id < ii) cout << " ";
		}
		cout << *i << "\n";
		id ++;
	}
	cout << "\n\n\n";
}
