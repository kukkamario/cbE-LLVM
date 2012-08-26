#include "funcdef.h"
RT_FUNC(mStringNull, R_STRING, P_VOID);

RT_FUNC(mStringSet, R_VOID, P_STRING_POINTER(dest), P_STRING(src));

RT_FUNC(mStringConstructEmpty, R_VOID, P_STRING_POINTER(str));

RT_FUNC(mCreateStringFromCharArray, R_STRING, P_CHAR_ARRAY(str));

RT_FUNC(mStringFromInt, R_STRING, P_INT(i));

RT_FUNC(mStringFromFloat, R_STRING, P_FLOAT(f));

RT_FUNC(mStringToInt, R_INT, P_STRING(str));

RT_FUNC(mStringToFloat, R_FLOAT, P_STRING(str));

RT_FUNC(mStringAdd, R_STRING, P_STRING(s1), P_STRING(s2));

RT_FUNC(mStringToBool, R_BOOL, P_STRING(str));

RT_FUNC(mStringEqual, R_BOOL, P_STRING(s1), P_STRING(s2));

RT_FUNC(mStringNotEqual, R_BOOL, P_STRING(s1), P_STRING(s2));

RT_FUNC(mStringLess, R_BOOL, P_STRING(s1), P_STRING(s2));

RT_FUNC(mStringGreater, R_BOOL, P_STRING(s1), P_STRING(s2));

RT_FUNC(mStringGreaterOrEqual, R_BOOL, P_STRING(s1), P_STRING(s2));

RT_FUNC(mStringLessOrEqual, R_BOOL, P_STRING(s1), P_STRING(s2));

RT_FUNC(mFunctionChr, R_STRING, P_INT(c));
