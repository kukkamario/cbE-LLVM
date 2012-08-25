#include "exception.h"
int32_t Exception::mGlobalIndex = 0;

Exception::Exception(Type t, const string &m):
	mMsg(m),
	mType(t),
	mByteCodeIndex(mGlobalIndex)
{

}
