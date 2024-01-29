/**
 * (c) Ivan Janevski
 *
 * Rounded Buffer
 */
#include "common.h"

// define: typedef
#define DEFINE_STRUCT_BUFFER_TYPEDEF(NAME, T) \
    typedef struct NAME NAME##_t;
    
// define: struct
#define DEFINE_STRUCT_BUFFER_STRUCT(NAME, T) \
    struct NAME { \
        T *b_data; \
        int b_head; \
        int b_tail; \
        int b_count; \
        int b_size; \
        STRUCT_MUTEX_DECLARE(b_mutex) \
        STRUCT_MT_SAFE_DECLARE(b_mt_safe) \
    };

// define: new
#define DEFINE_STRUCT_BUFFER_NEW(NAME, T) \
    STRUCT_ATTRIB struct NAME *NAME##_new(int _size)\
    { \
        struct NAME *_buffer = (struct NAME*) STRUCT_CALLOC(1, struct NAME); \
        if (_size > 0) { \
            _buffer->b_data = STRUCT_CALLOC(_size, T); \
            _buffer->b_size = _size; \
        } else { \
            _buffer->b_data = NULL; \
            _buffer->b_size = 0; \
        } \
        _buffer->b_head = 0; \
        _buffer->b_tail = 0; \
        _buffer->b_count = 0; \
        STRUCT_MUTEX_INIT(_buffer->b_mutex) \
        STRUCT_MT_SAFE_INIT(_buffer->b_mt_safe) \
        return _buffer; \
    }

// define: init
#define DEFINE_STRUCT_BUFFER_INIT(NAME, T) \
    STRUCT_ATTRIB void NAME##_init(struct NAME *_buffer, int _size)\
    { \
        if (_size > 0) { \
            _buffer->b_data = STRUCT_CALLOC(_size, T); \
            _buffer->b_size = _size; \
        } else { \
            _buffer->b_data = NULL; \
            _buffer->b_size = 0; \
        } \
        _buffer->b_head = 0; \
        _buffer->b_tail = 0; \
        _buffer->b_count = 0; \
        _buffer->b_size = _size; \
        STRUCT_MUTEX_INIT(_buffer->b_mutex) \
        STRUCT_MT_SAFE_INIT(_buffer->b_mt_safe) \
    }

// define: destroy
#define DEFINE_STRUCT_BUFFER_DESTROY(NAME, T) \
    void NAME##_destroy(struct NAME* _buffer)\
    { \
        STRUCT_MT_SAFE_DESTROY(_buffer->b_mt_safe) \
        STRUCT_MUTEX_DESTROY(_buffer->b_mutex); \
        STRUCT_FREE(_buffer->b_data); \
    }

// define: free
#define DEFINE_STRUCT_BUFFER_FREE(NAME, T) \
    void NAME##_free(struct NAME* _buffer)\
    { \
        STRUCT_MT_SAFE_DESTROY(_buffer->b_mt_safe) \
        STRUCT_MUTEX_DESTROY(_buffer->b_mutex); \
        STRUCT_FREE(_buffer->b_data); \
        STRUCT_FREE(_buffer); \
    }

// mutex lock / unlock
#if defined(STRUCT_THREADS)
    // define: lock
    #define DEFINE_STRUCT_BUFFER_LOCK(NAME, T) \
        STRUCT_ATTRIB void NAME##_lock(struct NAME *_buffer)\
        { \
            STRUCT_MUTEX_LOCK(_buffer->b_mutex); \
        }

    // define: unlock
    #define DEFINE_STRUCT_BUFFER_UNLOCK(NAME, T) \
        STRUCT_ATTRIB void NAME##_unlock(struct NAME *_buffer)\
        { \
            STRUCT_MUTEX_UNLOCK(_buffer->b_mutex); \
        }
