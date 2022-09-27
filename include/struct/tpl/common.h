#ifndef _STRUCT_COMMON_H_
#define  _STRUCT_COMMON_H_
#include <stdlib.h>
#include <stdarg.h>
#define STRUCT_CALLOC calloc
#define STRUCT_FREE free
#define STRUCT_CONSTRUCTOR
#define STRUCT_DESTRUCTOR
#define STRUCT_CMP(x, y) (((x) > (y)) ? 1 : ((x) < (y)) ? -1 : 0)
#define STRUCT_DEFAULT 0
#define STRUCT_TRUE 1
#define STRUCT_FALSE 0
#endif
