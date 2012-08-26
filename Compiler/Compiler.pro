QT -= core gui
TEMPLATE = app
CONFIG -= qt
CONFIG += console
CONFIG += precompiled_header

win32 {
	INCLUDEPATH += "$$(LLVM_INCLUDE)" "$$(BOOST_INCLUDE)"
	LIBS +=  -L"$$(LLVM_LIB)"
	#LIBS +=  -lAdvapi32  -lShell32  -lLLVMSupport  -lLLVMAnalysis  -lLLVMCore  -lLLVMDebugInfo  -lLLVMObject -lLLVMScalarOpts  -lLLVMTransformUtils  -lLLVMTarget  -lLLVMMCParser  -lLLVMSelectionDAG  -lLLVMX86Info  -lLLVMX86AsmParser  -lLLVMX86AsmPrinter  -lLLVMTableGen  -lLLVMX86Utils  -lLLVMX86Desc  -lLLVMX86CodeGen  -lLLVMArchive  -lLLVMBitReader  -lLLVMBitWriter  -lLLVMAsmParser  -lLLVMAsmPrinter  -lLLVMInterpreter  -lLLVMInstCombine  -lLLVMMC  -lLLVMMCJIT  -lLLVMCodeGen  -lLLVMipo  -lLLVMipa  -lLLVMLinker  -lLLVMJIT  -lLLVMExecutionEngine
	DEFINES += __STDC_LIMIT_MACROS __STDC_CONSTANT_MACROS

	LIBS += -lLLVMVectorize -lLLVMipo -lLLVMLinker -lLLVMBitReader -lLLVMBitWriter -lLLVMInstrumentation -lLLVMX86CodeGen -lLLVMX86AsmParser -lLLVMX86Disassembler -lLLVMRuntimeDyld -lLLVMExecutionEngine -lLLVMArchive -lLLVMAsmParser -lLLVMAsmPrinter -lLLVMSelectionDAG -lLLVMX86Desc -lLLVMMCParser -lLLVMCodeGen -lLLVMX86AsmPrinter -lLLVMX86Info -lLLVMScalarOpts -lLLVMX86Utils -lLLVMInstCombine -lLLVMTransformUtils -lLLVMipa -lLLVMAnalysis -lLLVMTarget -lLLVMCore -lLLVMMC -lLLVMObject -lLLVMSupport
	LIBS += -lKernel32 -ldbghelp -lPsapi -lAdvapi32 -lShell32
}

HEADERS += \
    bytecodereader.h \
    bytecode.h \
    instruction.h \
    stringpool.h \
    llvmmodulegenerator.h \
    llvmlinker.h \
    stackvalue.h \
    variable.h \
    cbfunction.h \
    casts.h \
    exception.h \
    precompiled.h \
    rtstring.h \
    operations.h \
    callcreator.h \
    rtsystem.h \
    debug.h \
    rtmath.h

SOURCES += main.cpp \
	bytecodereader.cpp \
	bytecode.cpp \
    stringpool.cpp \
    instruction.cpp \
    llvmmodulegenerator.cpp \
    llvmlinker.cpp \
    cbfunction.cpp \
    casts.cpp \
    exception.cpp \
    rtstring.cpp \
    operations.cpp \
    callcreator.cpp \
    rtsystem.cpp \
    rtmath.cpp

PRECOMPILED_HEADER = precompiled.h