#else
    // define: lock
    #define DEFINE_STRUCT_BUFFER_LOCK(NAME, T)

    // define: unlock
    #define DEFINE_STRUCT_BUFFER_UNLOCK(NAME, T)
#endif

// mt_safe lock/unlock
#if defined(STRUCT_THREADS) && defined(STRUCT_MT_SAFE)
    // define: mt_safe_lock
    #define DEFINE_STRUCT_BUFFER_MT_SAFE_LOCK(NAME, T) \
        STRUCT_ATTRIB void NAME##_mt_safe_lock(struct NAME *_buffer)\
        { \
            STRUCT_MT_SAFE_LOCK(_buffer->b_mutex); \
        }

    // define: unlock
    #define DEFINE_STRUCT_BUFFER_MT_SAFE_UNLOCK(NAME, T) \
        STRUCT_ATTRIB void NAME##_mt_safe_unlock(struct NAME *_buffer)\
        { \
            STRUCT_MT_SAFE_UNLOCK(_buffer->b_mutex); \
        }
#else
    // define: lock
    #define DEFINE_STRUCT_BUFFER_MT_SAFE_LOCK(NAME, T)

    // define: unlock
    #define DEFINE_STRUCT_BUFFER_MT_SAFE_UNLOCK(NAME, T)
#endif

// define: count
#define DEFINE_STRUCT_BUFFER_COUNT(NAME, T) \
    STRUCT_ATTRIB int NAME##_count(struct NAME* _buffer)\
    { \
        STRUCT_MT_SAFE_LOCK(_buffer->b_mt_safe) \
        int _count = _buffer->b_count; \
        STRUCT_MT_SAFE_UNLOCK(_buffer->b_mt_safe) \
        return _count; \
    }

// define: size
#define DEFINE_STRUCT_BUFFER_SIZE(NAME, T) \
    STRUCT_ATTRIB int NAME##_size(struct NAME* _buffer)\
    { \
        STRUCT_MT_SAFE_LOCK(_buffer->b_mt_safe) \
        int _size = _buffer->b_size; \
        STRUCT_MT_SAFE_UNLOCK(_buffer->b_mt_safe) \
        return _size; \
    }

// define: empty
#define DEFINE_STRUCT_BUFFER_EMPTY(NAME, T) \
    STRUCT_ATTRIB int NAME##_empty(struct NAME* _buffer)\
    { \
        STRUCT_MT_SAFE_LOCK(_buffer->b_mt_safe) \
        int _empty = (_buffer->b_count == 0); \
        STRUCT_MT_SAFE_UNLOCK(_buffer->b_mt_safe) \
        return _empty; \
    }

// define: full
#define DEFINE_STRUCT_BUFFER_FULL(NAME, T) \
    STRUCT_ATTRIB int NAME##_full(struct NAME* _buffer)\
    { \
        STRUCT_MT_SAFE_LOCK(_buffer->b_mt_safe) \
        int _full = (_buffer->b_count == _buffer->b_size); \
        STRUCT_MT_SAFE_UNLOCK(_buffer->b_mt_safe) \
        return _full; \
    }

// body: void buffer_push_head(NAME *_buffer, T _data)
#define STRUCT_BODY_BUFFER_PUSH_HEAD(NAME, T) \
    { \
        int _count = 0; \
        STRUCT_MT_SAFE_LOCK(_buffer->b_mt_safe); \
        STRUCT_ASSIGN(_buffer->b_data[_buffer->b_head], _data); \
        if (_buffer->b_count < _buffer->b_size) { \
            _buffer->b_count += 1; \
            if (_buffer->b_head == 0) { \
                _buffer->b_head = _buffer->b_size - 1; \
            } else { \
                _buffer->b_head -= 1; \
            } \
            _count = 1; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_buffer->b_mt_safe); \
        return _count; \
    }

