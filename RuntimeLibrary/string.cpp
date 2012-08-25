#include "string.h"
#include <stdio.h>

RT_FUNC(mStringNull, R_STRING, P_VOID) {
	return ISString();
}

RT_FUNC(mStringSet, R_VOID, P_STRING_POINTER(dest), P_STRING(src)) {
	*dest = src;
}

RT_FUNC(mStringConstructEmpty, R_VOID, P_STRING_POINTER(str)) {
	ISString::construct(str);
}

RT_FUNC(mCreateStringFromCharArray, R_STRING, P_CHAR_ARRAY(str)) {
	return ISString(str);
}

RT_FUNC(mStringFromInt, R_STRING, P_INT(i)) {
	return ISString::fromInt(i);
}

RT_FUNC(mStringFromFloat, R_STRING, P_FLOAT(f)) {
	return ISString::fromFloat(f);
}

RT_FUNC(mStringToInt, R_INT, P_STRING(str)) {
	return str.toInt();
}

RT_FUNC(mStringToFloat, R_FLOAT, P_STRING(str)) {
	return str.toFloat();
}

RT_FUNC(mStringAdd, R_STRING, P_STRING(s1), P_STRING(s2)) {
	return s1 + s2;
}

RT_FUNC(mStringToBool, R_BOOL, P_STRING(str)) {
	return !str.empty();
}

RT_FUNC(mStringEqual, R_BOOL, P_STRING(s1), P_STRING(s2)) {
	return s1 == s2;
}

RT_FUNC(mStringNotEqual, R_BOOL, P_STRING(s1), P_STRING(s2)) {
	return s1 != s2;
}

RT_FUNC(mStringLess, R_BOOL, P_STRING(s1), P_STRING(s2)) {
	return s1 < s2;
}

RT_FUNC(mStringGreater, R_BOOL, P_STRING(s1), P_STRING(s2)) {
	return s1 > s2;
}

RT_FUNC(mStringGreaterOrEqual, R_BOOL, P_STRING(s1), P_STRING(s2)) {
	return s1 >= s2;
}

RT_FUNC(mStringLessOrEqual, R_BOOL, P_STRING(s1), P_STRING(s2)) {
	return s1 <= s2;
}
