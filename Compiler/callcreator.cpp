#include "callcreator.h"
#include "exception.h"
#include "casts.h"
#include "rtstring.h"
#include <stdarg.h>

#define MAX_COST 100000
int32_t costTable[] = {
	0, 1, 100, 0, MAX_COST, MAX_COST, //Int
	2, 0, 100, 2, MAX_COST, MAX_COST, //Float
	1000, 1000, 0, 2, MAX_COST, MAX_COST, //String
	0, 1, 100, 0, MAX_COST, MAX_COST, //Bool
	MAX_COST, MAX_COST, MAX_COST, MAX_COST, 0, MAX_COST, //TypePtr
	MAX_COST, MAX_COST, MAX_COST, MAX_COST, MAX_COST, MAX_COST //Void
};
int32_t getCost(StackValue::Type from, StackValue::Type to) {
	int fromI = (int)from;
	int toI = (int)to;
	if (fromI < 0 || fromI > StackValue::Void || toI < 0 || toI > StackValue::Void ) {
		return MAX_COST;
	}
	return costTable[fromI * 6 + toI];
}

struct RuntimeFunction {
		Function *mFunc;
		StackValue::Type mRetType;
		vector<StackValue::Type> mParamTypes;
};

StackValue createCall_(IRBuilder<> *b, int32_t argNum, ...) {
	va_list va;
	va_start(va, argNum);
	vector<StackValue> params;
	for (int i = 0; i < argNum; i++) {
		params.push_back(va_arg(va, StackValue));
	}

	vector<RuntimeFunction> functions;
	Function *func;
	while ((func = va_arg(va, Function*)) != 0) {
		if (func->arg_size() != argNum) {
			throw Exception(Exception::Function, "Invalid createCall: " + func->getName().str() + " parametre count isn't " + boost::lexical_cast<string>(argNum));
		}
		RuntimeFunction rtFunc;
		rtFunc.mFunc = func;
		rtFunc.mRetType = StackValue::Void;
		Function::arg_iterator arg_i = func->arg_begin();
		if (arg_i != func->arg_end()) {
			Argument *arg = arg_i;
			if (arg->getType() == String::mStructType->getPointerTo(0) && arg->hasStructRetAttr()) {
				rtFunc.mRetType = StackValue::String;
				arg_i++;
			}
			while (arg_i != func->arg_end()) {
				Argument *arg = arg_i;
				if (arg->getType() == b->getInt32Ty()) {
					rtFunc.mParamTypes.push_back(StackValue::Int);
				}
				else if (arg->getType() == b->getFloatTy()) {
					rtFunc.mParamTypes.push_back(StackValue::Float);
				}
				else if (arg->getType() == String::mStructType->getPointerTo(0)) {
					rtFunc.mParamTypes.push_back(StackValue::String);
				}
				else {
					throw Exception(Exception::Function, "Invalid createCall: " + func->getName().str() + ": Unknown parametre type");
				}
				arg_i++;
			}
		}
		if (rtFunc.mRetType == StackValue::Void) {
			if (func->getReturnType() == b->getInt32Ty()) {
				rtFunc.mRetType = StackValue::Int;
			}
			else if (func->getReturnType() == b->getFloatTy()) {
				rtFunc.mRetType = StackValue::Float;
			}
			else if (func->getReturnType() == b->getVoidTy()) {
				rtFunc.mRetType = StackValue::Void;
			}
			else {
				throw Exception(Exception::Function, "Invalid createCall: " + func->getName().str() + ": Unknown return type");
			}
		}
		functions.push_back(rtFunc);
	}

	if (functions.size() == 0) {
		throw Exception(Exception::Function, "Invalid createCall: No function pointers");
	}

	RuntimeFunction rtFunc;
	if (functions.size() == 1) {
		rtFunc = functions.front();
	}
	else {
		int32_t bestCost = MAX_COST;
		RuntimeFunction bestFunction;
		for (vector<RuntimeFunction>::iterator funcI = functions.begin(); funcI != functions.end(); funcI++) {
			int32_t totalCost = 0;
			rtFunc = *funcI;
			vector<StackValue::Type>::iterator argTypeI = rtFunc.mParamTypes.begin();
			for (vector<StackValue>::iterator i = params.begin(); i != params.end(); i++) {
				totalCost += getCost(i->mType,*argTypeI);
				argTypeI++;
			}
			if (totalCost < bestCost) {
				bestFunction = rtFunc;
				bestCost = totalCost;
			}
		}

		if (bestCost >= MAX_COST) {
			throw Exception(Exception::Function, "Can't find function with given arguments");
		}

		rtFunc = bestFunction;
	}
	vector<Value*> paramValues;
	if (rtFunc.mRetType == StackValue::String) {
		paramValues.push_back(b->CreateAlloca(String::mStructType));
	}
	vector<StackValue>::iterator pI = params.begin();
	for (vector<StackValue::Type>::iterator i = rtFunc.mParamTypes.begin(); i != rtFunc.mParamTypes.end(); i++) {
		if (*i == StackValue::Float) {
			paramValues.push_back(Cast::toFloat(b, *pI));
		}
		else if( *i == StackValue::Int) {
			paramValues.push_back(Cast::toInt(b, *pI));
		}
		else if ( *i == StackValue::String) {
			paramValues.push_back(Cast::toString(b, *pI));
		}
		pI++;
	}

	StackValue ret;
	ret.mConstant = false;
	if (rtFunc.mRetType == StackValue::String) {
		b->CreateCall(rtFunc.mFunc, paramValues);
		ret.mValue = paramValues.front();
	}
	else {
		ret.mValue = b->CreateCall(rtFunc.mFunc, paramValues);
	}
	ret.mType = rtFunc.mRetType;
	return ret;
}

