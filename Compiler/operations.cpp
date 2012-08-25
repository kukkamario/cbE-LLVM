#include "operations.h"
#include "casts.h"
#include "rtstring.h"
#include "callcreator.h"
namespace Operation {
	StackValue add(IRBuilder<> *b, const StackValue &left, const StackValue &right) {
		if (left.mType == StackValue::String || right.mType == StackValue::String) {
			AllocaInst *retStr = b->CreateAlloca(String::mStructType);
			b->CreateCall3(String::mStringAdd, retStr, Cast::toString(b, left), Cast::toString(b, right));
			return StackValue(StackValue::String, retStr);
		}
		if (left.mType == StackValue::Float || right.mType == StackValue::Float) {
			return StackValue(StackValue::Float, b->CreateFAdd(Cast::toFloat(b, left), Cast::toFloat(b, right)));
		}
		return StackValue(StackValue::Int, b->CreateAdd(Cast::toInt(b, left), Cast::toInt(b, right)));
	}
	StackValue subtract(IRBuilder<> *b, const StackValue &left, const StackValue &right) {
		if (left.mType == StackValue::Float || right.mType == StackValue::Float) {
			return StackValue(StackValue::Float, b->CreateFSub(Cast::toFloat(b, left), Cast::toFloat(b, right)));
		}
		return StackValue(StackValue::Int, b->CreateSub(Cast::toInt(b, left), Cast::toInt(b, right)));
	}

	StackValue divide(IRBuilder<> *b, const StackValue &left, const StackValue &right) {
		if (left.mType == StackValue::Float || right.mType == StackValue::Float) {
			return StackValue(StackValue::Float, b->CreateFDiv(Cast::toFloat(b, left), Cast::toFloat(b, right)));
		}
		return StackValue(StackValue::Int, b->CreateSDiv(Cast::toInt(b, left), Cast::toInt(b, right)));
	}
	StackValue multiply(IRBuilder<> *b, const StackValue &left, const StackValue &right) {
		if (left.mType == StackValue::Float || right.mType == StackValue::Float) {
			return StackValue(StackValue::Float, b->CreateFMul(Cast::toFloat(b, left), Cast::toFloat(b, right)));
		}
		return StackValue(StackValue::Bool, b->CreateMul(Cast::toInt(b, left), Cast::toInt(b, right)));
	}

	StackValue unaryMinus(IRBuilder<> *b, const StackValue &value) {
		if (value.mType == StackValue::Float) {
			return StackValue(StackValue::Float, b->CreateFSub(ConstantFP::getZeroValueForNegation(b->getFloatTy()), Cast::toFloat(b, value)));
		}
		return StackValue(StackValue::Bool, b->CreateSub(ConstantInt::get(b->getInt32Ty(), 0), Cast::toInt(b, value)));
	}

	StackValue equal(IRBuilder<> *b, const StackValue &left, const StackValue &right) {
		if (left.mType == StackValue::String || right.mType == StackValue::String) {
			return createCall(b, 2, left, right, String::mStringEqual);
		}
		if (left.mType == StackValue::Float || right.mType == StackValue::Float) {
			return StackValue(StackValue::Bool, b->CreateFCmpOEQ(Cast::toFloat(b, left), Cast::toFloat(b, right)));
		}
		return StackValue(StackValue::Bool, b->CreateICmpEQ(Cast::toInt(b, left), Cast::toInt(b, right)));
	}

	StackValue notEqual(IRBuilder<> *b, const StackValue &left, const StackValue &right) {
		if (left.mType == StackValue::String || right.mType == StackValue::String) {
			return createCall(b, 2, left, right, String::mStringNotEqual);
		}
		if (left.mType == StackValue::Float || right.mType == StackValue::Float) {
			return StackValue(StackValue::Bool, b->CreateFCmpONE(Cast::toFloat(b, left), Cast::toFloat(b, right)));
		}
		return StackValue(StackValue::Bool, b->CreateICmpNE(Cast::toInt(b, left), Cast::toInt(b, right)));
	}


	StackValue greater(IRBuilder<> *b, const StackValue &left, const StackValue &right) {
		if (left.mType == StackValue::String || right.mType == StackValue::String) {
			return createCall(b, 2, left, right, String::mStringGreater);
		}
		if (left.mType == StackValue::Float || right.mType == StackValue::Float) {
			return StackValue(StackValue::Bool, b->CreateFCmpOGT(Cast::toFloat(b, left), Cast::toFloat(b, right)));
		}
		return StackValue(StackValue::Bool, b->CreateICmpSGT(Cast::toInt(b, left), Cast::toInt(b, right)));
	}

	StackValue less(IRBuilder<> *b, const StackValue &left, const StackValue &right) {
		if (left.mType == StackValue::String || right.mType == StackValue::String) {
			return createCall(b, 2, left, right, String::mStringLess);
		}
		if (left.mType == StackValue::Float || right.mType == StackValue::Float) {
			return StackValue(StackValue::Bool, b->CreateFCmpOLT(Cast::toFloat(b, left), Cast::toFloat(b, right)));
		}
		return StackValue(StackValue::Bool, b->CreateICmpSLT(Cast::toInt(b, left), Cast::toInt(b, right)));
	}

	StackValue greaterOrEqual(IRBuilder<> *b, const StackValue &left, const StackValue &right) {
		if (left.mType == StackValue::String || right.mType == StackValue::String) {
			return createCall(b, 2, left, right, String::mStringGreaterOrEqual);
		}
		if (left.mType == StackValue::Float || right.mType == StackValue::Float) {
			return StackValue(StackValue::Bool, b->CreateFCmpOGE(Cast::toFloat(b, left), Cast::toFloat(b, right)));
		}
		return StackValue(StackValue::Bool, b->CreateICmpSGE(Cast::toInt(b, left), Cast::toInt(b, right)));
	}

	StackValue lessOrEqual(IRBuilder<> *b, const StackValue &left, const StackValue &right) {
		if (left.mType == StackValue::String || right.mType == StackValue::String) {
			return createCall(b, 2, left, right, String::mStringLessOrEqual);
		}
		if (left.mType == StackValue::Float || right.mType == StackValue::Float) {
			return StackValue(StackValue::Bool, b->CreateFCmpOLE(Cast::toFloat(b, left), Cast::toFloat(b, right)));
		}
		return StackValue(StackValue::Bool, b->CreateICmpSLE(Cast::toInt(b, left), Cast::toInt(b, right)));
	}
}
