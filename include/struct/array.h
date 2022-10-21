/**
 * (c) Ivan Janevski
 *
 * Array
 */

#include "common.h"

// define: typedef
#define DEFINE_STRUCT_ARRAY_TYPEDEF(NAME, T) \
    typedef struct NAME NAME##_t;

// define: struct
#define DEFINE_STRUCT_ARRAY_STRUCT(NAME, T) \
    struct NAME { \
        T *a_data; \
        int a_size; \
        STRUCT_MUTEX_DECLARE(a_mutex) \
        STRUCT_MT_SAFE_DECLARE(a_mt_safe) \
    };

// define: new
#define DEFINE_STRUCT_ARRAY_NEW(NAME, T) \
    STRUCT_ATTRIB struct NAME *NAME##_new(int _size) \
    { \
        struct NAME *_array = (struct NAME*) STRUCT_CALLOC(1, struct NAME); \
        _array->a_data = (T*) STRUCT_CALLOC(_size, T); \
        _array->a_size = _size; \
        STRUCT_MUTEX_INIT(_array->a_mutex) \
        STRUCT_MT_SAFE_INIT(_array->a_mt_safe) \
        return _array; \
    }

// define: init
#define DEFINE_STRUCT_ARRAY_INIT(NAME, T) \
    STRUCT_ATTRIB void NAME##_init(struct NAME *_array, int  _size) \
    { \
        _array->a_data = (T*) STRUCT_CALLOC(_size, T); \
        _array->a_size = _size; \
        STRUCT_MUTEX_INIT(_array->a_mutex) \
        STRUCT_MT_SAFE_INIT(_array->a_mt_safe) \
    }

// define: destroy
#define DEFINE_STRUCT_ARRAY_DESTROY(NAME, T) \
    STRUCT_ATTRIB void NAME##_destroy(struct NAME *_array) \
    { \
        STRUCT_FREE(_array->a_data); \
        _array->a_data = STRUCT_NULL; \
        _array->a_size = 0; \
        STRUCT_MT_SAFE_DESTROY(_array->a_mutex) \
    }
    
// define: free
#define DEFINE_STRUCT_ARRAY_FREE(NAME, T) \
    STRUCT_ATTRIB void NAME##_free(struct NAME *_array) \
    { \
        STRUCT_FREE(_array->a_data); \
        _array->a_data = STRUCT_NULL; \
        _array->a_size = 0; \
        STRUCT_MUTEX_DESTROY(_array->a_mutex) \
        STRUCT_MT_SAFE_DESTROY(_array->a_mt_safe) \
        STRUCT_FREE(_array); \
    }

// define: clone
#define DEFINE_STRUCT_ARRAY_CLONE(NAME, T) \
    STRUCT_ATTRIB struct NAME *NAME##_clone(struct NAME *_array) \
    {  \
        struct NAME *_clone = (struct NAME*) STRUCT_CALLOC(1, struct NAME); \
        STRUCT_MT_SAFE_LOCK(_array->a_mt_safe) \
        _clone->a_data = (T*) STRUCT_CALLOC(_array->a_size, T); \
        _clone->a_size = _array->a_size; \
        STRUCT_MT_SAFE_INIT(_clone->a_mutex) \
        int _i; \
        for (_i = 0; _i < _clone->a_size; _i++) { \
            STRUCT_ASSIGN(_clone->a_data[_i], _array->a_data[_i]); \
        } \
        STRUCT_MT_SAFE_UNLOCK(_array->a_mt_safe) \
        return _clone; \
    }

