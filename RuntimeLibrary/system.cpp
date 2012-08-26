#include "system.h"
#include <time.h>
#include <stdio.h>

RT_FUNC(mFunctionTimer, R_INT, P_VOID) {
	return clock();
}

RT_FUNC(mCommandEnd, R_VOID, P_VOID) {
	exit(0);
}

RT_FUNC(mCommandPrintI, R_VOID, P_INT(i)) {
	printf("%i\n", i);
}

RT_FUNC(mCommandPrintF, R_VOID, P_FLOAT(f)) {
	printf("%f\n", f);
}

RT_FUNC(mCommandPrintS, R_VOID, P_STRING(s)) {
	printf("%s\n", s.getRef().c_str());
}

extern "C" void cbRuntimeMain(int argc, char *argv[]) {
	srand(clock());
}
