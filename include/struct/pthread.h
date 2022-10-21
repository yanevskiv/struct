/**
 * (c) Ivan Janevski
 */
#undef STRUCT_MUTEX_DECLARE
#undef STRUCT_MUTEX_INIT
#undef STRUCT_MUTEX_DESTROY
#undef STRUCT_MUTEX_LOCK
#undef STRUCT_MUTEX_UNLOCK

#define STRUCT_MUTEX_DECLARE(mutex) pthread_mutex_t mutex;
#define STRUCT_MUTEX_INIT(mutex)    pthread_mutex_init(&(mutex), STRUCT_NULL);
#define STRUCT_MUTEX_DESTROY(mutex) pthread_mutex_destroy(&(mutex));
#define STRUCT_MUTEX_LOCK(mutex)    pthread_mutex_lock(&(mutex));
#define STRUCT_MUTEX_UNLOCK(mutex)  pthread_mutex_unlock(&(mutex));

