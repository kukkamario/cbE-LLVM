#ifndef STRINGPOOL_H
#define STRINGPOOL_H
#include <precomp.h>
class StringPool {
	public:
		typedef QList<string>::iterator iterator;
		typedef QList<string>::const_iterator const_iterator;
		StringPool();
		void reserve(qint32 size) {mPool.reserve(size);}
		void append(const string &str) {mPool.append(str);}
		string get(qint32 key) const { return mPool[key];}
		void clear() { mPool.clear(); }
		const_iterator begin() const { return mPool.begin();}
		iterator begin() { return mPool.begin();}
		const_iterator end() const { return mPool.end();}
		iterator end() { return mPool.end();}
	private:
		QList<string> mPool;
};

#endif // STRINGPOOL_H
