/** 
 * (c) Ivan Janevski
 *
 * Common defs
 */

#include <stdlib.h>
#include <stdarg.h>
// memory
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

// define what?
#define STRUCT_DEFINE_STACK
#define STRUCT_DEFINE_QUEUE
#define STRUCT_DEFINE_DOUBLE_STACK
#define STRUCT_DEFINE_DOUBLE_QUEUE
#define STRUCT_DEFINE_BUFFER
#define STRUCT_DEFINE_PRIORITY_QUEUE

// operators
#define STRUCT_ASSIGN(x, y) ((x) = (y))
#define STRUCT_LESS(x, y) ((x) < (y))
#define STRUCT_EQUALS(x, y) ((x) == (y))

#define STRUCT_ADD(x, y) ((x) + (y))
#define STRUCT_SUB(x, y) ((x) - (y))
#define STRUCT_NEG(x) (-(x))
#define STRUCT_MUL(x, y) ((x) * (y))
#define STRUCT_DIV(x, y) ((x) / (y))


// concurrnecy 
#if !defined(STRUCT_MUTEX) 
    //  remove invalid
    #undef STRUCT_MUTEX_INIT
    #undef STRUCT_MUTEX_LOCK
    #undef STRUCT_MUTEX_UNLOCK
    #undef STRUCT_MUTEX_DESTROY

    // Mutex
    #define STRUCT_MUTEX(mutex)
    #define STRUCT_MUTEX_INIT(mutex)
    #define STRUCT_MUTEX_LOCK(mutex)
    #define STRUCT_MUTEX_UNLOCK(mutex)
    #define STRUCT_MUTEX_DESTROY(mutex)

    // MT-safe
    #define STRUCT_MUTEX_SAFE_LOCK(mutex)
    #define STRUCT_MUTEX_SAFE_UNLOCK(mutex)
#endif

// thread safety
#if defined(STRUCT_MT_SAFE)
    #undef STRUCT_MUTEX_SAFE_LOCK
    #undef STRUCT_MUTEX_SAFE_UNLOCK
    #define STRUCT_MUTEX_SAFE_LOCK(mutex)   STRUCT_MUTEX_LOCK(mutex)
    #define STRUCT_MUTEX_SAFE_UNLOCK(mutex) STRUCT_MUTEX_UNLOCK(mutex)
#else
    #ifndef STRUCT_MUTEX_SAFE_LOCK
        #define STRUCT_MUTEX_SAFE_LOCK
    #endif
    #ifndef STRUCT_MUTEX_SAFE_LOCK
        #define STRUCT_MUTEX_SAFE_UNLOCK
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

#if defined(STRUCT_VA)
    #include <stdarg.h>
#endif

// cast 
#if !defined(STRUCT_CAST)
    #define STRUCT_CAST
#endif

// suffix
#if !defined(STRUCT_SUFFIX)
    #define STRUCT_SUFFIX
#endif
