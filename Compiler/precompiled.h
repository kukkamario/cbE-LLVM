#ifndef PRECOMP_H
#define PRECOMP_H
#include <string>
#include <list>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <stack>
#include <map>
#include <vector>
#include <stdint.h>

#include <llvm/LLVMContext.h>
#include <llvm/Module.h>
#include <llvm/DerivedTypes.h>
#include <llvm/Constants.h>
#include <llvm/GlobalVariable.h>
#include <llvm/Function.h>
#include <llvm/CallingConv.h>
#include <llvm/BasicBlock.h>
#include <llvm/Instructions.h>
#include <llvm/InlineAsm.h>
#include <llvm/Support/FormattedStream.h>
#include <llvm/Support/MathExtras.h>
#include <llvm/Pass.h>
#include <llvm/PassManager.h>
#include <llvm/PassManagers.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/Assembly/PrintModulePass.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/Support/Path.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/Mangler.h>
#include <llvm/ADT/Triple.h>
#include <llvm/Support/IRBuilder.h>
#include <llvm/Support/DynamicLibrary.h>
#include "llvm/LinkAllPasses.h"
#include <llvm/Linker.h>
#include <llvm/Bitcode/ReaderWriter.h>

#include <boost/lexical_cast.hpp>
using namespace std;
using namespace llvm;
#endif // PRECOMP_H
