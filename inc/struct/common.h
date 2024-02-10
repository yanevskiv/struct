/** 
 * Common defs
 */

#include <stdlib.h>
#include <stdarg.h>

// memory allocation
#if !defined(STRUCT_CALLOC) || !defined(STRUCT_FREE) || !defined(STRUCT_NULL)
    #include <stdlib.h>
    #undef STRUCT_CALLOC
    #undef STRUCT_FREE
    #undef STRUCT_NULL

    #define STRUCT_CALLOC(n, T) calloc((n), sizeof(T))
    #define STRUCT_FREE(x) free(x)
    #define STRUCT_NULL NULL
#endif

//#define STRUCT_NULL NULL
//#define STRUCT_DEFAULT NULL
//#define STRUCT_CTOR
//#define STRUCT_DTOR

// success / failure
#if !defined(STRUCT_FAILURE) || !defined(STRUCT_SUCCESS)
    #define STRUCT_FAILURE 0
    #define STRUCT_SUCCESS 1 
#endif

// errors
#define STRUCT_ERROR_FUNCTION(func, err)

// what to define
#define STRUCT_DEFINE_STACK
#define STRUCT_DEFINE_QUEUE
#define STRUCT_DEFINE_DOUBLE_STACK
#define STRUCT_DEFINE_DOUBLE_QUEUE
#define STRUCT_DEFINE_BUFFER
#define STRUCT_DEFINE_PRIORITY_QUEUE

// comparison
#define STRUCT_ASSIGN(x, y) ((x) = (y))
#define STRUCT_LESS(x, y) ((x) < (y))
#define STRUCT_GREATER(x, y) (! STRUCT_LESS((x), (y)))
#define STRUCT_EQUALS(x, y) ((x) == (y))

// arithmetic
#define STRUCT_ADD(x, y) ((x) + (y))
#define STRUCT_SUB(x, y) ((x) - (y))
#define STRUCT_NEG(x) (-(x))
#define STRUCT_MUL(x, y) ((x) * (y))
#define STRUCT_DIV(x, y) ((x) / (y))
#define STRUCT_MOD(x, y) ((x) % (y))

// threads
#if defined(_PTHREAD_H)
    #define STRUCT_THREADS
    #include "pthread.h"
#endif
#if !defined(STRUCT_THREADS) 
    //  remove invalid
    #undef STRUCT_MUTEX_DECLARE
    #undef STRUCT_MUTEX_INIT
    #undef STRUCT_MUTEX_LOCK
    #undef STRUCT_MUTEX_UNLOCK
    #undef STRUCT_MUTEX_DESTROY

    // Mutex
    #define STRUCT_MUTEX_DECLARE(mutex)
    #define STRUCT_MUTEX_INIT(mutex)
    #define STRUCT_MUTEX_LOCK(mutex)
    #define STRUCT_MUTEX_UNLOCK(mutex)
    #define STRUCT_MUTEX_DESTROY(mutex)
#endif

// MT-safety
#if defined(STRUCT_MT_SAFE)
    #undef STRUCT_MT_SAFE_DECLARE
    #undef STRUCT_MT_SAFE_INIT
    #undef STRUCT_MT_SAFE_DESTROY
    #undef STRUCT_MT_SAFE_LOCK
    #undef STRUCT_MT_SAFE_UNLOCK

    #define STRUCT_MT_SAFE_DECLARE(mutex) STRUCT_MUTEX_DECLARE(mutex);
    #define STRUCT_MT_SAFE_INIT(mutex)    STRUCT_MUTEX_INIT(mutex)
    #define STRUCT_MT_SAFE_DESTROY(mutex) STRUCT_MUTEX_DESTROY(mutex)
    #define STRUCT_MT_SAFE_LOCK(mutex)    STRUCT_MUTEX_LOCK(mutex)
    #define STRUCT_MT_SAFE_UNLOCK(mutex)  STRUCT_MUTEX_UNLOCK(mutex)
#else
    #ifndef STRUCT_MT_SAFE_DECLARE
        #define STRUCT_MT_SAFE_DECLARE(mutex)
    #endif
    #ifndef STRUCT_MT_SAFE_INIT
        #define STRUCT_MT_SAFE_INIT(mutex)
    #endif
    #ifndef STRUCT_MT_SAFE_DESTROY
        #define STRUCT_MT_SAFE_DESTROY(mutex)
    #endif
    #ifndef STRUCT_MT_SAFE_LOCK
        #define STRUCT_MT_SAFE_LOCK(mutex)
    #endif
    #ifndef STRUCT_MT_SAFE_UNLOCK
        #define STRUCT_MT_SAFE_UNLOCK(mutex)
    #endif
#endif

// features
#if defined(STRUCT_EVERYTHING)
    #if !defined(STRUCT_NO_QUEUE) && !defined(STRUCT_QUEUE)
        #define STRUCT_QUEUE
    #endif
    #if !defined(STRUCT_NO_STACK) && !defined(STRUCT_STACK)
        #define STRUCT_STACK
    #endif
    #if !defined(STRUCT_NO_DOUBLE_STACK) && !defined(STRUCT_DOUBLE_STACK)
        #define STRUCT_DOUBLE_STACK
    #endif
#endif

// varargs
#if defined(STRUCT_VA)
    #include <stdarg.h>
#endif

// cast & backcast
#if !defined(STRUCT_CAST)
    #define STRUCT_CAST
#endif
#if !defined(STRUCT_BACKCAST)
    #define STRUCT_BACKCAST
#endif

// suffix
#if !defined(STRUCT_SUFFIX)
    #define STRUCT_SUFFIX
#endif

// atributes
#if !defined(STRUCT_ATTRIB)
    #undef STRUCT_ATTRIB_STATIC
    #undef STRUCT_ATTRIB_EXTERN
    #undef STRUCT_ATTRIB_INLINE

    // static
    #if !defined(STRUCT_STATIC)
        #define STRUCT_ATTRIB_STATIC
    #else
        #define STRUCT_ATTRIB_STATIC static
    #endif

    // extern
    #if !defined(STRUCT_EXTERN)
        #define STRUCT_ATTRIB_EXTERN
    #else
        #define STRUCT_ATTRIB_EXTERN extern
    #endif

    // inline
    #if !defined(STRUCT_INLINE)
        #define STRUCT_ATTRIB_INLINE 
    #else
        #define STRUCT_ATTRIB_INLINE inline
    #endif

    // custom
    #if !defined(STRUCT_ATTRIB_CUSTOM)
        #define STRUCT_ATTRIB_CUSTOM
    #endif

    #define STRUCT_ATTRIB \
        STRUCT_ATTRIB_STATIC \
        STRUCT_ATTRIB_EXTERN \
        STRUCT_ATTRIB_INLINE \
        STRUCT_ATTRIB_CUSTOM
#endif

// zero value 
#if !defined(STRUCT_ZERO)
    #define STRUCT_ZERO 0
#endif

// value of one
#if !defined(STRUCT_ONE)
    #define STRUCT_ONE 1
#endif

// invalid value (defaults to zero)
#if !defined(STRUCT_INVALID)
    #define STRUCT_INVALID STRUCT_ZERO
#endif

// generic data
#if !defined(STRUCT_GENERIC_DATA)
    #define STRUCT_GENERIC_DATA void*
#endif

// bool
#if !defined(STRUCT_BOOL)
    #define STRUCT_BOOL int
#endif
#if !defined(STRUCT_TRUE)
    #define STRUCT_TRUE 1
#endif
#if !defined(STRUCT_FALSE)
    #define STRUCT_FALSE 0
#endif