// define: slice
#define DEFINE_STRUCT_ARRAY_SLICE(NAME, T) \
    STRUCT_ATTRIB struct NAME *NAME##_slice(struct NAME *_array, int _left, int _right) \
    { \
        STRUCT_MT_SAFE_LOCK(_array->a_mt_safe) \
        struct NAME *_slice = (struct NAME*) STRUCT_CALLOC(1, struct NAME); \
        STRUCT_MT_SAFE_INIT(_slice->a_mutex); \
        if (_left == _right) { \
            _slice->a_data = STRUCT_NULL; \
            _slice->a_size = 0; \
        } else if (_left < _right) { \
            _slice->a_data = (T*) STRUCT_CALLOC(_array->a_size, T); \
            _slice->a_size = _right - _left; \
            int _i; \
            for (_i = 0; _i < _slice->a_size; _i++) { \
                STRUCT_ASSIGN(_slice->a_data[_i], _array->a_data[_i + _left]); \
            } \
        } else if (_left > _right) { \
            _slice->a_data = (T*) STRUCT_CALLOC(_array->a_size, T); \
            _slice->a_size = _left - _right; \
            int _i; \
            for (_i = 0; _i < _slice->a_size; _i++) { \
                STRUCT_ASSIGN(_slice->a_data[_slice->a_size - (_i - 1)], _array->a_data[_i + _left]); \
            } \
        } \
        STRUCT_MT_SAFE_UNLOCK(_array->a_mt_safe) \
        return _slice; \
    }

// define: copy
#define DEFINE_STRUCT_ARRAY_COPY(NAME, T) \
    STRUCT_ATTRIB void NAME##_copy(struct NAME *_array, struct NAME *_from) \
    { \
        STRUCT_MT_SAFE_LOCK(_array->a_mt_safe) \
        int _i; \
        for (_i = 0; _i < _array->a_size && _i < _from->a_size; _i++) { \
            STRUCT_ASSIGN(_array->a_data[_i], _from->a_data[_i]); \
        } \
        STRUCT_MT_SAFE_UNLOCK(_array->a_mt_safe) \
    }

// define: size
#define DEFINE_STRUCT_ARRAY_SIZE(NAME ,T) \
    STRUCT_ATTRIB int NAME##_size(struct NAME *_array) \
    { \
        int _size = 0; \
        STRUCT_MT_SAFE_LOCK(_array->a_mt_safe) \
        _size = _array->a_size; \
        STRUCT_MT_SAFE_UNLOCK(_array->a_mt_safe) \
        return _size; \
    }

// define: resize
#define DEFINE_STRUCT_ARRAY_RESIZE(NAME, T) \
    STRUCT_ATTRIB void NAME##_resize(struct NAME *_array, int _size) \
    { \
        STRUCT_MT_SAFE_LOCK(_array->a_mt_safe) \
        if (_size <= 0) { \
            STRUCT_FREE(_array->a_data); \
            _array->a_data = STRUCT_NULL; \
            _array->a_size = 0; \
        } else { \
            T* _data = (T*) STRUCT_CALLOC(_size, T); \
            int _i; \
            for (_i = 0; _i < _array->a_size && _i < _size; _i++) { \
                STRUCT_ASSIGN(_data[_i], _array->a_data[_i]); \
            } \
            if (_array->a_data) \
                STRUCT_FREE(_array->a_data); \
            _array->a_data = _data; \
            _array->a_size = _size; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_array->a_mt_safe) \
    }

// define: truncate
#define DEFINE_STRUCT_ARRAY_TRUNCATE(NAME, T) \
    STRUCT_ATTRIB void NAME##_truncate(struct NAME *_array, int _front, int _back) \
    { \
        STRUCT_MT_SAFE_LOCK(_array->a_mt_safe) \
        if (_front + _back >= _array->a_size) { \
            if (_array->a_size > 0) { \
                STRUCT_FREE(_array->a_data); \
                _array->a_data = STRUCT_NULL; \
                _array->a_size = 0; \
            } \
        } else { \
            int _i, _size = _array->a_size - (_front + _back); \
            T *_data = (T*) STRUCT_CALLOC(_size, T); \
            for (_i = 0; _i < _size; _i++) { \
                STRUCT_ASSIGN(_data[_i], _array->a_data[_i + _front]); \
            } \
            STRUCT_FREE(_array->a_data); \
            _array->a_data = _data; \
            _array->a_size = _size; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_array->a_mt_safe) \
    }

