#include "exception.h"

Exception::Exception(Type t, const string &m):
	mMsg(m),
	mType(t)
{
}
