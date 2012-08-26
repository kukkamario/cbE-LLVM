#include "cbfunction.h"
#include "stackvalue.h"
#include "llvmmodulegenerator.h"
#include "casts.h"
#include "operations.h"
#include "exception.h"
#include "bytecode.h"
#include "rtstring.h"
#include "rtsystem.h"
#include "callcreator.h"
#include "rtmath.h"

CBFunction::CBFunction(const ByteCode *bc, ByteCode::iterator begin, ByteCode::iterator end, Function *func, Module *mod, bool isMain) :
	mBCBegin(begin),
	mBCEnd(end),
	mFunction(func),
	mIsMain(isMain),
	mModule(mod),
	mByteCode(bc)
{
}

bool CBFunction::parse() {
	LLVMModuleGenerator *modGen = LLVMModuleGenerator::instance();
	LLVMContext &context = mFunction->getContext();
	IntegerType *intType = IntegerType::get(context, 32);
	IntegerType *byteType = IntegerType::get(context, 8);
	PointerType *bytePointer = PointerType::get(byteType, 0);
	PointerType *bytePointerPointer = PointerType::get(bytePointer, 0);
	Type *voidType = Type::getVoidTy(context);

	ConstantInt *constIntZero = ConstantInt::get(IntegerType::get(context, 32), 0);
	Constant *constFloatZero = ConstantFP::get(Type::getFloatTy(context), 0.0);
	//First pass
	set<int32_t> blockBorders;
	int32_t index = 0;
	for (ByteCode::iterator i = mBCBegin; i != mBCEnd; i++) {
		CBInstruction inst = *i;
		switch (inst.mOpCode) {
			case OCJump:
				blockBorders.insert(i->mData);
				blockBorders.insert(index + 1);
				break;
			case OCCommand:
				switch (inst.mData) {
					case 12: //Goto
						i++;
						index++;
						blockBorders.insert(i->mData);
						blockBorders.insert(index + 1);
						break;
				}
				break;
		}
		index++;
	}

	set<int32_t>::iterator borderIterator = blockBorders.begin();
	if (borderIterator != blockBorders.end()) {
		if (*borderIterator == 1) {
			borderIterator++;
		}
	}

	index = 0;
	map<int32_t, BasicBlock*> basicBlocks;
	BasicBlock *currentBlock = BasicBlock::Create(mFunction->getContext(), "", mFunction);
	if (mIsMain) {
		Function::arg_iterator arg_i = mFunction->arg_begin();
		Value* mainArgc = arg_i++;
		mainArgc->setName("argc");
		Value* mainArgv = arg_i++;
		mainArgv->setName("argv");
		vector<Value*> args;
		args.push_back(mainArgc);
		args.push_back(mainArgv);
		CallInst::Create(modGen->cbRuntimeMain, args, "", currentBlock);
	}
	basicBlocks[1] = currentBlock;

	for (ByteCode::iterator i = mBCBegin; i != mBCEnd; i++) {
		if (borderIterator != blockBorders.end() && *borderIterator == index) {
			currentBlock = BasicBlock::Create(mFunction->getContext(), "", mFunction);
			basicBlocks[index] = currentBlock;
			borderIterator++;
		}
		i->mBasicBlock = currentBlock;
		index++;
	}

	index = 0;
	bool skipToBlockChange = false;
	int32_t skipBr = 0;
	IRBuilder<> *builder = 0;
	currentBlock = 0;
	stack<StackValue> varStack;
	StackValue stackValue;
	for (ByteCode::iterator i = mBCBegin; i != mBCEnd; i++) {
		Exception::mGlobalIndex = index;
		CBInstruction &inst = *i;
		if (inst.mBasicBlock != currentBlock) {
			if (builder) {
				if (skipBr != index && !skipToBlockChange) builder->CreateBr(inst.mBasicBlock);
				delete builder;
			}
			currentBlock = inst.mBasicBlock;
			builder = new IRBuilder<>(currentBlock);
			skipToBlockChange = false;
		}
		if (skipToBlockChange) continue;
		switch (inst.mOpCode) {
			case OCPushInt: {
				stackValue.mType = StackValue::Int;
				stackValue.mValue = ConstantInt::get(intType, inst.mData, true);
				stackValue.mConstant = true;
				stackValue.mInt = inst.mData;
				varStack.push(stackValue);
				break;
			}
			case OCPushSomething: {
				int32_t type = varStack.top().mInt; varStack.pop();
				switch( type) {
					case 2: //Float
						stackValue.mFloat = inst.mFData;
						stackValue.mType = StackValue::Float;
						stackValue.mValue = ConstantFP::get(builder->getFloatTy(), inst.mFData);
						break;
					case 5: {//String
						if (inst.mData == 0) {
							stackValue.mString = 0;
							stackValue.mValue = 0;
						}
						else {
							const StringPool::ConstString &str = mByteCode->stringPool().get(inst.mData - 1);
							stackValue.mValue = str.mGlobalVariable;
							stackValue.mString = &str.mString;
						}
						stackValue.mType = StackValue::String;
						break;
					}
					default:
						throw Exception(Exception::Unimplemented, "Unimplemented PushSomething");
				}
				stackValue.mConstant = true;
				varStack.push(stackValue);
				break;
			}
			case OCSetInt: {
				stackValue = varStack.top(); varStack.pop();
				builder->CreateStore(Cast::toInt(builder, stackValue), mIntVars[inst.mData - 1].mAllocaInst,false);
				break;
			}
			case  OCPushVariable: {
				stackValue = varStack.top(); varStack.pop();//Type
				switch(stackValue.mInt) {
					case 1: //Int
						stackValue.mValue = builder->CreateLoad(mIntVars[inst.mData - 1].mAllocaInst, false);
						stackValue.mType = StackValue::Int;
						break;
					case 2: //Float
						stackValue.mValue = builder->CreateLoad(mFloatVars[inst.mData - 1].mAllocaInst, false);
						stackValue.mType = StackValue::Float;
						break;
					case 5: //String
						stackValue.mValue = mStringVars[inst.mData -1].mAllocaInst;
						stackValue.mType = StackValue::String;
						break;
				}
				stackValue.mConstant = false;
				varStack.push(stackValue);
				break;
			}
			case OCSetFloat: {
				stackValue = varStack.top(); varStack.pop();
				builder->CreateStore(Cast::toFloat(builder, stackValue), mFloatVars[inst.mData - 1].mAllocaInst, false);
				break;
			}
			case OCIncVar: {
				LoadInst *var = builder->CreateLoad(mIntVars[inst.mData - 1].mAllocaInst, false);
				Value *result = builder->CreateAdd(var, builder->getInt32(1));
				builder->CreateStore(result, mIntVars[inst.mData - 1].mAllocaInst, false);
				break;
			}
			case OCOperation: {
				switch(inst.mData) {
					case 1: { //unary minus
						StackValue val = varStack.top(); varStack.pop();
						stackValue = Operation::unaryMinus(builder, val);
						varStack.push(stackValue);
						break;
					}
					case 2: { //unary plus
						StackValue val = varStack.top(); varStack.pop();
						//stackValue = Operation::unaryPlus(builder, val);
						varStack.push(stackValue);
						break;
					}
					case 3: {
						StackValue right = varStack.top(); varStack.pop();
						StackValue left = varStack.top(); varStack.pop();
						//stackValue = Operation::power(builder, left, right);
						varStack.push(stackValue);
						break;
					}
					case 4: {//Addition
						StackValue right = varStack.top(); varStack.pop();
						StackValue left = varStack.top(); varStack.pop();
						stackValue = Operation::add(builder, left, right);
						varStack.push(stackValue);
						break;
					}
					case 5:{//Subtraction
						StackValue right = varStack.top(); varStack.pop();
						StackValue left = varStack.top(); varStack.pop();
						stackValue = Operation::subtract(builder, left, right);
						varStack.push(stackValue);
						break;
					}
					case 6: { //multiplication
						StackValue right = varStack.top(); varStack.pop();
						StackValue left = varStack.top(); varStack.pop();
						stackValue = Operation::multiply(builder, left, right);
						varStack.push(stackValue);
						break;
					}
					case 7: { //division
						StackValue right = varStack.top(); varStack.pop();
						StackValue left = varStack.top(); varStack.pop();
						stackValue = Operation::divide(builder, left, right);
						varStack.push(stackValue);
						break;
					}
					case 8: { //modulo
						StackValue right = varStack.top(); varStack.pop();
						StackValue left = varStack.top(); varStack.pop();
						//stackValue = Operation::modulo(builder, left, right);
						varStack.push(stackValue);
						break;
					}
					case 9: { //shl
						StackValue right = varStack.top(); varStack.pop();
						StackValue left = varStack.top(); varStack.pop();
						//stackValue = Operation::shl(builder, left, right);
						varStack.push(stackValue);
						break;
					}
					case 10: { //shr
						StackValue right = varStack.top(); varStack.pop();
						StackValue left = varStack.top(); varStack.pop();
						//stackValue = Operation::shr(builder, left, right);
						varStack.push(stackValue);
						break;
					}
					case 11: { //sar
						StackValue right = varStack.top(); varStack.pop();
						StackValue left = varStack.top(); varStack.pop();
						//stackValue = Operation::sar(builder, left, right);
						varStack.push(stackValue);
						break;
					}
					case 12: {//less than
						StackValue right = varStack.top(); varStack.pop();
						StackValue left = varStack.top(); varStack.pop();
						stackValue = Operation::less(builder, left, right);
						varStack.push(stackValue);
						break;
					}
					case 13: {//greater than
						StackValue right = varStack.top(); varStack.pop();
						StackValue left = varStack.top(); varStack.pop();
						stackValue = Operation::greater(builder, left, right);
						varStack.push(stackValue);
						break;
					}
					case 14: { //equal
						StackValue right = varStack.top(); varStack.pop();
						StackValue left = varStack.top(); varStack.pop();
						stackValue = Operation::equal(builder, left, right);
						varStack.push(stackValue);
						break;
					}
					case 15: { //equal
						StackValue right = varStack.top(); varStack.pop();
						StackValue left = varStack.top(); varStack.pop();
						stackValue = Operation::equal(builder, left, right);
						varStack.push(stackValue);
						break;
					}
					case 16: {//lessThanOrEqual
						StackValue right = varStack.top(); varStack.pop();
						StackValue left = varStack.top(); varStack.pop();
						stackValue = Operation::lessOrEqual(builder, left, right);
						varStack.push(stackValue);
						break;
					}
					case 17: {//lessThanOrEqual
						StackValue right = varStack.top(); varStack.pop();
						StackValue left = varStack.top(); varStack.pop();
						stackValue = Operation::greaterOrEqual(builder, left, right);
						varStack.push(stackValue);
						break;
					}
					case 18: { //and
						StackValue right = varStack.top(); varStack.pop();
						StackValue left = varStack.top(); varStack.pop();
						//stackValue = Operation::and(builder, left, right);
						varStack.push(stackValue);
						break;
					}
					case 19: { //or
						StackValue right = varStack.top(); varStack.pop();
						StackValue left = varStack.top(); varStack.pop();
						//stackValue = Operation::or(builder, left, right);
						varStack.push(stackValue);
						break;
					}
					case 20: { //xor
						StackValue right = varStack.top(); varStack.pop();
						StackValue left = varStack.top(); varStack.pop();
						//stackValue = Operation::xor(builder, left, right);
						varStack.push(stackValue);
						break;
					}
					case 21: { //not
						StackValue val = varStack.top(); varStack.pop();
						//stackValue = Operation::not(builder, val);
						varStack.push(stackValue);
						break;
					}
				}
				break;
			}
			case OCJump: {
				stackValue = varStack.top(); varStack.pop();
				builder->CreateCondBr(Cast::toBool(builder, stackValue), basicBlocks[index + 1], basicBlocks[inst.mData]);
				skipBr = index + 1;
			}
			case OCCommand: {
				switch (inst.mData) {
					case 12: //Goto
						i++;
						index++;
						assert(basicBlocks[i->mData] != 0);
						builder->CreateBr(basicBlocks[i->mData]);
						skipBr = index + 1;
						break;
					case 69:
						createCall(builder, 0, System::mCommandEnd);
						skipToBlockChange = true;
						builder->CreateUnreachable();
						break;
					case 77: {//SetVariable
						int32_t type = varStack.top().mInt; varStack.pop();
						int32_t place = varStack.top().mInt; varStack.pop();
						StackValue val = varStack.top(); varStack.pop();
						switch (type) {
							case 1: //String
								builder->CreateCall2(String::mStringSet,
													 mStringVars[place - 1].mAllocaInst,
													 Cast::toString(builder, val));
								break;
						}
					}
					case 97: //Array numbers
						varStack.pop(); //Short
						varStack.pop(); //Byte
						varStack.pop(); //String
						varStack.pop(); //Float
						varStack.pop(); //Int
						break;
					case 98: //Global variables
						varStack.pop(); //Short
						varStack.pop(); //Byte
						varStack.pop(); //String
						varStack.pop(); //Float
						varStack.pop(); //Int
						break;
					case 99: {
							int32_t typePtrs = varStack.top().mInt; varStack.pop();//TypePtrs
							int32_t shorts = varStack.top().mInt; varStack.pop();//Short
							int32_t bytes = varStack.top().mInt; varStack.pop();//Byte
							int32_t strings = varStack.top().mInt; varStack.pop();//String
							int32_t floats = varStack.top().mInt; varStack.pop();//Float
							int32_t ints = varStack.top().mInt; varStack.pop();//Int
							if (ints > 0) {
								mIntVars = new Variable[ints];
								for (int32_t i = 0; i < ints; i++) {
									mIntVars[i].mAllocaInst = builder->CreateAlloca(intType);
									builder->CreateStore(constIntZero, mIntVars[i].mAllocaInst, false);
								}
							}
							Type *floatTy = builder->getFloatTy();
							if (floats > 0) {
								mFloatVars = new Variable[floats];
								for (int32_t i = 0; i < floats; i++) {
									mFloatVars[i].mAllocaInst = builder->CreateAlloca(floatTy);
									builder->CreateStore(constFloatZero, mFloatVars[i].mAllocaInst, false);
								}
							}
							if (strings > 0) {
								mStringVars = new Variable[strings];
								for (int32_t i = 0; i < strings; i++) {
									mStringVars[i].mAllocaInst = builder->CreateAlloca(String::mStructType);
									builder->CreateCall(String::mStringConstructEmpty, mStringVars[i].mAllocaInst);
								}
							}
							break;
						}
					case 207: {//Print
						stackValue = varStack.top(); varStack.pop();
						createCall(builder, 1, stackValue, System::mCommandPrintI, System::mCommandPrintF, System::mCommandPrintS);
						break;
					}
				}
				break;
			}
			case OCFunction: {
				switch (inst.mData) {
					case 117: {//Rand
						StackValue max = varStack.top(); varStack.pop();
						StackValue min = varStack.top(); varStack.pop();
						stackValue = createCall(builder, 2, min, max, Math::mFunctionRand);
						varStack.push(stackValue);
						break;
					}
					case 161: {//Chr
						StackValue c = varStack.top(); varStack.pop();
						stackValue = createCall(builder, 1, c, String::mFunctionChr);
						varStack.push(stackValue);
						break;
					}
					case 422: //Timer
						stackValue = createCall(builder, 0, System::mFunctionTimer);
						varStack.push(stackValue);
						break;
				}
				break;
			}
		}
		index++;
	}
	//builder->CreateRet(constIntZero);

	FunctionPassManager passManager(mModule);
	//passManager.add(createOptimizePHIsPass());
	//passManager.add(createPeepholeOptimizerPass());
	//passManager.add(createIfConverterPass());
	//passManager.add(createBranchFoldingPass(true));
	//passManager.add(createLowerSwitchPass());
	passManager.add(createInstructionCombiningPass());
	passManager.run(*mFunction);
	return true;
}