// define: extend
#define DEFINE_STRUCT_ARRAY_EXTEND(NAME, T) \
    STRUCT_ATTRIB void NAME##_extend(struct NAME *_array, int _front, int _back) \
    { \
        STRUCT_MT_SAFE_LOCK(_array->a_mt_safe) \
        if (_front + _back == 0) \
            return; \
        int _i, _size = _array->a_size + _front + _back; \
        T *_data = (T*) STRUCT_CALLOC(_size, T); \
        for (_i = _front; _i + _back < _array->a_size; _i++) { \
            STRUCT_ASSIGN(_data[_i], _array->a_data[_i]); \
        } \
        if (_array->a_data) \
            STRUCT_FREE(_array->a_data); \
        _array->a_data = _data; \
        _array->a_size = _size; \
        STRUCT_MT_SAFE_UNLOCK(_array->a_mt_safe) \
    }

// define: pad
#define DEFINE_STRUCT_ARRAY_EXTEND_PAD(NAME, T) \
    STRUCT_ATTRIB void NAME##_extend_pad(struct NAME *_array, T _pad, int _front, int _back) \
    { \
        STRUCT_MT_SAFE_LOCK(_array->a_mt_safe) \
        if (_front + _back == 0) \
            return; \
        int _i, _size = _array->a_size + _front + _back; \
        T *_data = (T*) STRUCT_CALLOC(_size, T); \
        for (_i = 0; _i < _front; _i++) { \
            STRUCT_ASSIGN(_data[_i], _pad); \
        } \
        for (_i = 0; _i < _array->a_size; _i++) { \
            STRUCT_ASSIGN(_data[_i + _front], _array->a_data[_i]); \
        } \
        for (_i = 0; _i < _back; _i++) { \
            STRUCT_ASSIGN(_data[_front + _array->a_size + _i], _pad); \
        } \
        if (_array->a_data) \
            STRUCT_FREE(_array->a_data); \
        _array->a_data = _data; \
        _array->a_size = _size; \
        STRUCT_MT_SAFE_UNLOCK(_array->a_mt_safe) \
    }


// define: fill
#define DEFINE_STRUCT_ARRAY_FILL(NAME, T) \
    STRUCT_ATTRIB void NAME##_fill(struct NAME *_array, T _val) \
    { \
        STRUCT_MT_SAFE_LOCK(_array->a_mt_safe) \
        int _i; \
        for (_i = 0; _i < _array->a_size; _i++) { \
            STRUCT_ASSIGN(_array->a_data[_i], _val); \
        } \
        STRUCT_MT_SAFE_UNLOCK(_array->a_mt_safe) \
    }

// define: pad
#define DEFINE_STRUCT_ARRAY_PAD(NAME, T) \
    STRUCT_ATTRIB void NAME##_pad(struct NAME *_array, T _val, int _left, int _right) \
    { \
        int _i; \
        for (_i = 0; _i < _array->a_size && _i < _left; _i++) { \
            STRUCT_ASSIGN(_array->a_data[_i], _val); \
        } \
        for (_i = 0; _i < _array->a_size && _i < _right; _i++) { \
            STRUCT_ASSIGN(_array->a_data[_array->a_size - (_i + 1)], _val); \
        } \
    }
// define: pad_left
#define DEFINE_STRUCT_ARRAY_PAD_LEFT(NAME, T) \
    STRUCT_ATTRIB void NAME##_pad_left(struct NAME *_array, T _val, int _count) \
    { \
        STRUCT_MT_SAFE_LOCK(_array->a_mt_safe) \
        int _i; \
        for (_i = 0; _i < _array->a_size && _i < _count; _i++) { \
            STRUCT_ASSIGN(_array->a_data[_i], _val); \
        } \
        STRUCT_MT_SAFE_UNLOCK(_array->a_mt_safe) \
    }

