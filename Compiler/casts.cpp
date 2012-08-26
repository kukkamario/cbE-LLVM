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
			case StackValue::String: {
				if (val.mString == 0) {
					AllocaInst *str = b->CreateAlloca(String::mStructType);
					b->CreateCall(String::mStringNull, str);
					return str;
				}
				AllocaInst *str = b->CreateAlloca(String::mStructType);
				Value *charArrayPtr = b->CreateBitCast(val.mValue, b->getInt8Ty()->getPointerTo());
				b->CreateCall2(String::mCreateStringFromCharArray, str, charArrayPtr);
				return str;
			}
			case StackValue::Int: {
				AllocaInst *str = b->CreateAlloca(String::mStructType);
				b->CreateCall2(String::mStringFromInt, str, val.mValue);
				return str;
			}
			case StackValue::Float: {
				AllocaInst *str = b->CreateAlloca(String::mStructType);
				b->CreateCall2(String::mStringFromFloat, str, val.mValue);
				return str;
			}
			default:
				cerr << "Wtf error 1: Cast::toString\n";
				return 0;
		}
	}
	switch (val.mType) {
		case StackValue::String:
			return val.mValue;
		case StackValue::Bool: {
			AllocaInst *str = b->CreateAlloca(String::mStructType);
			b->CreateCall2(String::mStringFromInt, str, toBool(b, val));
			return str;
		}
		case StackValue::Int: {
			AllocaInst *str = b->CreateAlloca(String::mStructType);
			b->CreateCall2(String::mStringFromInt, str, val.mValue);
			return str;
		}
		case StackValue::Float: {
			AllocaInst *str = b->CreateAlloca(String::mStructType);
			b->CreateCall2(String::mStringFromFloat, str, val.mValue);
			return str;
		}
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
				if (val.mString == 0) return b->getInt32(0);
				try {
					return b->getInt32(boost::lexical_cast<int32_t>(*val.mString));
				}
				catch (boost::bad_lexical_cast &) {
					throw Exception(Exception::Cast, "\"" + *val.mString + "\" cannot be converted to integer");
				};
				return 0;
			default:
				cerr << "Wtf error 1: Cast::toInt\n";
				return 0;
		}
	}
	switch (val.mType) {
		case StackValue::Bool:
			return b->CreateIntCast(val.mValue, b->getInt32Ty(), false);
		case StackValue::Int:
			return val.mValue;
		case StackValue::Float:
			return b->CreateFPToSI(val.mValue, Type::getInt32Ty(b->getContext()));
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
				return ConstantFP::get(b->getFloatTy(), (float)val.mInt);
			case StackValue::Float:
				return ConstantFP::get(b->getFloatTy(), val.mFloat);
			case StackValue::String:
				if (val.mString == 0) return ConstantFP::get(b->getFloatTy(), 0);
				try {
					return ConstantFP::get(b->getContext(), APFloat(boost::lexical_cast<float>(*val.mString)));
				}
				catch (boost::bad_lexical_cast &) {
					throw Exception(Exception::Cast, "\"" + *val.mString + "\" cannot be converted to float");
				};
				return 0;
			default:
				cerr << "Wtf error 1: Cast::toFloat\n";
				return 0;
		}
	}
	switch (val.mType) {
		case StackValue::Bool:
			return b->CreateSIToFP(val.mValue, b->getFloatTy());
		case StackValue::Int:
			return b->CreateSIToFP(val.mValue, b->getFloatTy());
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
	if (val.mConstant) {
		switch(val.mType) {
			case StackValue::Int:
				return b->getInt16(val.mInt);
			case StackValue::Float:
				return b->getInt16((uint16_t)(val.mFloat + 0.5f));
			case StackValue::String:
				if (val.mString == 0) return b->getInt32(0);
				try {
					return b->getInt16(boost::lexical_cast<uint16_t>(*val.mString));
				}
				catch (boost::bad_lexical_cast &) {
					throw Exception(Exception::Cast, "\"" + *val.mString + "\" cannot be converted to integer");
				};
				return 0;
			default:
				cerr << "Wtf error 1: Cast::toShort\n";
				return 0;
		}
	}
	switch (val.mType) {
		case StackValue::Bool:
		case StackValue::Int:
			return b->CreateIntCast(val.mValue, b->getInt16Ty(), false);
		case StackValue::Float:
			return b->CreateFPToUI(val.mValue, Type::getInt16Ty(b->getContext()));
		case StackValue::String:
			return b->CreateIntCast(b->CreateCall(String::mStringToInt, val.mValue), b->getInt16Ty(), false);
		default:
			cerr << "Wtf error 2: Cast::toShort\n";
			return 0;
	}
}

Value *toByte(IRBuilder<> *b, const StackValue &val) {
	if (val.mConstant) {
		switch(val.mType) {
			case StackValue::Int:
				return b->getInt8(val.mInt);
			case StackValue::Float:
				return b->getInt8((uint8_t)(val.mFloat + 0.5f));
			case StackValue::String:
				if (val.mString == 0) return b->getInt(0);
				try {
					return b->getInt8(boost::lexical_cast<uint8_t>(*val.mString));
				}
				catch (boost::bad_lexical_cast &) {
					throw Exception(Exception::Cast, "\"" + *val.mString + "\" cannot be converted to integer");
				};
				return 0;
			default:
				cerr << "Wtf error 1: Cast::toShort\n";
				return 0;
		}
	}
	switch (val.mType) {
		case StackValue::Bool:
		case StackValue::Int:
			return b->CreateIntCast(val.mValue, b->getInt8Ty(), false);
		case StackValue::Float:
			return b->CreateFPToUI(val.mValue, b->getInt8Ty());
		case StackValue::String:
			return b->CreateIntCast(b->CreateCall(String::mStringToInt, val.mValue), b->getInt8Ty(), false);
		default:
			cerr << "Wtf error 2: Cast::toByte\n";
			return 0;
	}
}

Value *toBool(IRBuilder<> *b, const StackValue &val) {
	if (val.mConstant) {
		switch (val.mType) {
			case StackValue::String:
				if (val.mString == 0) return b->getInt1(false);
				return b->getInt1(!val.mString->empty());
			case StackValue::Int:
				return b->getInt1(val.mInt != 0);
			case StackValue::Float:
				return b->getInt1(val.mFloat != 0.0f);
			default:
				cerr << "Wtf error 1: Cast::toBool\n";
				return 0;
		}
	}
	switch (val.mType) {
		case StackValue::String:
			return b->CreateCall(String::mStringToBool, val.mValue);
		case StackValue::Float: //TODO: Check...
			return b->CreateFCmpUNE(val.mValue, ConstantFP::get(b->getFloatTy(), 0.0));
		case StackValue::Bool:
			return val.mValue;
		case StackValue::Int:
			return b->CreateICmpNE(val.mValue,ConstantInt::get(b->getInt32Ty(), 0));
		default:
			cerr << "Wtf error 2: Cast::toBool\n";
			return 0;
	}
}

} //namespace Cast
