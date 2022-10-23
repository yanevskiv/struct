/**
 * (c) Ivan Janevski
 */


// mutex
#undef STRUCT_MUTEX_DECLARE
#undef STRUCT_MUTEX_INIT
#undef STRUCT_MUTEX_DESTROY
#undef STRUCT_MUTEX_LOCK
#undef STRUCT_MUTEX_UNLOCK

#if !defined(STRUCT_PTHREAD_MTUEX_ATTR)
    #define STRUCT_PTHREAD_MUTEX_ATTR STRUCT_NULL
#endif
#define STRUCT_MUTEX_DECLARE(mutex) pthread_mutex_t mutex;
#define STRUCT_MUTEX_INIT(mutex)    pthread_mutex_init(&(mutex), STRUCT_PTHREAD_MUTEX_ATTR);
#define STRUCT_MUTEX_DESTROY(mutex) pthread_mutex_destroy(&(mutex));
#define STRUCT_MUTEX_LOCK(mutex)    pthread_mutex_lock(&(mutex));
#define STRUCT_MUTEX_UNLOCK(mutex)  pthread_mutex_unlock(&(mutex));

// rwlock
#undef STRUCT_RWLOCK_DECLARE
#undef STRUCT_RWLOCK_INIT
#undef STRUCT_RWLOCK_DESTROY
#undef STRUCT_RWLOCK_RDLOCK
#undef STRUCT_RWLOCK_WRLOCK
#undef STRUCT_RWLOCK_UNLOCK

#if !defined(STRUCT_PTHREAD_RWLOCK_ATTR)
    #define STRUCT_PTHREAD_RWLOCK_ATTR STRUCT_NULL
#endif
#define STRUCT_RWLOCK_DECLARE(rwlock) pthread_rwlock_t rwlock;
#define STRUCT_RWLOCK_INIT(rwlock)    pthread_rwlock_init(&(rwlock), STRUCT_PTHREAD_RWOLOCK_ATTR);
#define STRUCT_RWLOCK_DESTROY(rwlock) pthread_rwlock_destroy(&(rwlock));
#define STRUCT_RWLOCK_RDLOCK(rwlock)  pthread_rwlock_rdlock(&(rwlock));
#define STRUCT_RWLOCK_WRLOCK(rwlock)  pthread_rwlock_wrlock(&(rwlock));
#define STRUCT_RWLOCK_UNLOCK(rwlock)  pthread_rwlock_unlock(&(rwlock));
