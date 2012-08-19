#include "cbfunction.h"
#include "stackvalue.h"
#include "llvmmodulegenerator.h"


CBFunction::CBFunction(ByteCode::iterator begin, ByteCode::iterator end, Function *func, Module *mod, bool isMain) :
	mBCBegin(begin),
	mBCEnd(end),
	mFunction(func),
	mIsMain(isMain),
	mModule(mod)
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
		CallInst *cbMainCall = CallInst::Create(modGen->cbRuntimeMain, args, "", currentBlock);
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
			case OCPushSomething:
				switch( inst.mData) {

				}

			case OCSetInt: {
				stackValue = varStack.top(); varStack.pop();
				builder->CreateStore(stackValue.mValue, mIntVars[inst.mData - 1].mAllocaInst,false);
				break;
			}
			case  OCPushVariable: {
				stackValue = varStack.top(); varStack.pop();//Type
				switch(stackValue.mInt) {
					case 1: //Int
						stackValue.mValue = builder->CreateLoad(mIntVars[inst.mData - 1].mAllocaInst, false);
						stackValue.mType = StackValue::Int;
				}
				stackValue.mConstant = false;
				varStack.push(stackValue);
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
					case 4: {//Addition
						StackValue right = varStack.top(); varStack.pop();
						StackValue left = varStack.top(); varStack.pop();
						stackValue.mValue = builder->CreateAdd(left.mValue, right.mValue);
						stackValue.mType = StackValue::Int;
						stackValue.mConstant = false;
						varStack.push(stackValue);
						break;
					}
					case 5:{//Subtraction
						StackValue right = varStack.top(); varStack.pop();
						StackValue left = varStack.top(); varStack.pop();
						stackValue.mValue = builder->CreateSub(left.mValue, right.mValue);
						stackValue.mType = StackValue::Int;
						stackValue.mConstant = false;
						varStack.push(stackValue);
						break;
					}
					case 16: {//lessThanOrEqual
						StackValue right = varStack.top(); varStack.pop();
						StackValue left = varStack.top(); varStack.pop();
						stackValue.mValue = builder->CreateICmpSLE(left.mValue, right.mValue);
						stackValue.mType = StackValue::Int;
						stackValue.mConstant = false;
						varStack.push(stackValue);
					}
				}
				break;
			}
			case OCJump: {
				stackValue = varStack.top(); varStack.pop();
				builder->CreateCondBr(stackValue.mValue, basicBlocks[index + 1], basicBlocks[inst.mData]);
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
						builder->CreateCall(modGen->commandEnd->function());
						skipToBlockChange = true;
						builder->CreateUnreachable();
						break;
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
							break;
						}
					case 207: {//Print
						stackValue = varStack.top(); varStack.pop();
						builder->CreateCall(modGen->commandPrintI->function(), stackValue.mValue);
						break;
					}
				}
				break;
			}
			case OCFunction: {
				switch (inst.mData) {
					case 422: //Timer
						stackValue.mValue = builder->CreateCall(modGen->functionTimer->function());
						stackValue.mType = StackValue::Int;
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