// define: pad_right
#define DEFINE_STRUCT_ARRAY_PAD_RIGHT(NAME, T) \
    STRUCT_ATTRIB void NAME##_pad_right(struct NAME *_array, T _val, int _count) \
    { \
        STRUCT_MT_SAFE_LOCK(_array->a_mt_safe) \
        int _i; \
        for (_i = 0; _i < _array->a_size && _i < _count; _i++) { \
            STRUCT_ASSIGN(_array->a_data[_array->a_size - (_i + 1)], _val); \
        } \
        STRUCT_MT_SAFE_UNLOCK(_array->a_mt_safe) \
    }

// define: trim
#define DEFINE_STRUCT_ARRAY_TRIM(NAME, T) \
    STRUCT_ATTRIB void NAME##_trim(struct NAME *_array, T _val) \
    { \
        STRUCT_MT_SAFE_LOCK(_array->a_mt_safe) \
        if (_array->a_size > 0) { \
            int _i, _front, _back, _size = _array->a_size; \
            for (_front = 0; _front < _array->a_size && _size > 0; _front++, _size--) { \
                if (! (STRUCT_EQUALS(_array->a_data[_front], _val))) \
                    break; \
            } \
            for (_back = _array->a_size - 1; _back >= 0 && _size > 0; _back--, _size--) { \
                if (! (STRUCT_EQUALS(_array->a_data[_back], _val))) \
                    break; \
            } \
            if (_size == 0) { \
                STRUCT_FREE(_array->a_data); \
                _array->a_data = STRUCT_NULL; \
                _array->a_size = 0; \
            } else { \
                T *_data = (T*) STRUCT_CALLOC(_size, T); \
                for (_i = 0; _i < _size; _i++) { \
                    STRUCT_ASSIGN(_data[_i], _array->a_data[_i + _front]); \
                } \
                STRUCT_FREE(_array->a_data); \
                _array->a_data = _data; \
                _array->a_size = _size; \
            } \
        } \
        STRUCT_MT_SAFE_UNLOCK(_array->a_mt_safe) \
    }

// define: get
#define DEFINE_STRUCT_ARRAY_GET(NAME, T) \
    STRUCT_ATTRIB T NAME##_get(struct NAME *_array, int _at) \
    { \
        T _return; \
        STRUCT_MT_SAFE_LOCK(_array->a_mt_safe) \
        STRUCT_ASSIGN(_return, _array->a_data[_at]); \
        STRUCT_MT_SAFE_UNLOCK(_array->a_mt_safe) \
        return _return; \
    }

// define: set
#define DEFINE_STRUCT_ARRAY_SET(NAME, T) \
    STRUCT_ATTRIB void NAME##_set(struct NAME *_array, T _set, int _at) \
    { \
        STRUCT_MT_SAFE_LOCK(_array->a_mt_safe) \
        STRUCT_ASSIGN(_array->a_data[_at], _set); \
        STRUCT_MT_SAFE_UNLOCK(_array->a_mt_safe) \
    }

// define: lock
#define DEFINE_STRUCT_ARRAY_LOCK(NAME, T) \
    STRUCT_ATTRIB void NAME##_lock(struct NAME *_array) \
    { \
        STRUCT_MUTEX_LOCK(_array->a_mutex) \
    }

// define: unlock
#define DEFINE_STRUCT_ARRAY_UNLOCK(NAME, T) \
    STRUCT_ATTRIB void NAME##_unlock(struct NAME *_array) \
    { \
        STRUCT_MUTEX_UNLOCK(_array->a_mutex) \
    }