// body: void buffer_push_tail(NAME *_buffer, T _data)
#define STRUCT_BODY_BUFFER_PUSH_TAIL(NAME, T) \
    { \
        int _count = 0; \
        STRUCT_MT_SAFE_LOCK(_buffer->b_mt_safe); \
        STRUCT_ASSIGN(_buffer->b_data[_buffer->b_tail], _data); \
        if (_buffer->b_count < _buffer->b_size) { \
            _buffer->b_count += 1; \
            if (_buffer->b_tail == _buffer->b_size - 1) { \
                _buffer->b_count = 0; \
            } else { \
                _buffer->b_tail += 1; \
            } \
            _count = 1; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_buffer->b_mt_safe); \
        return _count; \
    }

// body: void buffer_pop_head(NAME *_buffer, T *_return)
#define STRUCT_BODY_BUFFER_POP_HEAD(NAME, T) \
    { \
        int _count = 0; \
        STRUCT_MT_SAFE_LOCK(_buffer->b_mt_safe); \
        if (_buffer->b_count > 0) { \
            STRUCT_ASSIGN(*_return, _buffer->b_data[_buffer->b_head]); \
            _buffer->b_count -= 1; \
            if (_buffer->b_head == _buffer->b_size - 1) { \
                _buffer->b_head = 0; \
            } else { \
                _buffer->b_head += 1; \
            } \
            _count = 1; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_buffer->b_mt_safe); \
        return _count; \
    }

// body: void buffer_pop_tail(NAME *_buffer, T *_return)
#define STRUCT_BODY_BUFFER_POP_TAIL(NAME, T) \
    { \
        int _count = 0; \
        STRUCT_MT_SAFE_LOCK(_buffer->b_mt_safe); \
        if (_buffer->b_count > 0) { \
            STRUCT_ASSIGN(*_return, _buffer->b_data[_buffer->b_tail]); \
            _buffer->b_count -= 1; \
            if (_buffer->b_tail == 0) { \
                _buffer->b_tail = _buffer->b_size - 1; \
            } else { \
                _buffer->b_tail -= 1; \
            } \
            _count = 1; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_buffer->b_mt_safe); \
        return _count; \
    }

// define: put
#define DEFINE_STRUCT_BUFFER_PUT(NAME, T) \
    STRUCT_ATTRIB int NAME##_put(struct NAME *_buffer, T _data) \
    STRUCT_BODY_BUFFER_PUSH_TAIL(NAME, T)

// define: take
#define DEFINE_STRUCT_BUFFER_TAKE(NAME, T) \
    STRUCT_ATTRIB int NAME##_take(struct NAME *_buffer, T *_return) \
    STRUCT_BODY_BUFFER_POP_HEAD(NAME, T)

// define: at
#define DEFINE_STRUCT_BUFFER_AT(NAME, T) \
    STRUCT_ATTRIB T NAME##_at(struct NAME *_buffer, int _at) \
    { \
        STRUCT_MT_SAFE_LOCK(_buffer->b_mt_safe); \
        T _return = STRUCT_INVALID; \
        STRUCT_ASSIGN(_return, (_buffer->b_data[(_buffer->b_head + _at) % _buffer->b_size])); \
        STRUCT_MT_SAFE_UNLOCK(_buffer->b_mt_safe); \
        return _return; \
    }

// define: get
#define DEFINE_STRUCT_BUFFER_GET(NAME, T) \
    STRUCT_ATTRIB int NAME##_get(struct NAME *_buffer, T *_return, int _at) \
    { \
        STRUCT_MT_SAFE_LOCK(_buffer->b_mt_safe); \
        int _count = 0; \
        if (_return && _at >= 0 && _at < _buffer->b_count) { \
            STRUCT_ASSIGN(*_return, _buffer->b_data[(_buffer->b_head + _at) % _buffer->b_size]); \
            _count = 1; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_buffer->b_mt_safe); \
        return _count; \
    }

