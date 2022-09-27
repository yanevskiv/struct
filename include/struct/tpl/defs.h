#include <stdlib.h>
#include <stdarg.h>
#define STRUCT_CALLOC(n, s) (calloc((n), (s)))
#define STRUCT_FREE(x) free(x)

#ifdef STRUCT_TYPE_PRIMITIVE
#define STRUCT_EQUALS(x, y) ((x) == (y))
#endif
#define STRUCT_AT(i, j, M, N) ((j) * (M) + i)
#define STRUCT_SUM(x, y) ((x) + (y))
#define STRUCT_MUL(x, y) ((x) * (y))

#define STRUCT_TRUE 1
#define STRUCT_FALSE 0
#define STRUCT_DEFAULT 0
#define STRUCT_DESTRUCTOR(x) 
#define STRUCT_CONSTRUCTOR(x) (x)

#ifdef STRUCT_TYPE_STRUCT
#undef STRUCT_EQUALS
#define STRUCT_EQUALS
#define STRUCT_WRAP_EQUALS(NAME, T, EQ) NAME##_data_equals EQ
#endif

//#define STRUCT_EQUALS(NAME, T) NAME##_data_equals
//#define STRUCT_SUM(NAME, T)    NAME##_data_sum
//#define STRUCT_MUL(NAME, T)    NAME##_data_mul
