#include "funcdef.h"

RT_FUNC(mFunctionTimer, R_INT, P_VOID);

RT_FUNC(mCommandEnd, R_VOID, P_VOID);

RT_FUNC(mCommandPrintI, R_VOID, P_INT(i));
RT_FUNC(mCommandPrintF, R_VOID, P_FLOAT(f));
RT_FUNC(mCommandPrintS, R_VOID, P_STRING(s));
