#ifdef RUNTIME
	#ifndef RUNTIME_FUNCDEF_H
	#define RUNTIME_FUNCDEF_H
	#include "isstring.h"
	#include <stdint.h>
	#define P_INT(_name_) int _name_
	#define P_FLOAT(_name_) float _name_
	#define P_STRING(_name_) const ISString &_name_
	#define P_CHAR_ARRAY(_name_) char *_name_
	#define R_VOID void
	#define R_INT int
	#define R_FLOAT float
	#define R_STRING ISString
	#define RT_FUNC(_name_, _ret_type_ , ...) extern "C" (_ret_type_) _name_ ( __VA_ARGS__ )

	#endif
#else
	#ifdef RT_COMPILER_HEADER
		#ifndef COMPILER_HEADER_FUNCDEF_H
		#define COMPILER_HEADER_FUNCDEF_H
		#define P_INT(_name_)
		#define P_FLOAT(_name_)
		#define P_STRING(_name_)
		#define P_CHAR_ARRAY(_name_)
		#define R_VOID
		#define R_INT
		#define R_FLOAT
		#define R_STRING

		#define RT_FUNC(_name_, _ret_type_ , ...) RT_EXTERN Function *_name_;
		#endif
	#else //COMPILER_HEADER
		#ifndef COMPILER_SOURCE_FUNCDEF_H
		#define COMPILER_SOURCE_FUNCDEF_H
		#define P_INT(_name_) rt_func_params.push_back(IntegerType::get(module->getContext(), 32));
		#define P_FLOAT(_name_) rt_func_params.push_back(Type::getFloatTy(module->getContext()));
		#define P_STRING(_name_) rt_func_params.push_back(PointerType::get(String::mStructType, 0));
		#define P_CHAR_ARRAY(_name_) rt_func_params.push_back(PointerType::get(IntegerType::get(module->getContext(), 8), 0));
		#define R_VOID Type::getVoidTy(module->getContext())
		#define R_INT IntegerType::get(module->getContext(), 32)
		#define R_FLOAT Type::getFloatTy(module->getContext())
		#define R_STRING String::mStructType
		#define RT_FUNC(_name_, _ret_type_ , ...) { \
			vector<Type*> rt_func_params; \
			__VA_ARGS__ \
			FunctionType *_name_##FuncTy = FunctionType::get(_ret_type_, rt_func_params, false); \
			_name_ = Function::Create(_name_##FuncTy, Function::ExternalLinkage, #_name_, module); }
		#endif //COMPILER_SOURCE_FUNCDEF_H
	#endif //COMPILER_HEADER
#endif
