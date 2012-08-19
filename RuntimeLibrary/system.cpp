#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <ctime>
extern "C" void commandPrintI(int i) {
	printf("%i\n", i);
}

extern "C" int32_t functionTimer() {
	return clock();
}

extern "C" void commandEnd() {
	exit(0);
}

extern "C" void cbRuntimeMain(int argc, char *argv[]) {
	printf("cbMain\n");
}
