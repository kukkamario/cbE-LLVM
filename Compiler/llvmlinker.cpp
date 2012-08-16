#include "llvmlinker.h"

LLVMLinker::LLVMLinker(Module *module):
	mLinker("LLVMCBCompiler", module)
{
}

bool LLVMLinker::link(const std::vector< sys::Path > &bitcodes) {
	return mLinker.LinkInFiles(bitcodes);
}

bool LLVMLinker::link(Module *m) {
	return mLinker.LinkInModule(m);
}

Module *LLVMLinker::releaseModule() {
	return mLinker.releaseModule();
}