#define DEFINE_STRUCT_ARRAY(NAME, T) \
    DEFINE_STRUCT_ARRAY_TYPEDEF(NAME, T) \
    DEFINE_STRUCT_ARRAY_STRUCT(NAME, T) \
    DEFINE_STRUCT_ARRAY_NEW(NAME, T) \
    DEFINE_STRUCT_ARRAY_INIT(NAME, T) \
    DEFINE_STRUCT_ARRAY_DESTROY(NAME, T) \
    DEFINE_STRUCT_ARRAY_FREE(NAME, T) \
    DEFINE_STRUCT_ARRAY_LOCK(NAME, T) \
    DEFINE_STRUCT_ARRAY_UNLOCK(NAME, T) \
    \
    DEFINE_STRUCT_ARRAY_CLONE(NAME, T) \
    DEFINE_STRUCT_ARRAY_SLICE(NAME, T) \
    DEFINE_STRUCT_ARRAY_COPY(NAME, T) \
    DEFINE_STRUCT_ARRAY_SIZE(NAME, T) \
    DEFINE_STRUCT_ARRAY_RESIZE(NAME, T) \
    DEFINE_STRUCT_ARRAY_TRUNCATE(NAME, T) \
    DEFINE_STRUCT_ARRAY_EXTEND(NAME, T) \
    DEFINE_STRUCT_ARRAY_EXTEND_PAD(NAME, T) \
    DEFINE_STRUCT_ARRAY_TRIM(NAME, T) \
    \
    DEFINE_STRUCT_ARRAY_PAD(NAME, T) \
    DEFINE_STRUCT_ARRAY_PAD_LEFT(NAME, T) \
    DEFINE_STRUCT_ARRAY_PAD_RIGHT(NAME, T) \
    DEFINE_STRUCT_ARRAY_FILL(NAME, T) \
    \
    DEFINE_STRUCT_ARRAY_GET(NAME, T) \
    DEFINE_STRUCT_ARRAY_SET(NAME, T) \
    \

/*
    DEFINE_STRUCT_ARRAY_GET(NAME, T) \
    DEFINE_STRUCT_ARRAY_GET_V(NAME, T) \
    DEFINE_STRUCT_ARRAY_GET_VL(NAME, T) \
    DEFINE_STRUCT_ARRAY_GET_VA(NAME, T) \
    \
    DEFINE_STRUCT_ARRAY_SET(NAME, T) \
    DEFINE_STRUCT_ARRAY_SET_V(NAME, T) \
    DEFINE_STRUCT_ARRAY_SET_VL(NAME, T) \
    DEFINE_STRUCT_ARRAY_SET_VA(NAME, T) \

    \
    DEFINE_STRUCT_ARRAY_FOR_EACH(NAME, T) \
    DEFINE_STRUCT_ARRAY_FOR_EACH_DATA(NAME, T) \
    DEFINE_STRUCT_ARRAY_FOR_EACH_INDEX(NAME, T) \
    DEFINE_STRUCT_ARRAY_FOR_EACH_INDEX_DATA(NAME, T) \
    DEFINE_STRUCT_ARRAY_MAP(NAME, T) \
    DEFINE_STRUCT_ARRAY_MAP_DATA(NAME, T) \

    \
    DEFINE_STRUCT_ARRAY_FLUSH_LEFT(NAME, T) \
    DEFINE_STRUCT_ARRAY_FLUSH_RIGHT(NAME, T) \
    DEFINE_STRUCT_ARRAY_SHIFT_LEFT(NAME, T) \
    DEFINE_STRUCT_ARRAY_SHIFT_RIGHT(NAME, T) \
    DEFINE_STRUCT_ARRAY_ROTATE_LEFT(NAME, T) \
    DEFINE_STRUCT_ARRAY_ROTATE_RIGHT(NAME, T) \
    DEFINE_STRUCT_ARRAY_SORT_BY(NAME, T) \
    DEFINE_STRUCT_ARRAY_SHUFFLE(NAME, T) \
    DEFINE_STRUCT_ARRAY_REVERSE(NAME, T) \
*/
/*
 * CUT
 * REVERSE
 * FILL
 */


