
#ifdef RUNTIME
	#ifndef RUNTIME_FUNCDEF_H
	#define RUNTIME_FUNCDEF_H
	#include "isstring.h"
	#include <stdint.h>
	#define P_INT(_name_) int _name_
	#define P_FLOAT(_name_) float _name_
	#define P_STRING(_name_) const ISString &_name_
	#define P_STRING_POINTER(_name_) ISString * _name_
	#define P_CHAR_ARRAY(_name_) char *_name_
	#define P_VOID void
	#define R_BOOL bool
	#define R_VOID void
	#define R_INT int
	#define R_FLOAT float
	#define R_STRING ISString
	#define RT_FUNC(_name_, _ret_type_, ...) extern "C" _ret_type_ _name_ ( __VA_ARGS__)

	#endif
#else
	#ifdef RT_COMPILER_HEADER
		#ifndef COMPILER_HEADER_FUNCDEF_H
		#define COMPILER_HEADER_FUNCDEF_H
		#ifdef RT_FUNC
			#undef RT_FUNC
			#undef P_INT
			#undef P_FLOAT
			#undef P_STRING
			#undef P_CHAR_ARRAY
			#undef P_STRING_POINTER
			#undef R_VOID
			#undef R_INT
			#undef R_FLOAT
			#undef R_STRING
			#undef R_BOOL
			#undef P_VOID
		#endif
		#define P_INT(_name_)
		#define P_FLOAT(_name_)
		#define P_STRING(_name_)
		#define P_STRING_POINTER(_name_)
		#define P_CHAR_ARRAY(_name_)
		#define P_VOID
		#define R_VOID
		#define R_INT
		#define R_FLOAT
		#define R_STRING
		#define R_BOOL

		#define RT_FUNC(_name_, _ret_type_, ...) RT_EXTERN Function *_name_;
		#endif
	#else //COMPILER_HEADER
		#ifndef COMPILER_SOURCE_FUNCDEF_H
		#define COMPILER_SOURCE_FUNCDEF_H
		#ifdef RT_FUNC
			#undef RT_FUNC
			#undef P_INT
			#undef P_FLOAT
			#undef P_STRING
			#undef P_CHAR_ARRAY
			#undef P_STRING_POINTER
			#undef P_VOID
			#undef R_VOID
			#undef R_INT
			#undef R_FLOAT
			#undef R_STRING
			#undef R_BOOL
		#endif
		#define P_INT(_name_)
		#define P_FLOAT(_name_)
		#define P_STRING(_name_)
		#define P_STRING_POINTER(_name_)
		#define P_CHAR_ARRAY(_name_)
		#define P_VOID
		#define R_VOID
		#define R_INT
		#define R_FLOAT
		#define R_STRING
		#define R_BOOL
#define RT_FUNC(_name_, _ret_type_, ...) _name_ = module->getFunction(#_name_);assert(_name_);
		#endif //COMPILER_SOURCE_FUNCDEF_H
	#endif //COMPILER_HEADER
#endif
