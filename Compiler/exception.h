#ifndef EXCEPTION_H
#define EXCEPTION_H
#include "precompiled.h"
class Exception
{
	public:
		enum Type {
			Cast,
			Function,
			Unknown
		};

		Exception(Type t, const string &m);
		Type type() const {return mType;}
		const string &msg() const{return mMsg;}
	private:
		string mMsg;
		Type mType;
};

inline ostream &operator << (ostream & s, const Exception &i) {
	char *exceptionNames[] = {
		"Cast",
		"Function"
		"Unknown"
	};
	s << exceptionNames[(int)i.type()] << " exception: " << i.msg();
	return s;
}

#endif // EXCEPTION_H
