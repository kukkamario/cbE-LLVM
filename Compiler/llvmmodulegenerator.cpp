#include "llvmmodulegenerator.h"
#include "stackvalue.h"
#include "runtimefunction.h"
#include "cbfunction.h"
static LLVMModuleGenerator *mInstance;
LLVMModuleGenerator::LLVMModuleGenerator()
{
	mInstance = this;
}

LLVMModuleGenerator *LLVMModuleGenerator::instance() {
	return mInstance;
}

void LLVMModuleGenerator::separateFunctions(const ByteCode & bc) {
	ByteCode byteCode;
	for (ByteCode::const_iterator i = bc.begin(); i != bc.end(); i++) {

	}
}

Module *LLVMModuleGenerator::generate(ByteCode &bc) {
	//Creating module
	Module *mod = new Module("cb-bytecode-module",getGlobalContext());
	mod->setDataLayout("e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128");

	//Target triple
	mod->setTargetTriple("i686-pc-win32");

	//Common types
	IntegerType *intType = IntegerType::get(mod->getContext(), 32);
	IntegerType *byteType = IntegerType::get(mod->getContext(), 8);
	PointerType *bytePointer = PointerType::get(byteType, 0);
	PointerType *bytePointerPointer = PointerType::get(bytePointer, 0);
	Type *voidType = Type::getVoidTy(mod->getContext());

	ConstantInt *constIntZero = ConstantInt::get(IntegerType::get(mod->getContext(), 32), 0);

	vector<Type*> commandPrintArgs;
	commandPrintArgs.push_back(intType);
	commandPrintI = new RuntimeFunction("commandPrintI", voidType, commandPrintArgs);
	commandPrintI->appendToModule(mod);

	vector<Type*> noArgs;
	functionTimer = new RuntimeFunction("functionTimer", intType, noArgs);
	functionTimer->appendToModule(mod);

	commandEnd = new RuntimeFunction("commandEnd", voidType, noArgs);
	commandEnd->appendToModule(mod);


	vector<Type*> mainFuncParams;
	mainFuncParams.push_back(intType);
	mainFuncParams.push_back(bytePointerPointer);
	//CB main
	FunctionType *cbRuntimeMainFuncType = FunctionType::get(voidType, mainFuncParams, false);
	cbRuntimeMain = Function::Create(cbRuntimeMainFuncType, GlobalValue::ExternalLinkage, "cbRuntimeMain", mod);
	cbRuntimeMain->setCallingConv(CallingConv::C);

	//Main function
	FunctionType *mainFuncType = FunctionType::get(intType, mainFuncParams, false);
	Function *mainFunc = Function::Create(mainFuncType, GlobalValue::ExternalLinkage, "main", mod);

	CBFunction cbMain(bc.begin(), bc.end(), mainFunc, mod, true);
	cbMain.parse();

	/*FunctionPassManager passManager(mod);
	passManager.add(createBasicAliasAnalysisPass());
	passManager.add(createInstructionCombiningPass());
	passManager.add(createReassociatePass());
	passManager.add(createGVNPass());
	passManager.add(createCFGSimplificationPass());
	passManager.doInitialization();
	passManager.run(*cbMainFunction); */
	return mod;
}

