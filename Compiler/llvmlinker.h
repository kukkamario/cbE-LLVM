#ifndef LINKER_H
#define LINKER_H
#include "precomp.h"
class LLVMLinker
{
	public:
		LLVMLinker(Module *module);
		bool link(const std::vector< sys::Path > &bitcodes);
		bool link(Module *m);
		Module *releaseModule();
	private:
		Linker mLinker;
};

#endif // LINKER_H
