#ifndef STRINGPOOL_H
#define STRINGPOOL_H
#include "precompiled.h"
class StringPool {
	public:
		struct ConstString {
				string mString;
				Constant *mConstArray;
				GlobalVariable *mGlobalVariable;
		};

		typedef vector<ConstString>::iterator iterator;
		typedef vector<ConstString>::const_iterator const_iterator;
		StringPool();
		void reserve(int32_t size) {mPool.reserve(size);}
		void append(const string &str);
		const ConstString &get(int32_t key) const { return mPool[key];}
		void clear() { mPool.clear(); }
		const_iterator begin() const { return mPool.begin();}
		iterator begin() { return mPool.begin();}
		const_iterator end() const { return mPool.end();}
		iterator end() { return mPool.end();}
		void addStringConstants(Module *module);
	private:
		vector<ConstString> mPool;
};

#endif // STRINGPOOL_H
