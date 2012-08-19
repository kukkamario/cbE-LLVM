#include "casts.h"
#include "exception.h"
#include <llvm/Type.h>
#include "rtstring.h"

namespace Cast {

void init() {

}

Value *toString(IRBuilder<> *b, const StackValue &val) {
	if (val.mConstant) {
		switch (val.mType) {
			case StackValue::String:
				return b->CreateCall(String::mStringFromCharArray, val.mValue);
			case StackValue::Int:
				return b->CreateCall(String::mStringFromInt, val.mValue);
			case StackValue::Float:
				return b->CreateCall(String::mStringFromFloat, val.mValue);
			default:
				cerr << "Wtf error 1: Cast::toString\n";
				return 0;
		}
	}
	switch (val.mType) {
		case StackValue::String:
			return val.mValue;
		case StackValue::Float:
			return b->CreateCall(String::mStringFromFloat, val.mValue);
		case StackValue::Int:
			return b->CreateCall(String::mStringFromInt, val.mValue);
		default:
			cerr << "Wtf error 2: Cast::toString\n";
			return 0;
	}
}

Value *toInt(IRBuilder<> *b, const StackValue &val) {
	if (val.mConstant) {
		switch(val.mType) {
			case StackValue::Int:
				return b->getInt32(val.mInt);
			case StackValue::Float:
				return b->getInt32((int)(val.mFloat + 0.5f));
			case StackValue::String:
				try {
					return b->getInt32(boost::lexical_cast<int32_t>(*val.mString));
				}
				catch (boost::bad_lexical_cast &e) {
					throw Exception(Exception::Cast, "\"" + *val.mString + "\" cannot be converted to integer");
				};
				return 0;
			default:
				cerr << "Wtf error 1: Cast::toInt\n";
				return 0;
		}
	}
	switch (val.mType) {
		case StackValue::Int:
			return val.mValue;
		case StackValue::Float:
			return b->CreateFPCast(val.mValue, Type::getInt32Ty(b->getContext()));
		case StackValue::String:
			return b->CreateCall(String::mStringToInt, val.mValue);
		default:
			cerr << "Wtf error 2: Cast::toInt\n";
			return 0;
	}

}

Value *toFloat(IRBuilder<> *b, const StackValue &val) {
	if (val.mConstant) {
		switch(val.mType) {
			case StackValue::Int:
				return b->getInt32(val.mInt);
			case StackValue::Float:
				return b->getInt32((int)(val.mFloat + 0.5f));
			case StackValue::String:
				try {
					return ConstantFP::get(b->getContext(), APFloat(boost::lexical_cast<float>(*val.mString)));
				}
				catch (boost::bad_lexical_cast &e) {
					throw Exception(Exception::Cast, "\"" + *val.mString + "\" cannot be converted to float");
				};
				return 0;
			default:
				cerr << "Wtf error 1: Cast::toFloat\n";
				return 0;
		}
	}
	switch (val.mType) {
		case StackValue::Int:
			return b->CreateIntCast(val.mValue, b->getFloatTy(), true);
		case StackValue::Float:
			return val.mValue;
		case StackValue::String:
			return b->CreateCall(String::mStringToFloat, val.mValue);
		default:
			cerr << "Wtf error 2: Cast::toFloat\n";
			return 0;
	}
}

Value *toShort(IRBuilder<> *b, const StackValue &val) {
	return 0;
}

Value *toByte(IRBuilder<> *b, const StackValue &val) {
	return 0;
}

} //namespace Cast
