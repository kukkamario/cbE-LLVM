#include "funcdef.h"
RT_FUNC(mStringFromCharArray, R_STRING, P_CHAR_ARRAY(str));
RT_FUNC(mStringFromInt, R_STRING, P_INT(i));
RT_FUNC(mStringFromFloat, R_STRING, P_FLOAT(f));
RT_FUNC(mStringToInt, R_INT, P_STRING(str));
RT_FUNC(mStringToFloat, R_FLOAT, P_STRING(str));