// define: resize
#define DEFINE_STRUCT_BUFFER_RESIZE(NAME, T) \
    STRUCT_ATTRIB void NAME##_resize(struct NAME *_buffer, int _size) \
    { \
        STRUCT_MT_SAFE_LOCK(_buffer->b_mt_safe); \
        if (_size == 0) { \
            if (_buffer->b_data) { \
                STRUCT_FREE(_buffer->b_data); \
            } \
            _buffer->b_data = STRUCT_NULL; \
            _buffer->b_head = 0; \
            _buffer->b_tail = 0; \
            _buffer->b_count = 0; \
            _buffer->b_size = 0; \
        } else { \
            T *_data = (T*) STRUCT_CALLOC(_size, T); \
            int _index; \
            for (_index = 0; _index < _buffer->b_size && _index < _size; _index++) { \
                STRUCT_ASSIGN(_data[_index], _buffer->b_data[(_buffer->b_head + _index) % _buffer->b_size]); \
            } \
            if (_buffer->b_data) { \
                STRUCT_FREE(_buffer->b_data); \
            } \
            _buffer->b_data = _data; \
            _buffer->b_head = 0; \
            _buffer->b_tail = _buffer->b_head + _index; \
            _buffer->b_count = _index; \
            _buffer->b_size = 0; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_buffer->b_mt_safe); \
    }

// define: clear
#define DEFINE_STRUCT_BUFFER_CLEAR(NAME, T) \
    STRUCT_ATTRIB void NAME##_clear(struct NAME *_buffer) { \
        STRUCT_MT_SAFE_LOCK(_buffer->b_mt_safe); \
        _buffer->b_head = 0; \
        _buffer->b_tail = 0; \
        _buffer->b_count = 0; \
        STRUCT_MT_SAFE_UNLOCK(_buffer->b_mt_safe); \
    }

// define: concat
#define DEFINE_STRUCT_BUFFER_CONCAT(NAME, T) \
    STRUCT_ATTRIB int NAME##_concat(struct NAME *_buffer, struct NAME *_from) \
    { \
        STRUCT_MT_SAFE_LOCK(_buffer->b_mt_safe); \
        int _index, _count = 0; \
        for (_index = 0; _index < _from->b_count; _index++) { \
            if (_buffer->b_count == _buffer->b_size) \
                break; \
            STRUCT_ASSIGN( \
                _buffer->b_data[(_buffer->b_head + _index) % _buffer->b_size], \
                _from->b_data[(_from->b_head + _index) % _from->b_size] \
            ); \
            _buffer->b_head = (_buffer->b_head + 1) % _buffer->b_size; \
            _buffer->b_count += 1; \
            _count += 1; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_buffer->b_mt_safe); \
        return _count; \
    }

// define: reverse
#define DEFINE_STRUCT_BUFFER_REVERSE(NAME, T) \
    STRUCT_ATTRIB void NAME##_reverse(struct NAME *_buffer) \
    { \
        STRUCT_MT_SAFE_LOCK(_buffer->b_mt_safe); \
        int _index, _halfCount = _buffer->b_count >> 1; \
        T _tmp; \
        for (_index = 0; _index < _halfCount; _index++) { \
            STRUCT_ASSIGN( \
                _tmp, \
                _buffer->b_data[(_buffer->b_head + _index) % _buffer->b_size] \
            ); \
            STRUCT_ASSIGN( \
                _buffer->b_data[(_buffer->b_head + _index) % _buffer->b_count], \
                _buffer->b_data[(_buffer->b_head + _buffer->b_count - _index - 1) % _buffer->b_size] \
            ); \
            STRUCT_ASSIGN( \
                _buffer->b_data[(_buffer->b_head + _buffer->b_count - _index - 1) % _buffer->b_size], \
                _tmp \
            ); \
        } \
        STRUCT_MT_SAFE_UNLOCK(_buffer->b_mt_safe); \
    }

