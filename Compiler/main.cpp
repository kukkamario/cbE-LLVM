#include "precompiled.h"
#include <iostream>
#include "bytecodereader.h"
#include "llvmmodulegenerator.h"
#include "llvmlinker.h"
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/system_error.h>
#include <llvm/Assembly/Parser.h>
#include <llvm/Support/IRReader.h>
#include <llvm/Object/COFF.h>
#include <llvm/Object/ObjectFile.h>
/*
#include <llvm/MC/MCELFObjectWriter.h>
#include <llvm/MC/MCWinCOFFObjectWriter.h>
#include <llvm/MC/MCAsmInfoCOFF.h>
#include <llvm/MC/MCAssembler.h>
#include <llvm/MC/MCObjectWriter.h>*/

using namespace std;


int main(int argc, char *argv[]) {
	if (argc == 3) {
		ByteCodeReader reader;
		cout << "Reading bytecode...\n";
		if (reader.readCBExecutable(string(argv[1]))) {
			reader.byteCode().print();

			cout << "Initializing LLVM...\n";
			InitializeNativeTarget();

			PassRegistry *registry = PassRegistry::getPassRegistry();
			initializePrintModulePassPass(*registry);
			initializeCore(*registry);
			initializeTransformUtils(*registry);
			initializeScalarOpts(*registry);
			initializeInstCombine(*registry);
			initializeIPO(*registry);
			initializeInstrumentation(*registry);
			initializeAnalysis(*registry);
			initializeIPA(*registry);
			initializeLowerSwitchPass(*registry);
			initializeStripSymbolsPass(*registry);
			initializePeepholeOptimizerPass(*registry);
			initializeVerifierPass(*registry);
			initializeInstCombine(*registry);
			initializePrintModulePassPass(*registry);
			initializeOptimizePHIsPass(*registry);
			initializeLoopSimplifyPass(*registry);
			initializeIfConverterPass(*registry);
			initializeLoopUnrollPass(*registry);
			initializeCodeGen(*registry);
			initializeTarget(*registry);
			initializeConstantMergePass(*registry);

			cout << "Generating LLVM-assembly...\n";
			LLVMModuleGenerator generator;
			SMDiagnostic diagnostic;
			Module *mod = ParseIRFile(argv[2], diagnostic, getGlobalContext());
			if (mod == 0) {
				cout << "Cant load runtime bitcode: \n" << diagnostic.getMessage() << "\n";
			}

			mod = generator.generate(reader.byteCode(), mod);
			if (mod == 0) {
				cerr << "Failed to create module\n";
				return 0;
			}

			/*cout << "Linking runtime...\n";
			LLVMLinker linker(mod);
			vector< sys::Path > bitcodefiles;
			bitcodefiles.push_back(sys::Path(argv[2]));
			if (linker.link(bitcodefiles)) {
				cerr << "Failed to link runtime\n";
				return 0;
			}
			mod = linker.releaseModule();*/

			cout << "Optimizing and verifying module...\n";
			string errorInfo;
			raw_fd_ostream assemblyFile("llvmassembly.ll", errorInfo);
			PassManager passManager;
			passManager.add(createPrintModulePass(&assemblyFile));
			//passManager.add(createLowerSwitchPass());
			passManager.add(createStripSymbolsPass());
			passManager.add(createLoopSimplifyPass());
			passManager.add(createLoopUnrollPass());
			passManager.add(createVerifierPass());
			passManager.add(createConstantMergePass());
			passManager.run(*mod);
			assemblyFile.close();


			cout << "Writing bitcode file...\n";
			raw_fd_ostream bitcodeFile("raw_bitcode.bc", errorInfo, raw_fd_ostream::F_Binary);
			if (errorInfo.empty()) {
				WriteBitcodeToFile(mod, bitcodeFile);
				bitcodeFile.close();
			}
			else {
				cerr << "Cant open file raw_bitcode.bc\n";
			}
			cout << "Optimizing bitcode...\n";
			system("opt -O3 -o optimized_bitcode.bc raw_bitcode.bc");
			cout << "Creating native assembly...\n";
			system("llc optimized_bitcode.bc -o native_asm.s"); //-filetype obj -o temp.o
			cout << "Building binary...\n";
			system("mingw32-g++ -o cbrun native_asm.s");
			cout << "Success\n";
		}
		else {
			cerr << "Cant open " << argv[1] << "\n";
		}
	}
	else {
		cerr << "Usage: cbE-LLVM.exe <file> <runtime library file>" << endl;
		return -1;
	}
	return 0;
}

