#include "math.h"
#include <math.h>

RT_FUNC(mFunctionRand, R_INT, P_INT(min), P_INT(max)) {
	return min + (rand() % (max - min));
}
