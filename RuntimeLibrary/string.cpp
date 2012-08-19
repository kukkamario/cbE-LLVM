#include "string.h"
#include "isstring.h"


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
