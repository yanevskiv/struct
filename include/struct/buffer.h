/**
 * (c) Ivan Janevski
 *
 * Rounded Buffer
 */
#include "common.h"

// define typedef struct buffer
#define DEFINE_STRUCT_BUFFER_TYPEDEF(NAME, T) \
    typedef struct NAME NAME##_t;
    
// define struct buffer
#define DEFINE_STRUCT_BUFFER_STRUCT(NAME, T) \
    struct NAME { \
        T *b_data; \
        int b_head; \
        int b_tail; \
        int b_count; \
        int b_size; \
        STRUCT_MUTEX(b_mutex); \
    };

// define buffer_init
#define DEFINE_STRUCT_BUFFER_INIT(NAME, T) \
    struct NAME *NAME##_init(int _size) { \
        struct NAME *_buffer = (struct NAME*) STRUCT_CALLOC(1, struct NAME); \
        _buffer->b_data = STRUCT_CALLOC(_size, T); \
        _buffer->b_head = 0; \
        _buffer->b_tail = 0; \
        _buffer->b_count = 0; \
        _buffer->b_size = _size; \
        STRUCT_MUTEX_INIT(_buffer->b_mutex); \
        return _buffer; \
    }

// define buffer_destroy
#define DEFINE_STRUCT_BUFFER_DESTROY(NAME, T) \
    void NAME##_destroy(struct NAME* _buffer) { \
        STRUCT_MUTEX_DESTROY(_buffer->b_mutex); \
        STRUCT_FREE(_buffer->b_data); \
        STRUCT_FREE(_buffer); \
    }

// define buffer_count
#define DEFINE_STRUCT_BUFFER_COUNT(NAME, T) \
    int NAME##_count(struct NAME* _buffer) { \
        return _buffer->b_count; \
    }

// define buffer_size
#define DEFINE_STRUCT_BUFFER_SIZE(NAME, T) \
    int NAME##_size(struct NAME* _buffer) { \
        return _buffer->b_size; \
    }

// define buffer_empty
#define DEFINE_STRUCT_BUFFER_EMPTY(NAME, T) \
    int NAME##_empty(struct NAME* _buffer) { \
        return _buffer->b_count == 0; \
    }

// define buffer_full
#define DEFINE_STRUCT_BUFFER_FULL(NAME, T) \
    int NAME##_full(struct NAME* _buffer) { \
        return _buffer->b_count == _buffer->b_size; \
    }

// define buffer_put
#define DEFINE_STRUCT_BUFFER_PUT(NAME, T) \
    int NAME##_put(struct NAME *_buffer, T _data) { \
        STRUCT_MUTEX_SAFE_LOCK(_buffer->b_mutex); \
        int _success = 0; \
        if (_buffer->b_count < _buffer->b_size) { \
            _buffer->b_data[_buffer->b_head] = _data; \
            _buffer->b_head = (_buffer->b_head + 1) % _buffer-> b_size; \
            _buffer->b_count += 1; \
            _success = 1; \
        } \
        STRUCT_MUTEX_SAFE_UNLOCK(_buffer->b_mutex); \
        return _success; \
    }

// define buffer_get
#define DEFINE_STRUCT_BUFFER_GET(NAME, T) \
    int NAME##_get(struct NAME *_buffer, T* _return) { \
        int _success = 0; \
        if (_return) { \
            STRUCT_MUTEX_SAFE_LOCK(_buffer->b_mutex); \
            if (_buffer->b_count >= 0) { \
                *_return = _buffer->b_data[_buffer->b_tail]; \
                _buffer->b_tail = (_buffer->b_tail + 1) % _buffer->b_size; \
                _buffer->b_count -= 1; \
                _success = 1; \
            } \
            STRUCT_MUTEX_SAFE_UNLOCK(_buffer->b_mutex); \
        } \
        return _success; \
    }

// define buffer_at
#define DEFINE_STRUCT_BUFFER_AT(NAME, T) \
    T NAME##_at(struct NAME *_buffer, int _at) { \
        return _buffer->b_data[(_buffer->b_tail + _at) % _buffer->b_size]; \
    }

// define buffer_lock and buffer_unlock
#ifndef STRUCT_LOCKS
    #define DEFINE_STRUCT_BUFFER_LOCK(NAME, T)
    #define DEFINE_STRUCT_BUFFER_UNLOCK(NAME, T)
#else
    #define DEFINE_STRUCT_BUFFER_LOCK(NAME, T) \
        void NAME##_lock(struct NAME *_buffer) { \
            STRUCT_MUTEX_LOCK(_buffer->b_mutex); \
        }
    #define DEFINE_STRUCT_BUFFER_UNLOCK(NAME, T) \
        void NAME##_unlock(struct NAME *_buffer) { \
            STRUCT_MUTEX_UNLOCK(_buffer->b_mutex); \
        }
#endif

#define DEFINE_STRUCT_BUFFER(NAME, T) \
    DEFINE_STRUCT_BUFFER_TYPEDEF(NAME, T) \
    DEFINE_STRUCT_BUFFER_STRUCT(NAME, T) \
    DEFINE_STRUCT_BUFFER_INIT(NAME, T) \
    DEFINE_STRUCT_BUFFER_DESTROY(NAME, T) \
    DEFINE_STRUCT_BUFFER_COUNT(NAME, T) \
    DEFINE_STRUCT_BUFFER_SIZE(NAME, T) \
    DEFINE_STRUCT_BUFFER_EMPTY(NAME, T) \
    DEFINE_STRUCT_BUFFER_FULL(NAME, T) \
    \
    DEFINE_STRUCT_BUFFER_PUT(NAME, T) \
    DEFINE_STRUCT_BUFFER_GET(NAME, T) \
    DEFINE_STRUCT_BUFFER_AT(NAME, T) \
    DEFINE_STRUCT_BUFFER_LOCK(NAME, T) \
    DEFINE_STRUCT_BUFFER_UNLOCK(NAME, T)

// DEFINE_STRUCT_BUFFER_RESIZE(NAME, T) \
// DEFINE_STRUCT_BUFFER_PUSH_HEAD(NAME, T) \
// DEFINE_STRUCT_BUFFER_PUSH_TAIL(NAME, T) \
// DEFINE_STRUCT_BUFFER_POP_HEAD(NAME, T) \
// DEFINE_STRUCT_BUFFER_POP_TAIL(NAME, T) \
// DEFINE_STRUCT_BUFFER_CLEAR(NAME, T) \
//
// DEFINE_STRUCT_BUFFER_CONCAT(NAME, T) \
// DEFINE_STRUCT_BUFFER_REVERSE(NAME, T) \
// DEFINE_STRUCT_BUFFER_SLICE(NAME, T) \
// DEFINE_STRUCT_BUFFER_CUT(NAME, T) \
//
// DEFINE_STRUCT_BUFFER_REMOVE(NAME, T) \
// DEFINE_STRUCT_BUFFER_REMOVE_ALL(NAME, T) \
// DEFINE_STRUCT_BUFFER_TRUNCATE(NAME, T) \
// DEFINE_STRUCT_BUFFER_FOR_EACH(NAME, T) \
// DEFINE_STRUCT_BUFFER_MAP(NAME, T) \
// DEFINE_STRUCT_BUFFER_FIND(NAME, T) \
// DEFINE_STRUCT_BUFFER_REDUCE(NAME, T)