// custom: for_each
#define DEFINE_STRUCT_BUFFER_CUSTOM_FOR_EACH(NAME, T, FNAME, FUNC) \
    STRUCT_ATTRIB void NAME##FNAME(struct NAME *_buffer) \
    { \
        STRUCT_MT_SAFE_LOCK(_buffer->b_mt_safe); \
        int _index; \
        for (_index = 0; _index < _buffer->b_count; _index++) { \
            FUNC((_buffer->b_data[(_buffer->b_head + _index) % _buffer->b_count])); \
        } \
        STRUCT_MT_SAFE_UNLOCK(_buffer->b_mt_safe); \
    }

#define DEFINE_STRUCT_BUFFER(NAME, T) \
    DEFINE_STRUCT_BUFFER_TYPEDEF(NAME, T) \
    DEFINE_STRUCT_BUFFER_STRUCT(NAME, T) \
    DEFINE_STRUCT_BUFFER_NEW(NAME, T) \
    DEFINE_STRUCT_BUFFER_INIT(NAME, T) \
    DEFINE_STRUCT_BUFFER_DESTROY(NAME, T) \
    DEFINE_STRUCT_BUFFER_FREE(NAME, T) \
    \
    DEFINE_STRUCT_BUFFER_LOCK(NAME, T) \
    DEFINE_STRUCT_BUFFER_UNLOCK(NAME, T) \
    DEFINE_STRUCT_BUFFER_MT_SAFE_LOCK(NAME, T) \
    DEFINE_STRUCT_BUFFER_MT_SAFE_UNLOCK(NAME, T) \
    \
    DEFINE_STRUCT_BUFFER_COUNT(NAME, T) \
    DEFINE_STRUCT_BUFFER_SIZE(NAME, T) \
    DEFINE_STRUCT_BUFFER_EMPTY(NAME, T) \
    DEFINE_STRUCT_BUFFER_FULL(NAME, T) \
    \
    DEFINE_STRUCT_BUFFER_PUT(NAME, T) \
    DEFINE_STRUCT_BUFFER_TAKE(NAME, T) \
    DEFINE_STRUCT_BUFFER_AT(NAME, T) \
    \
    DEFINE_STRUCT_BUFFER_RESIZE(NAME, T) \
    DEFINE_STRUCT_BUFFER_CLEAR(NAME, T) \
    DEFINE_STRUCT_BUFFER_CONCAT(NAME, T) \
    DEFINE_STRUCT_BUFFER_REVERSE(NAME, T) \


/*
TODO:
    DEFINE_STRUCT_BUFFER_PUSH_HEAD(NAME, T) \
    DEFINE_STRUCT_BUFFER_PUSH_TAIL(NAME, T) \
    DEFINE_STRUCT_BUFFER_POP_HEAD(NAME, T) \
    DEFINE_STRUCT_BUFFER_POP_TAIL(NAME, T) \

    DEFINE_STRUCT_BUFFER_SLICE(NAME, T) \
    DEFINE_STRUCT_BUFFER_CUT(NAME, T) \

    DEFINE_STRUCT_BUFFER_REMOVE(NAME, T) \
    DEFINE_STRUCT_BUFFER_REMOVE_ALL(NAME, T) \
    DEFINE_STRUCT_BUFFER_TRUNCATE(NAME, T) \

    DEFINE_STRUCT_BUFFER_FOR_EACH(NAME, T) \
    DEFINE_STRUCT_BUFFER_FOR_EACH_INDEX(NAME, T) \
    DEFINE_STRUCT_BUFFER_FOR_EACH_DATA(NAME, T) \
    DEFINE_STRUCT_BUFFER_FOR_EACH_INIDEX_DATA(NAME, T) \

    DEFINE_STRUCT_BUFFER_MAP(NAME, T) \
    DEFINE_STRUCT_BUFFER_FIND(NAME, T) \
    DEFINE_STRUCT_BUFFER_REDUCE(NAME, T)
*/
