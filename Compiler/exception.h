#ifndef EXCEPTION_H
#define EXCEPTION_H
#include "precompiled.h"
class Exception
{
	public:
		enum Type {
			Cast,
			Function,
			Unimplemented,
			Unknown
		};

		Exception(Type t, const string &m);
		Type type() const {return mType;}
		const string &msg() const{return mMsg;}
		int32_t index() const {return mByteCodeIndex; }
		static int32_t mGlobalIndex;
	private:
		string mMsg;
		Type mType;
		int32_t mByteCodeIndex;
};

inline ostream &operator << (ostream & s, const Exception &i) {
	const char *exceptionNames[] = {
		"Cast",
		"Function",
		"Unimplemented feature",
		"Unknown"
	};
	s << "[" << i.index() <<"] " << exceptionNames[(int)i.type()] << " exception: " << i.msg();
	return s;
}

#endif // EXCEPTION_H
