QT += core gui
TEMPLATE = app
CONFIG += console
CONFIG += precompiled_header

win32 {
	INCLUDEPATH += "$$(LLVM_INCLUDE)"
	CONFIG(release, debug|release){
		LIBS += -L"$$(LLVM_LIB)/Release"
	} else {
		LIBS += -L"$$(LLVM_LIB)/Debug"
	}
	LIBS += -lAdvapi32 -lShell32 -lLLVMSupport -lLLVMAnalysis -lLLVMCore -lLLVMDebugInfo -lLLVMInstrumentation -lLLVMScalarOpts -lLLVMTransformUtils -lLLVMTarget -lLLVMMCParser  -lLLVMSelectionDAG -lLLVMX86Info -lLLVMX86AsmParser -lLLVMX86AsmPrinter -lLLVMTableGen -lLLVMX86Utils -lLLVMX86Desc -lFileUpdate -lLLVMX86CodeGen -lModuleMaker -lFileCheck -lLLVMArchive -lLLVMBitReader -lLLVMBitWriter -lLLVMAsmParser -lLLVMAsmPrinter -lLLVMInterpreter -lLLVMInstCombine -lLLVMMC -lLLVMMCJIT -lLLVMCodeGen -lLLVMipo -lLLVMipa -lLLVMLinker -lLLVMJIT -lLLVMExecutionEngine
}

HEADERS += \
    bytecodereader.h \
    bytecode.h \
    instruction.h \
    precomp.h \
    stringpool.h \
    llvmmodulegenerator.h \
    llvmlinker.h \
    stackvalue.h \
    runtimefunction.h \
    variable.h \
    cbfunction.h

SOURCES += main.cpp \
	bytecodereader.cpp \
	bytecode.cpp \
    stringpool.cpp \
    instruction.cpp \
    llvmmodulegenerator.cpp \
    llvmlinker.cpp \
    runtimefunction.cpp \
    cbfunction.cpp

PRECOMPILED_HEADER = precomp.h
