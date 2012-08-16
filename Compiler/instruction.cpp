#include "instruction.h"
const char* OpCodeNames[OpCodeCount] = {
	"Unknown",
	"PushInt",
	"SetInt",
	"SetFloat",
	"Command",
	"Data",
	"PushSomething",
	"Jump",
	"Operation",
	"Increase variable",
	"Increase global variable",
	"Push type member",
	"Push function pointer",
	"Push variable",
	"Function"
};

