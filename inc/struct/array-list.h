/**
 * Array List
 */

#define DEFINE_STRUCT_ARRAY_LIST_TYPEDEF(NAME, T) \
    typedef struct NAME NAME##_t;

#define DEFINE_STRUCT_ARRAY_LIST_STRUCT(NAME, T) \
    struct NAME { \
        T *al_data; \
        int al_count; \
        int al_capacity; \
        STRUCT_MUTEX_DECLARE(al_mutex) \
        STRUCT_MT_SAFE_DECLARE(al_mt_safe) \
    };

#define DEFINE_STRUCT_ARRAY_LIST(NAME, T)
    
