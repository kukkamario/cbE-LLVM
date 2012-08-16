#ifndef STRINGPOOL_H
#define STRINGPOOL_H
#include <precomp.h>
class StringPool {
	public:
		typedef vector<string>::iterator iterator;
		typedef vector<string>::const_iterator const_iterator;
		StringPool();
		void reserve(int32_t size) {mPool.reserve(size);}
		void append(const string &str) {mPool.push_back(str);}
		string get(int32_t key) const { return mPool[key];}
		void clear() { mPool.clear(); }
		const_iterator begin() const { return mPool.begin();}
		iterator begin() { return mPool.begin();}
		const_iterator end() const { return mPool.end();}
		iterator end() { return mPool.end();}
	private:
		vector<string> mPool;
};

#endif // STRINGPOOL_H
