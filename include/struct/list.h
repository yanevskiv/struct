/**
 * (c) Ivan Janevski
 *
 * Linked List
 */

#include "common.h"

// define: typedef
#define DEFINE_STRUCT_LIST_TYPEDEF(NAME, T) \
    typedef struct NAME NAME##_t; \
    typedef struct NAME##_elem NAME##_elem_t;

// define: struct
#define DEFINE_STRUCT_LIST_STRUCT(NAME, T) \
    struct NAME##_elem { \
        struct NAME##_elem *le_prev; \
        struct NAME##_elem *le_next; \
        T le_data; \
    }; \
    struct NAME { \
        struct NAME##_elem *l_head; \
        struct NAME##_elem *l_tail; \
        STRUCT_MUTEX_DECLARE(l_mutex); \
        STRUCT_MT_SAFE_DECLARE(l_mt_safe); \
    };

// define: new
#define DEFINE_STRUCT_LIST_NEW(NAME, T) \
    STRUCT_ATTRIB struct NAME *NAME##_new()\
    { \
        struct NAME *_list = (struct NAME*) STRUCT_CALLOC(1, struct NAME); \
        _list->l_head = STRUCT_NULL; \
        _list->l_tail = STRUCT_NULL; \
        STRUCT_MUTEX_INIT(_list->l_mutex); \
        STRUCT_MT_SAFE_INIT(_list->l_mt_safe); \
        return _list; \
    }

// define init
#define DEFINE_STRUCT_LIST_INIT(NAME, T) \
    STRUCT_ATTRIB struct NAME *NAME##_init(struct NAME *_list)\
    { \
        _list->l_head = STRUCT_NULL; \
        _list->l_tail = STRUCT_NULL; \
        STRUCT_MUTEX_INIT(_list->l_mutex); \
        STRUCT_MT_SAFE_INIT(_list->l_mt_safe); \
        return _list; \
    }

// define: destroy
#define DEFINE_STRUCT_LIST_DESTROY(NAME, T) \
    STRUCT_ATTRIB void NAME##_destroy(struct NAME *_list)\
    { \
        struct NAME##_elem *_iter = _list->l_head, *_next; \
        while (_iter) { \
            _next = _iter->le_next; \
            STRUCT_FREE(_iter); \
            _iter = _next; \
        } \
        _list->l_head = STRUCT_NULL; \
        _list->l_tail = STRUCT_NULL; \
        STRUCT_MUTEX_DESTROY(_list->l_mutex); \
        STRUCT_MT_SAFE_DESTROY(_list->l_mt_safe); \
    }

// define: free
#define DEFINE_STRUCT_LIST_FREE(NAME, T) \
    STRUCT_ATTRIB void NAME##_free(struct NAME *_list)\
    { \
        struct NAME##_elem *_iter = _list->l_head, *_next; \
        while (_iter) { \
            _next = _iter->le_next; \
            STRUCT_FREE(_iter); \
            _iter = _next; \
        } \
        STRUCT_MUTEX_DESTROY(_list->l_mutex); \
        STRUCT_MT_SAFE_DESTROY(_list->l_mt_safe); \
        STRUCT_FREE(_list); \
   }

// define: head
#define DEFINE_STRUCT_LIST_HEAD(NAME, T) \
    STRUCT_ATTRIB struct NAME##_elem *NAME##_head(struct NAME *_list)\
    { \
        struct NAME##_elem *_return; \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        _return = _list->l_head; \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
        return _return; \
    }

// define: tail
#define DEFINE_STRUCT_LIST_TAIL(NAME, T) \
    STRUCT_ATTRIB struct NAME##_elem *NAME##_tail(struct NAME *_list)\
    { \
        struct NAME##_elem *_return; \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        _return = _list->l_tail; \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
        return _return; \
    }

// define: lock
#define DEFINE_STRUCT_LIST_LOCK(NAME, T) \
    STRUCT_ATTRIB void NAME##_lock(struct NAME *_list) \
    { \
        STRUCT_MUTEX_LOCK(_list->l_mutex); \
    }

// define: unlock
#define DEFINE_STRUCT_LIST_UNLOCK(NAME, T) \
    STRUCT_ATTRIB void NAME##_unlock(struct NAME *_list) \
    { \
        STRUCT_MUTEX_UNLOCK(_list->l_mutex); \
    }

// define: mt_safe_lock
#define DEFINE_STRUCT_LIST_MT_SAFE_LOCK(NAME, T) \
    STRUCT_ATTRIB void NAME##_mt_safe_lock(struct NAME *_list) \
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mutex); \
    }

// define: mt_safe_unlock
#define DEFINE_STRUCT_LIST_MT_SAFE_UNLOCK(NAME, T) \
    STRUCT_ATTRIB void NAME##_mt_safe_unlock(struct NAME *_list) \
    { \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mutex); \
    }

// define: elem_new
#define DEFINE_STRUCT_LIST_ELEM_NEW(NAME, T) \
    STRUCT_ATTRIB struct NAME##_elem *NAME##_elem_new(T _data)\
    { \
        struct NAME##_elem *_elem = (struct NAME##_elem*) STRUCT_CALLOC(1, struct NAME##_elem); \
        _elem->le_next = STRUCT_NULL; \
        _elem->le_prev = STRUCT_NULL; \
        STRUCT_ASSIGN(_elem->le_data, _data); \
        return _elem; \
    }

// define: elem_init
#define DEFINE_STRUCT_LIST_ELEM_INIT(NAME, T) \
    STRUCT_ATTRIB void NAME##_elem_init(struct NAME##_elem *_elem , T _data)\
    { \
        _elem->le_next = STRUCT_NULL; \
        _elem->le_prev = STRUCT_NULL; \
        STRUCT_ASSIGN(_elem->le_data, _data); \
    }

// define: elem_destroy
#define DEFINE_STRUCT_LIST_ELEM_DESTROY(NAME, T) \
    STRUCT_ATTRIB void NAME##_elem_destroy(struct NAME##_elem *_elem)\
    { \
        _elem->le_prev = STRUCT_NULL; \
        _elem->le_next = STRUCT_NULL; \
    }

// define: elem_free
#define DEFINE_STRUCT_LIST_ELEM_FREE(NAME, T) \
    STRUCT_ATTRIB void NAME##_elem_free(struct NAME##_elem *_elem)\
    { \
        _elem->le_prev = STRUCT_NULL; \
        _elem->le_next = STRUCT_NULL; \
        STRUCT_FREE(_elem); \
    }

// define: elem_prev
#define DEFINE_STRUCT_LIST_ELEM_PREV(NAME, T) \
    STRUCT_ATTRIB struct NAME##_elem *NAME##_elem_prev(struct NAME##_elem *_elem)\
    { \
        return _elem->le_prev; \
    }

// define: elem_next
#define DEFINE_STRUCT_LIST_ELEM_NEXT(NAME, T) \
    STRUCT_ATTRIB struct NAME##_elem *NAME##_elem_next(struct NAME##_elem *_elem)\
    { \
        return _elem->le_next; \
    }

// define: elem_data
#define DEFINE_STRUCT_LIST_ELEM_DATA(NAME, T) \
    STRUCT_ATTRIB T NAME##_elem_data(struct NAME##_elem *_elem)\
    { \
        return _elem->le_data; \
    }

// define: elem_set
#define DEFINE_STRUCT_LIST_ELEM_SET(NAME, T) \
    STRUCT_ATTRIB void NAME##_elem_set(struct NAME##_elem *_elem, T _data)\
    { \
        STRUCT_ASSIGN(_elem->le_data, _data); \
    }

// define: empty
#define DEFINE_STRUCT_LIST_EMPTY(NAME, T) \
    STRUCT_ATTRIB int NAME##_empty(struct NAME *_list)\
    { \
        return _list->l_head == STRUCT_NULL; \
    }

// define: count
#define DEFINE_STRUCT_LIST_COUNT(NAME, T) \
    STRUCT_ATTRIB int NAME##_count(struct NAME *_list)\
    { \
        int _count = 0; \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME##_elem *_iter = _list->l_head; \
        while (_iter) { \
            _count += 1; \
            _iter = _iter->le_next; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
        return _count; \
    }

// define: clone
#define DEFINE_STRUCT_LIST_CLONE(NAME, T)  \
    STRUCT_ATTRIB struct NAME *NAME##_clone(struct NAME *_list)\
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME *_clone = (struct NAME*) STRUCT_CALLOC(1, struct NAME); \
        _clone->l_head = STRUCT_NULL; \
        _clone->l_tail = STRUCT_NULL; \
        STRUCT_MUTEX_INIT(_clone->l_mutex); \
        STRUCT_MT_SAFE_INIT(_clone->l_mt_safe); \
        struct NAME##_elem *_iter = _list->l_head, *_elem = STRUCT_NULL; \
        while (_iter) { \
            _elem = (struct NAME##_elem*) STRUCT_CALLOC(1, struct NAME); \
            _elem->le_next = STRUCT_NULL; \
            _elem->le_prev = STRUCT_NULL; \
            STRUCT_ASSIGN(_elem->le_data, _iter->le_data); \
            if (_clone->l_tail) { \
                _clone->l_tail->le_next = _elem; \
                _elem->le_prev = _clone->l_tail; \
                _clone->l_tail = _elem; \
            } else { \
                _clone->l_head = _elem; \
                _clone->l_tail = _elem; \
            } \
            _iter = _iter->le_next; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
        return _clone; \
    }

// define: clear
#define DEFINE_STRUCT_LIST_CLEAR(NAME, T) \
    STRUCT_ATTRIB void NAME##_clear(struct NAME *_list) { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME##_elem *_iter = _list->l_head, *_next = STRUCT_NULL; \
        while (_iter) { \
            _next = _iter->le_next; \
            STRUCT_FREE(_iter); \
            _iter = _next; \
        } \
        _list->l_head = STRUCT_NULL; \
        _list->l_tail = STRUCT_NULL; \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
    }

// body: void list_push_head(NAME *_list, T *_data)
#define STRUCT_BODY_LIST_PUSH_HEAD(NAME, T) \
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME##_elem *_elem = (struct NAME##_elem*) STRUCT_CALLOC(1, struct NAME##_elem); \
        _elem->le_prev = STRUCT_NULL; \
        _elem->le_next = STRUCT_NULL; \
        STRUCT_ASSIGN(_elem->le_data, _data); \
        if (_list->l_head) { \
            _elem->le_next = _list->l_head; \
            _list->l_head->le_prev = _elem; \
            _list->l_head = _elem; \
        } else { \
            _list->l_head = _elem; \
            _list->l_tail = _elem; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
    }

// body: void list_push_tail(NAME *_list, T *_data)
#define STRUCT_BODY_LIST_PUSH_TAIL(NAME, T) \
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME##_elem *_elem = (struct NAME##_elem*) STRUCT_CALLOC(1, struct NAME##_elem); \
        _elem->le_prev = STRUCT_NULL; \
        _elem->le_next = STRUCT_NULL; \
        STRUCT_ASSIGN(_elem->le_data, _data); \
        if (_list->l_tail) { \
            _elem->le_prev = _list->l_tail; \
            _list->l_tail->le_next = _elem; \
            _list->l_tail = _elem; \
        } else { \
            _list->l_tail = _elem; \
            _list->l_head = _elem; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
    }

// body: int list_pop_head(NAME *_list, T *_data)
#define STRUCT_BODY_LIST_POP_HEAD(NAME, T) \
    { \
        int _count = 0; \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME##_elem *_elem = _list->l_head; \
        if (_elem) { \
            if (_list->l_tail == _list->l_head)  { \
                _list->l_tail = STRUCT_NULL; \
                _list->l_head = STRUCT_NULL; \
            } else { \
                _list->l_head = _list->l_head->le_next; \
                _list->l_head->le_prev = STRUCT_NULL; \
            } \
            if (_return) { \
                STRUCT_ASSIGN(*_return, _elem->le_data); \
            } \
            STRUCT_FREE(_elem); \
            _count = 1; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
        return _count; \
    }

// body: int list_pop_tail(NAME *_list, T *_data)
#define STRUCT_BODY_LIST_POP_TAIL(NAME, T) \
    { \
        int _count = 0; \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME##_elem *_elem = _list->l_tail; \
        if (_elem) { \
            if (_list->l_tail == _list->l_head) { \
                _list->l_tail = STRUCT_NULL; \
                _list->l_head = STRUCT_NULL; \
            } else { \
                _list->l_tail = _list->l_tail->le_prev; \
                _list->l_tail->le_next = STRUCT_NULL; \
            } \
            if (_return) { \
                *_return = _elem->le_data; \
            } \
            STRUCT_FREE(_elem); \
            _count = 1; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
        return _count; \
    }

// body: void list_push_elem_head(NAME *_list, NAME##_elem *_elem)
#define STRUCT_BODY_LIST_PUSH_ELEM_HEAD(NAME, T) \
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        _elem->le_prev = STRUCT_NULL; \
        _elem->le_next = STRUCT_NULL; \
        if (_list->l_head) { \
            _elem->le_next = _list->l_head; \
            _list->l_head->le_prev = _elem; \
            _list->l_head = _elem; \
        } else { \
            _list->l_head = _elem; \
            _list->l_tail = _elem; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
    }

// body: void list_push_elem_tail(NAME *_list, NAME##_elem *_elem)
#define STRUCT_BODY_LIST_PUSH_ELEM_TAIL(NAME, T) \
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        _elem->le_prev = STRUCT_NULL; \
        _elem->le_next = STRUCT_NULL; \
        if (_list->l_tail) { \
            _elem->le_prev = _list->l_tail; \
            _list->l_tail->le_next = _elem; \
            _list->l_tail = _elem; \
        } else { \
            _list->l_tail = _elem; \
            _list->l_head = _elem; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
    }

// body: NAME##_elem *list_pop_elem_head(NAME *_list)
#define STRUCT_BODY_LIST_POP_ELEM_HEAD(NAME, T) \
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME##_elem *_elem = _list->l_head; \
        if (_elem) { \
            if (_list->l_tail == _list->l_head)  { \
                _list->l_tail = STRUCT_NULL; \
                _list->l_head = STRUCT_NULL; \
            } else { \
                _list->l_head = _list->l_head->le_next; \
                _list->l_head->le_prev = STRUCT_NULL; \
            } \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
        return _elem; \
    }

// body: NAME##_elem *list_pop_elem_tail(NAME *_list)
#define STRUCT_BODY_LIST_POP_ELEM_TAIL(NAME, T) \
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME##_elem *_elem = _list->l_tail; \
        if (_elem) { \
            if (_list->l_tail == _list->l_head) { \
                _list->l_tail = STRUCT_NULL; \
                _list->l_head = STRUCT_NULL; \
            } else { \
                _list->l_tail = _list->l_tail->le_prev; \
                _list->l_tail->le_next = STRUCT_NULL; \
            } \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
        return _elem; \
    }

// body: list_push_head_va(NAME *_list, T *_key, ...)
#define STRUCT_BODY_LIST_PUSH_HEAD_VA(NAME, T) \
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        va_list _va; \
        va_start(_va, _key); \
        T _data = va_arg(_va, T); \
        while (! (STRUCT_EQUALS(_data, _key))) { \
            struct NAME##_elem *_elem = (struct NAME##_elem*) STRUCT_CALLOC(1, struct NAME##_elem); \
            _elem->le_prev = STRUCT_NULL; \
            _elem->le_next = STRUCT_NULL; \
            STRUCT_ASSIGN(_elem->le_data, _data); \
            if (_list->l_head) { \
                _elem->le_next = _list->l_head; \
                _list->l_head->le_prev = _elem; \
                _list->l_head = _elem; \
            } else { \
                _list->l_head = _elem; \
                _list->l_tail = _elem; \
            } \
            _data = va_arg(_va, T); \
        } \
        va_end(_va); \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
    }


// body: list_push_tail_va(NAME *_list, T *_key, ...)
#define STRUCT_BODY_LIST_PUSH_TAIL_VA(NAME, T) \
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        va_list _va; \
        va_start(_va, _key); \
        T _data = va_arg(_va, T); \
        while (! (STRUCT_EQUALS(_data, _key))) { \
            struct NAME##_elem *_elem = (struct NAME##_elem*) STRUCT_CALLOC(1, struct NAME##_elem); \
            _elem->le_prev = STRUCT_NULL; \
            _elem->le_next = STRUCT_NULL; \
            STRUCT_ASSIGN(_elem->le_data, _data); \
            if (_list->l_tail) { \
                _elem->le_prev = _list->l_tail; \
                _list->l_tail->le_next = _elem; \
                _list->l_tail = _elem; \
            } else { \
                _list->l_tail = _elem; \
                _list->l_head = _elem; \
            } \
            _data = va_arg(_va, T); \
        } \
        va_end(_va); \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
    }

// body: list_pop_head_va(NAME *_list, ...)
#define STRUCT_BODY_LIST_POP_HEAD_VA(NAME, T) \
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        int _count = 0; \
        va_list _va; \
        va_start(_va, _list); \
        while (_list->l_head)  { \
            T* _return = va_arg(_va, T*); \
            if (! _return) {\
                break; \
            } \
            struct NAME##_elem *_elem = _list->l_head; \
            if (_list->l_tail == _list->l_head) { \
                _list->l_head = STRUCT_NULL; \
                _list->l_tail = STRUCT_NULL; \
            } else { \
                _list->l_head = _list->l_head->le_next; \
                _list->l_head->le_prev = STRUCT_NULL; \
            } \
            STRUCT_ASSIGN(*_return, _elem->le_data); \
            STRUCT_FREE(_elem); \
            _count += 1; \
        } \
        va_end(_va); \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
        return _count; \
    }

// body: list_pop_tail_va(NAME *_list, ...)
#define STRUCT_BODY_LIST_POP_TAIL_VA(NAME, T)\
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        int _count = 0; \
        va_list _va; \
        va_start(_va, _list); \
        while (_list->l_tail)  { \
            T* _return = va_arg(_va, T*); \
            if (! _return) {\
                break; \
            } \
            struct NAME##_elem *_elem = _list->l_tail; \
            if (_list->l_tail == _list->l_head) { \
                _list->l_tail = STRUCT_NULL; \
                _list->l_head = STRUCT_NULL; \
            } else { \
                _list->l_tail = _list->l_tail->le_prev; \
                _list->l_tail->le_next = STRUCT_NULL; \
            } \
            STRUCT_ASSIGN(*_return, _elem->le_data); \
            STRUCT_FREE(_elem); \
            _count += 1; \
        } \
        va_end(_va); \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
        return _count; \
    }

// body: void list_push_elem_head_va(NAME *_list, ...)
#define STRUCT_BODY_LIST_PUSH_ELEM_HEAD_VA(NAME, T) \
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        va_list _va; \
        va_start(_va, _list); \
        struct NAME##_elem *_elem = va_arg(_va, struct NAME##_elem*); \
        while (_elem) { \
            _elem->le_prev = STRUCT_NULL; \
            _elem->le_next = STRUCT_NULL; \
            if (! _list->l_head) { \
                _list->l_head = _elem; \
                _list->l_tail = _elem; \
            } else { \
                _elem->le_next = _list->l_head; \
                _list->l_head->le_prev = _elem; \
                _list->l_head = _elem; \
            } \
            _elem = va_arg(_va, struct NAME##_elem*); \
        } \
        va_end(_va); \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
     }

// body: void list_push_elem_tail_va(NAME *_list, ...)
#define STRUCT_BODY_LIST_PUSH_ELEM_TAIL_VA(NAME, T) \
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        va_list _va; \
        va_start(_va, _list); \
        struct NAME##_elem *_elem = va_arg(_va, struct NAME##_elem*); \
        while (_elem) { \
            _elem->le_prev = STRUCT_NULL; \
            _elem->le_next = STRUCT_NULL; \
            if (! _list->l_tail) { \
                _list->l_head = _elem; \
                _list->l_tail = _elem; \
            } else { \
                _elem->le_prev = _list->l_tail; \
                _list->l_tail->le_next = _elem; \
                _list->l_tail = _elem; \
            } \
            _elem = va_arg(_va, struct NAME##_elem*); \
        } \
        va_end(_va); \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
     }


// body: int list_pop_elem_head_va(NAME *_list, ...)
#define STRUCT_BODY_LIST_POP_ELEM_HEAD_VA(NAME, T) \
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        va_list _va; \
        va_start(_va, _list); \
        int _count = 0; \
        struct NAME##_elem *_elem, **_return = va_arg(_va, struct NAME##_elem**); \
        while (_return) { \
            if (! _list->l_head) { \
               *_return = STRUCT_NULL; \
            } else { \
                _elem = _list->l_head; \
                if (_list->l_head == _list->l_tail)  { \
                    _list->l_head = STRUCT_NULL; \
                    _list->l_tail = STRUCT_NULL; \
                } else { \
                    _list->l_head = _list->l_head->le_next; \
                    _list->l_head->le_prev = STRUCT_NULL; \
                } \
                _elem->le_prev = STRUCT_NULL; \
                _elem->le_next = STRUCT_NULL; \
                *_return = _elem; \
                _count += 1; \
            } \
            _return = va_arg(_va, struct NAME##_elem**); \
        } \
        va_end(_va); \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
        return _count; \
    }

// body list_pop_elem_tail_va(NAME *_list, ...)
#define STRUCT_BODY_LIST_POP_ELEM_TAIL_VA(NAME, T) \
     { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        va_list _va; \
        va_start(_va, _list); \
        int _count = 0; \
        struct NAME##_elem *_elem, **_return = va_arg(_va, struct NAME##_elem**); \
        while (_return) { \
            if (! _list->l_tail) { \
               *_return = STRUCT_NULL; \
            } else { \
                _elem = _list->l_tail; \
                if (_list->l_head == _list->l_tail)  { \
                    _list->l_head = STRUCT_NULL; \
                    _list->l_tail = STRUCT_NULL; \
                } else { \
                    _list->l_tail = _list->l_tail->le_prev; \
                    _list->l_tail->le_next = STRUCT_NULL; \
                } \
                _elem->le_prev = STRUCT_NULL; \
                _elem->le_next = STRUCT_NULL; \
                *_return = _elem; \
                _count += 1; \
            } \
            _return = va_arg(_va, struct NAME##_elem**); \
        } \
        va_end(_va); \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
        return _count; \
     }

// define double_queue
#if defined(STRUCT_DOUBLE_QUEUE)
    // define: enqueue_head
    #define DEFINE_STRUCT_LIST_ENQUEUE_HEAD(NAME, T) \
        STRUCT_ATTRIB void NAME##_enqueue_head(struct NAME *_list, T _data) \
        STRUCT_BODY_LIST_PUSH_HEAD(NAME, T)

    // define: enqueue_tail
    #define DEFINE_STRUCT_LIST_ENQUEUE_TAIL(NAME, T) \
        STRUCT_ATTRIB void NAME##_enqueue_tail(struct NAME *_list, T _data) \
        STRUCT_BODY_LIST_PUSH_TAIL(NAME, T)

    // define: dequeue_tail
    #define DEFINE_STRUCT_LIST_DEQUEUE_TAIL(NAME, T)\
        STRUCT_ATTRIB int NAME##_dequeue_tail(struct NAME *_list, T *_return) \
        STRUCT_BODY_LIST_POP_TAIL(NAME, T)

    // define: dequeue_head
    #define DEFINE_STRUCT_LIST_DEQUEUE_HEAD(NAME, T) \
        STRUCT_ATTRIB int NAME##_dequeue_head(struct NAME *_list, T *_return) \
        STRUCT_BODY_LIST_POP_HEAD(NAME, T)

    // define: enqueue_elem_head
    #define DEFINE_STRUCT_LIST_ENQUEUE_ELEM_HEAD(NAME, T) \
        STRUCT_ATTRIB void NAME##_enqueue_elem_head(struct NAME *_list, struct NAME##_elem *_elem) \
        STRUCT_BODY_LIST_PUSH_ELEM_HEAD(NAME, T)

    // define: enqueue_elem_tail
    #define DEFINE_STRUCT_LIST_ENQUEUE_ELEM_TAIL(NAME, T) \
        STRUCT_ATTRIB void NAME##_enqueue_elem_tail(struct NAME *_list, struct NAME##_elem *_elem) \
        STRUCT_BODY_LIST_PUSH_ELEM_TAIL(NAME, T)

    // define: dequeue_elem_tail
    #define DEFINE_STRUCT_LIST_DEQUEUE_ELEM_TAIL(NAME, T) \
        STRUCT_ATTRIB struct NAME##_elem *NAME##_dequeue_elem_tail(struct NAME *_list) \
        STRUCT_BODY_LIST_POP_ELEM_TAIL(NAME, T)

    // define: dequeue_elem_head
    #define DEFINE_STRUCT_LIST_DEQUEUE_ELEM_HEAD(NAME, T) \
        STRUCT_ATTRIB struct NAME##_elem *NAME##_dequeue_elem_head(struct NAME *_list) \
        STRUCT_BODY_LIST_POP_ELEM_HEAD(NAME, T)

    // define double_queue_va
    #if defined(STRUCT_VA)
        // define: enqueue_head_va
        #define DEFINE_STRUCT_LIST_ENQUEUE_HEAD_VA(NAME, T) \
            STRUCT_ATTRIB void NAME##_enqueue_head_va(struct NAME *_list, T _key, ...) \
            STRUCT_BODY_LIST_PUSH_HEAD_VA(NAME, T)

        // define: enqueue_tail_va
        #define DEFINE_STRUCT_LIST_ENQUEUE_TAIL_VA(NAME, T) \
            STRUCT_ATTRIB void NAME##_enqueue_tail_va(struct NAME *_list, T _key, ...) \
            STRUCT_BODY_LIST_PUSH_TAIL_VA(NAME, T)

        // define: dequeue_head_va
        #define DEFINE_STRUCT_LIST_DEQUEUE_HEAD_VA(NAME, T) \
            STRUCT_ATTRIB int NAME##_dequeue_head_va(struct NAME *_list, ...) \
            STRUCT_BODY_LIST_POP_HEAD_VA(NAME, T)

        // define: dequeue_tail_va
        #define DEFINE_STRUCT_LIST_DEQUEUE_TAIL_VA(NAME, T) \
            STRUCT_ATTRIB int NAME##_dequeue_tail_va(struct NAME *_list, ...) \
            STRUCT_BODY_LIST_POP_TAIL_VA(NAME, T)

        // define: enqueue_elem_head_va
        #define DEFINE_STRUCT_LIST_ENQUEUE_ELEM_HEAD_VA(NAME, T) \
            STRUCT_ATTRIB void NAME##_enqueue_elem_head_va(struct NAME *_list, ...) \
            STRUCT_BODY_LIST_PUSH_ELEM_HEAD_VA(NAME, T)

        // define: enqueue_elem_tail_va
        #define DEFINE_STRUCT_LIST_ENQUEUE_ELEM_TAIL_VA(NAME, T) \
            STRUCT_ATTRIB void NAME##_enqueue_elem_tail_va(struct NAME *_list, ...) \
            STRUCT_BODY_LIST_PUSH_ELEM_TAIL_VA(NAME, T)

        // define: dequeue_elem_head_va
        #define DEFINE_STRUCT_LIST_DEQUEUE_ELEM_HEAD_VA(NAME, T) \
            STRUCT_ATTRIB int NAME##_dequeue_elem_head_va(struct NAME *_list, ...) \
            STRUCT_BODY_LIST_POP_ELEM_HEAD_VA(NAME, T)

        // define: dequeue_elem_tail_va
        #define DEFINE_STRUCT_LIST_DEQUEUE_ELEM_TAIL_VA(NAME, T) \
            STRUCT_ATTRIB int NAME##_dequeue_elem_tail_va(struct NAME *_list, ...) \
            STRUCT_BODY_LIST_POP_ELEM_TAIL_VA(NAME, T)
    #endif
#endif

#if !defined(STRUCT_DOUBLE_QUEUE)
    // define: enqueue_head
    #define DEFINE_STRUCT_LIST_ENQUEUE_HEAD(NAME, T)
    // define: enqueue_tail
    #define DEFINE_STRUCT_LIST_ENQUEUE_TAIL(NAME, T)
    // define: dequeue_head
    #define DEFINE_STRUCT_LIST_DEQUEUE_HEAD(NAME, T)
    // define: dequeue_tail
    #define DEFINE_STRUCT_LIST_DEQUEUE_TAIL(NAME, T)

    // define: enqueue_elem_head
    #define DEFINE_STRUCT_LIST_ENQUEUE_ELEM_HEAD(NAME, T)
    // define: enqueue_elem_tail
    #define DEFINE_STRUCT_LIST_ENQUEUE_ELEM_TAIL(NAME, T)
    // define: dequeue_elem_head
    #define DEFINE_STRUCT_LIST_DEQUEUE_ELEM_HEAD(NAME, T)
    // define: dequeue_elem_tail
    #define DEFINE_STRUCT_LIST_DEQUEUE_ELEM_TAIL(NAME, T)
#endif

#if !defined(STRUCT_DOUBLE_QUEUE) || !defined(STRUCT_VA)
    // define: enqueue_head_va
    #define DEFINE_STRUCT_LIST_ENQUEUE_HEAD_VA(NAME, T)
    // define: enqueue_tail_va
    #define DEFINE_STRUCT_LIST_ENQUEUE_TAIL_VA(NAME, T)
    // define: dequeue_head_va
    #define DEFINE_STRUCT_LIST_DEQUEUE_HEAD_VA(NAME, T)
    // define: dequeue_tail_va
    #define DEFINE_STRUCT_LIST_DEQUEUE_TAIL_VA(NAME, T)

    // define: enqueue_elem_head_va
    #define DEFINE_STRUCT_LIST_ENQUEUE_ELEM_HEAD_VA(NAME, T)
    // define: enqueue_elem_tail_va
    #define DEFINE_STRUCT_LIST_ENQUEUE_ELEM_TAIL_VA(NAME, T)
    // define: dequeue_elem_head_va
    #define DEFINE_STRUCT_LIST_DEQUEUE_ELEM_HEAD_VA(NAME, T)
    // define: dequeue_elem_tail_va
    #define DEFINE_STRUCT_LIST_DEQUEUE_ELEM_TAIL_VA(NAME, T)
#endif

// define double_stack
#if defined(STRUCT_DOUBLE_STACK)
    // define: push_head
    #define DEFINE_STRUCT_LIST_PUSH_HEAD(NAME, T) \
        STRUCT_ATTRIB void NAME##_push_head(struct NAME *_list, T _data) \
        STRUCT_BODY_LIST_PUSH_HEAD(NAME, T)

    // define: push_tail
    #define DEFINE_STRUCT_LIST_PUSH_TAIL(NAME, T) \
        STRUCT_ATTRIB void NAME##_push_tail(struct NAME *_list, T _data) \
        STRUCT_BODY_LIST_PUSH_TAIL(NAME, T)

    // define: pop_tail
    #define DEFINE_STRUCT_LIST_POP_TAIL(NAME, T)\
        STRUCT_ATTRIB int NAME##_pop_tail(struct NAME *_list, T *_return) \
        STRUCT_BODY_LIST_POP_TAIL(NAME, T)

    // define: pop_head
    #define DEFINE_STRUCT_LIST_POP_HEAD(NAME, T) \
        STRUCT_ATTRIB int NAME##_pop_head(struct NAME *_list, T *_return) \
        STRUCT_BODY_LIST_POP_HEAD(NAME, T)

    // define: push_elem_head
    #define DEFINE_STRUCT_LIST_PUSH_ELEM_HEAD(NAME, T) \
        STRUCT_ATTRIB void NAME##_push_elem_head(struct NAME *_list, struct NAME##_elem *_elem) \
        STRUCT_BODY_LIST_PUSH_ELEM_HEAD(NAME, T)

    // define: push_elem_tail
    #define DEFINE_STRUCT_LIST_PUSH_ELEM_TAIL(NAME, T) \
        STRUCT_ATTRIB void NAME##_push_elem_tail(struct NAME *_list, struct NAME##_elem *_elem) \
        STRUCT_BODY_LIST_PUSH_ELEM_TAIL(NAME, T)

    // define: pop_elem_tail
    #define DEFINE_STRUCT_LIST_POP_ELEM_TAIL(NAME, T) \
        STRUCT_ATTRIB struct NAME##_elem *NAME##_pop_elem_tail(struct NAME *_list) \
        STRUCT_BODY_LIST_POP_ELEM_TAIL(NAME, T)

    // define: pop_elem_head
    #define DEFINE_STRUCT_LIST_POP_ELEM_HEAD(NAME, T) \
        STRUCT_ATTRIB struct NAME##_elem *NAME##_pop_elem_head(struct NAME *_list) \
        STRUCT_BODY_LIST_POP_ELEM_HEAD(NAME, T)

    // define double_stack_va
    #if defined(STRUCT_VA)
        // define: push_head_va
        #define DEFINE_STRUCT_LIST_PUSH_HEAD_VA(NAME, T) \
            STRUCT_ATTRIB void NAME##_push_head_va(struct NAME *_list, T _key, ...) \
            STRUCT_BODY_LIST_PUSH_HEAD_VA(NAME, T)

        // define: pop_tail_va
        #define DEFINE_STRUCT_LIST_PUSH_TAIL_VA(NAME, T) \
            STRUCT_ATTRIB void NAME##_push_tail_va(struct NAME *_list, T _key, ...) \
            STRUCT_BODY_LIST_PUSH_TAIL_VA(NAME, T)

        // define: pop_head_va
        #define DEFINE_STRUCT_LIST_POP_HEAD_VA(NAME, T) \
            STRUCT_ATTRIB int NAME##_pop_head_va(struct NAME *_list, ...) \
            STRUCT_BODY_LIST_POP_HEAD_VA(NAME, T)

        // define: pop_tail_va
        #define DEFINE_STRUCT_LIST_POP_TAIL_VA(NAME, T) \
            STRUCT_ATTRIB int NAME##_pop_tail_va(struct NAME *_list, ...) \
            STRUCT_BODY_LIST_POP_TAIL_VA(NAME, T)

        // define: push_head_va
        #define DEFINE_STRUCT_LIST_PUSH_ELEM_HEAD_VA(NAME, T) \
            STRUCT_ATTRIB void NAME##_push_elem_head_va(struct NAME *_list, ...) \
            STRUCT_BODY_LIST_PUSH_ELEM_HEAD_VA(NAME, T)

        // define: pop_tail_va
        #define DEFINE_STRUCT_LIST_PUSH_ELEM_TAIL_VA(NAME, T) \
            STRUCT_ATTRIB void NAME##_push_elem_tail_va(struct NAME *_list, ...) \
            STRUCT_BODY_LIST_PUSH_ELEM_TAIL_VA(NAME, T)

        // define: pop_head_va
        #define DEFINE_STRUCT_LIST_POP_ELEM_HEAD_VA(NAME, T) \
            STRUCT_ATTRIB int NAME##_pop_elem_head_va(struct NAME *_list, ...) \
            STRUCT_BODY_LIST_POP_ELEM_HEAD_VA(NAME, T)

        // define: pop_tail_va
        #define DEFINE_STRUCT_LIST_POP_ELEM_TAIL_VA(NAME, T) \
            STRUCT_ATTRIB int NAME##_pop_elem_tail_va(struct NAME *_list, ...) \
            STRUCT_BODY_LIST_POP_ELEM_TAIL_VA(NAME, T)
    #endif
#endif

#if !defined(STRUCT_DOUBLE_STACK)
    // define: push_head
    #define DEFINE_STRUCT_LIST_PUSH_HEAD(NAME, T)
    // define: push_tail
    #define DEFINE_STRUCT_LIST_PUSH_TAIL(NAME, T)
    // define: pop_head
    #define DEFINE_STRUCT_LIST_POP_HEAD(NAME, T)
    // define: pop_tail
    #define DEFINE_STRUCT_LIST_POP_TAIL(NAME, T)

    // define: push_elem_head
    #define DEFINE_STRUCT_LIST_PUSH_ELEM_HEAD(NAME, T)
    // define: push_elem_tail
    #define DEFINE_STRUCT_LIST_PUSH_ELEM_TAIL(NAME, T)
    // define: pop_elem_head
    #define DEFINE_STRUCT_LIST_POP_ELEM_HEAD(NAME, T)
    // define: pop_elem_tail
    #define DEFINE_STRUCT_LIST_POP_ELEM_TAIL(NAME, T)
#endif

#if !defined(STRUCT_DOUBLE_STACK) || !defined(STRUCT_VA)
    // define: push_head_va
    #define DEFINE_STRUCT_LIST_PUSH_HEAD_VA(NAME, T)
    // define: push_tail_va
    #define DEFINE_STRUCT_LIST_PUSH_TAIL_VA(NAME, T)
    // define: pop_head_va
    #define DEFINE_STRUCT_LIST_POP_HEAD_VA(NAME, T)
    // define: pop_tail_va
    #define DEFINE_STRUCT_LIST_POP_TAIL_VA(NAME, T)

    // define: push_elem_head_va
    #define DEFINE_STRUCT_LIST_PUSH_ELEM_HEAD_VA(NAME, T)
    // define: push_elem_tail_va
    #define DEFINE_STRUCT_LIST_PUSH_ELEM_TAIL_VA(NAME, T)
    // define: pop_elem_head_va
    #define DEFINE_STRUCT_LIST_POP_ELEM_HEAD_VA(NAME, T)
    // define: pop_elem_tail_va
    #define DEFINE_STRUCT_LIST_POP_ELEM_TAIL_VA(NAME, T)
#endif


#if defined(STRUCT_QUEUE)
    // define: enqueue
    #define DEFINE_STRUCT_LIST_ENQUEUE(NAME, T) \
        STRUCT_ATTRIB void NAME##_enqueue(struct NAME *_list, T _data) \
        STRUCT_BODY_LIST_PUSH_TAIL(NAME, T)

    // define: dequeue
    #define DEFINE_STRUCT_LIST_DEQUEUE(NAME, T) \
        STRUCT_ATTRIB int NAME##_dequeue(struct NAME *_list, T *_return) \
        STRUCT_BODY_LIST_POP_HEAD(NAME, T)

    // define: enqueue_elem
    #define DEFINE_STRUCT_LIST_ENQUEUE_ELEM(NAME, T) \
        STRUCT_ATTRIB void NAME##_enqueue_elem(struct NAME *_list, struct NAME##_elem *_elem) \
        STRUCT_BODY_LIST_PUSH_ELEM_HEAD(NAME, T)

    // define: dequeue_elem
    #define DEFINE_STRUCT_LIST_DEQUEUE_ELEM(NAME, T) \
        STRUCT_ATTRIB struct NAME##_elem *NAME##_dequeue_elem(struct NAME *_list) \
        STRUCT_BODY_LIST_POP_ELEM_TAIL(NAME, T)

    #if defined(STRUCT_VA)
        // define: enqueue_va
        #define DEFINE_STRUCT_LIST_ENQUEUE_VA(NAME, T) \
            STRUCT_ATTRIB void NAME##_enqueue_va(struct NAME *_list, T _key, ...) \
            STRUCT_BODY_LIST_PUSH_TAIL_VA(NAME, T)

        // define: dequeue_va
        #define DEFINE_STRUCT_LIST_DEQUEUE_VA(NAME, T) \
            STRUCT_ATTRIB int NAME##_dequeue_va(struct NAME *_list, ...) \
            STRUCT_BODY_LIST_POP_HEAD_VA(NAME, T)

        // define: enqueue_elem_va
        #define DEFINE_STRUCT_LIST_ENQUEUE_ELEM_VA(NAME, T) \
            STRUCT_ATTRIB void NAME##_enqueue_elem_va(struct NAME *_list, ...) \
            STRUCT_BODY_LIST_PUSH_ELEM_TAIL_VA(NAME, T)

        // define: dequeue_elem_va
        #define DEFINE_STRUCT_LIST_DEQUEUE_ELEM_VA(NAME, T) \
            STRUCT_ATTRIB int NAME##_dequeue_elem_va(struct NAME *_list, ...) \
            STRUCT_BODY_LIST_POP_ELEM_HEAD_VA(NAME, T)
    #else
        // define: enqueue_va
        #define DEFINE_STRUCT_LIST_ENQUEUE_VA(NAME, T)
        // define: dequeue_va
        #define DEFINE_STRUCT_LIST_DEQUEUE_VA(NAME, T)
        // define: enqueue_elem_va
        #define DEFINE_STRUCT_LIST_ENQUEUE_ELEM_VA(NAME, T)
        // define: dequeue_elem_va
        #define DEFINE_STRUCT_LIST_DEQUEUE_ELEM_VA(NAME, T)
    #endif
#else
    // define: enqueue
    #define DEFINE_STRUCT_LIST_ENQUEUE(NAME, T)
    // define: dequeue
    #define DEFINE_STRUCT_LIST_DEQUEUE(NAME, T)
    // define: enqueue_elem
    #define DEFINE_STRUCT_LIST_ENQUEUE_ELEM(NAME, T)
    // define: dequeue_elem
    #define DEFINE_STRUCT_LIST_DEQUEUE_ELEM(NAME, T)

    // define: enqueue_va
    #define DEFINE_STRUCT_LIST_ENQUEUE_VA(NAME, T)
    // define: dequeue_va
    #define DEFINE_STRUCT_LIST_DEQUEUE_VA(NAME, T)
    // define: enqueue_elem_va
    #define DEFINE_STRUCT_LIST_ENQUEUE_ELEM_VA(NAME, T)
    // define: dequeue_elem_va
    #define DEFINE_STRUCT_LIST_DEQUEUE_ELEM_VA(NAME, T)

#endif

// define list stack
#if defined(STRUCT_STACK)
   // define: push
   #define DEFINE_STRUCT_LIST_PUSH(NAME, T) \
       STRUCT_ATTRIB void NAME##_push(struct NAME *_list, T _data) \
       STRUCT_BODY_LIST_PUSH_TAIL(NAME, T)

    // define: pop
    #define DEFINE_STRUCT_LIST_POP(NAME, T) \
       STRUCT_ATTRIB  int NAME##_pop(struct NAME *_list, T *_return) \
       STRUCT_BODY_LIST_POP_TAIL(NAME, T)

   // define: push_elem
   #define DEFINE_STRUCT_LIST_PUSH_ELEM(NAME, T) \
        STRUCT_ATTRIB void NAME##_push_elem(struct NAME *_list, struct NAME##_elem *_elem) \
        STRUCT_BODY_LIST_PUSH_ELEM_TAIL(NAME, T)

    // define: pop_elem
    #define DEFINE_STRUCT_LIST_POP_ELEM(NAME, T) \
        STRUCT_ATTRIB struct NAME##_elem *NAME##_pop_elem(struct NAME *_list) \
        STRUCT_BODY_LIST_POP_ELEM_TAIL(NAME, T)

    // cast: list_push
    #define DEFINE_STRUCT_LIST_PUSH_CAST(NAME, SUFFIX, TS, TD) \
        STRUCT_ATTRIB void NAME##_push##SUFFIX(struct NAME *_list, TD _data)\
        { \
            NAME##_push(_list, STRUCT_CAST(TS, _data)); \
        }

    // cast: list_pop
    #define DEFINE_STRUCT_LIST_POP_BACKCAST(NAME, SUFFIX, TS, TD) \
        STRUCT_ATTRIB int NAME##_pop##SUFFIX(struct NAME *_list, TD *_return)\
        { \
            TS _data; \
            if (NAME##_pop(_list, &_data)) { \
                *_return = STRUCT_BACKCAST(TD, _data); \
                return 1; \
            } \
            return 0; \
        }

    #if defined(STRUCT_VA)
        // define: push_va
        #define DEFINE_STRUCT_LIST_PUSH_VA(NAME, T) \
            STRUCT_ATTRIB void NAME##_push_va(struct NAME *_list, T _key, ...) \
           STRUCT_BODY_LIST_PUSH_TAIL_VA(NAME, T)

         // define: pop_va
         #define DEFINE_STRUCT_LIST_POP_VA(NAME, T) \
            STRUCT_ATTRIB int NAME##_pop_va(struct NAME *_list, ...) \
            STRUCT_BODY_LIST_POP_TAIL_VA(NAME, T)

        // define: push_elem_va
        #define DEFINE_STRUCT_LIST_PUSH_ELEM_VA(NAME, T) \
            STRUCT_ATTRIB void NAME##_push_elem_va(struct NAME *_list, ...)  \
            STRUCT_BODY_LIST_PUSH_ELEM_TAIL_VA(NAME, T)

         // define: pop_elem_va
         #define DEFINE_STRUCT_LIST_POP_ELEM_VA(NAME, T) \
            STRUCT_ATTRIB int NAME##_pop_elem_va(struct NAME *_list, ...)  \
            STRUCT_BODY_LIST_POP_ELEM_TAIL_VA(NAME, T)
        
    #else
        // define: push_va
        #define DEFINE_STRUCT_LIST_PUSH_VA(NAME, T)
        // define: pop_va
        #define DEFINE_STRUCT_LIST_POP_VA(NAME, T)
        // define: push_elem_va
        #define DEFINE_STRUCT_LIST_PUSH_ELEM_VA(NAME, T)
        // define: pop_elem_va
        #define DEFINE_STRUCT_LIST_POP_ELEM_VA(NAME, T)
    #endif
#else
    // define: push
    #define DEFINE_STRUCT_LIST_PUSH(NAME, T)
    // define: pop
    #define DEFINE_STRUCT_LIST_POP(NAME, T)
    // define: push_elem
    #define DEFINE_STRUCT_LIST_PUSH_ELEM(NAME, T)
    // define: pop_elem
    #define DEFINE_STRUCT_LIST_POP_ELEM(NAME, T)
 
    // define: push_va
    #define DEFINE_STRUCT_LIST_PUSH_VA(NAME, T)
    // define: pop_va
    #define DEFINE_STRUCT_LIST_POP_VA(NAME, T)
    // define: push_elem_va
    #define DEFINE_STRUCT_LIST_PUSH_ELEM_VA(NAME, T)
    // define: pop_elem_va
    #define DEFINE_STRUCT_LIST_POP_ELEM_VA(NAME, T)
#endif




// define: slice
#define DEFINE_STRUCT_LIST_SLICE(NAME, T) \
    STRUCT_ATTRIB struct NAME *NAME##_slice(struct NAME *_list, int _from, int _to)\
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME *_slice = (struct NAME*) STRUCT_CALLOC(1, struct NAME); \
        _slice->l_head = STRUCT_NULL; \
        _slice->l_tail = STRUCT_NULL; \
        STRUCT_MUTEX_INIT(_slice->l_mutex); \
        STRUCT_MT_SAFE_INIT(_slice->l_mt_safe); \
        struct NAME##_elem *_iter = _list->l_head, *_elem = STRUCT_NULL; \
        int _count = 0; \
        while (_iter && (_count < _to || _to < 0)) {\
            if (_count >= _from) { \
                _elem = (struct NAME##_elem*) STRUCT_CALLOC(1, struct NAME##_elem); \
                _elem->le_next = STRUCT_NULL; \
                _elem->le_next = STRUCT_NULL; \
                STRUCT_ASSIGN(_elem->le_data, _iter->le_data); \
                if (_slice->l_tail) { \
                    _elem->le_prev = _slice->l_tail; \
                    _slice->l_tail->le_next = _elem; \
                    _slice->l_tail = _elem; \
                } else { \
                    _slice->l_head = _elem; \
                    _slice->l_tail = _elem; \
                } \
            } \
            _iter = _iter->le_next; \
            _count += 1; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
        return _slice; \
    }

// define: concat
#define DEFINE_STRUCT_LIST_CONCAT(NAME, T) \
    STRUCT_ATTRIB void NAME##_concat(struct NAME *_list, struct NAME *_src)\
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME##_elem *_iter = _list->l_head, *_elem = STRUCT_NULL; \
        while (_iter) { \
            _elem = (struct NAME##_elem*) STRUCT_CALLOC(1, struct NAME##_elem); \
            _elem->le_next = STRUCT_NULL; \
            _elem->le_next = STRUCT_NULL; \
            STRUCT_ASSIGN(_elem->le_data, _iter->le_data); \
            if (_list->l_tail) { \
                _elem->le_prev = _list->l_tail; \
                _list->l_tail->le_next = _elem; \
                _list->l_tail = _elem; \
            } else { \
                _list->l_head = _elem; \
                _list->l_tail = _elem; \
            } \
            _iter = _iter->le_next; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
    }

// define: reverse
#define DEFINE_STRUCT_LIST_REVERSE(NAME, T) \
    STRUCT_ATTRIB void NAME##_reverse(struct NAME *_list)\
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME##_elem *_iter = _list->l_head, *_next, *_tmp; \
        while (_iter) { \
            _next = _iter->le_next; \
            _tmp = _iter->le_next; \
            _iter->le_next = _iter->le_prev; \
            _iter->le_prev = _tmp; \
            _iter = _next; \
        } \
        _tmp = _list->l_head; \
        _list->l_head = _list->l_tail; \
        _list->l_tail = _tmp; \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
    }

// define: trunacte
#define DEFINE_STRUCT_LIST_TRUNCATE(NAME, T) \
    STRUCT_ATTRIB void NAME##_truncate(struct NAME* _list, int _head, int _tail)\
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME##_elem *_iter, *_next, *_prev; \
        for (_iter = _list->l_head; _head > 0 && _list->l_head; _head -= 1) { \
            _next = _iter->le_next; \
            if (_list->l_tail == _list->l_head) { \
                _list->l_tail = STRUCT_NULL; \
                _list->l_head = STRUCT_NULL; \
            } else { \
                _list->l_head = _list->l_head->le_next; \
                _list->l_head->le_prev = STRUCT_NULL; \
            } \
            STRUCT_FREE(_iter); \
            _iter = _next; \
        } \
        for (_iter = _list->l_tail; _tail > 0 && _list->l_tail; _tail -= 1) { \
            _prev = _iter->le_prev; \
            if (_list->l_tail == _list->l_head) { \
                _list->l_tail = STRUCT_NULL; \
                _list->l_head = STRUCT_NULL; \
            } else { \
                _list->l_tail = _list->l_tail->le_prev; \
                _list->l_tail->le_next = STRUCT_NULL; \
            } \
            STRUCT_FREE(_iter); \
            _iter = _prev; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
    }

// define: cut
#define DEFINE_STRUCT_LIST_CUT(NAME, T) \
    STRUCT_ATTRIB struct NAME* NAME##_cut(struct NAME* _list, int _from, int _to)\
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME *_cut = (struct NAME*) STRUCT_CALLOC(1, struct NAME); \
        _cut->l_head = STRUCT_NULL; \
        _cut->l_tail = STRUCT_NULL; \
        STRUCT_MUTEX_INIT(_cut->l_mutex); \
        STRUCT_MT_SAFE_INIT(_cut->l_mt_safe); \
        struct NAME##_elem *_iter = _list->l_head, *_next; \
        int _count = 0; \
        while (_iter && (_count < _to || _to < 0)) { \
            _next = _iter; \
            if (_count >= _from) { \
                if (_iter->le_prev) { \
                    _iter->le_prev->le_next = _iter->le_next; \
                } else { \
                    _list->l_head = _list->l_head->le_next; \
                    if (_list->l_head) { \
                        _list->l_head->le_prev = STRUCT_NULL; \
                    } else { \
                        _list->l_tail = STRUCT_NULL; \
                    } \
                } \
                if (_iter->le_next) { \
                    _iter->le_next->le_prev = _iter->le_prev; \
                } else { \
                    _list->l_tail = _list->l_tail->le_prev; \
                    if (_list->l_head) { \
                        _list->l_head->le_prev = STRUCT_NULL; \
                    } else { \
                        _list->l_head = STRUCT_NULL; \
                    } \
                } \
                _iter->le_next = STRUCT_NULL; \
                _iter->le_prev = STRUCT_NULL; \
                if (_cut->l_tail) { \
                    _iter->le_prev = _cut->l_tail; \
                    _cut->l_tail->le_next = _iter; \
                    _cut->l_tail = _iter; \
                } else { \
                    _cut->l_tail = _iter; \
                    _cut->l_head = _iter; \
                } \
            } \
            _iter = _next; \
            _count += 1; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
        return _cut; \
    }

// define: for_each
#define DEFINE_STRUCT_LIST_FOR_EACH(NAME, T)  \
    STRUCT_ATTRIB void NAME##_for_each(struct NAME *_list, void (*_func)(T))\
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME##_elem *_iter = _list->l_head, *_next; \
        while (_iter) { \
            _next = _iter->le_next; \
            _func(_iter->le_data); \
            _iter = _next; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
    }

// define: for_each_data
#define DEFINE_STRUCT_LIST_FOR_EACH_DATA(NAME, T, TD)  \
    STRUCT_ATTRIB void NAME##_for_each_data(struct NAME *_list, void (*_func)(T, TD), TD _data)\
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME##_elem *_iter = _list->l_head; \
        while (_iter) { \
            _func(_iter->le_data, _data); \
            _iter = _iter->le_next; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
    }

// define: for_each_index
#define DEFINE_STRUCT_LIST_FOR_EACH_INDEX(NAME, T)  \
    STRUCT_ATTRIB void NAME##_for_each_INDEX(struct NAME *_list, void (*_func)(T, int))\
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME##_elem *_iter = _list->l_head, *_next; \
        int _count = 0; \
        while (_iter) { \
            _next = _iter->le_next; \
            _func(_iter->le_data, _count); \
            _iter = _next; \
            _count += 1; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
    }

// define: for_each_data_index
#define DEFINE_STRUCT_LIST_FOR_EACH_DATA_INDEX(NAME, T, TD)  \
    STRUCT_ATTRIB void NAME##_for_each_data_index(struct NAME *_list, void (*_func)(T, TD, int), TD _data)\
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME##_elem *_iter = _list->l_head, *_next; \
        int _count = 0; \
        while (_iter) { \
            _next = _iter->le_next; \
            _func(_iter->le_data, _data, _count); \
            _iter = _next; \
            _count += 1; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
    }

// define: for_each_index
#define DEFINE_STRUCT_LIST_FOR_EACH_INDEX(NAME, T)  \
    STRUCT_ATTRIB void NAME##_for_each_INDEX(struct NAME *_list, void (*_func)(T, int))\
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME##_elem *_iter = _list->l_head, *_next; \
        int _count = 0; \
        while (_iter) { \
            _next = _iter->le_next; \
            _func(_iter->le_data, _count); \
            _iter = _next; \
            _count += 1; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
    }

// define: for_each_elem
#define DEFINE_STRUCT_LIST_FOR_EACH_ELEM(NAME, T)  \
    STRUCT_ATTRIB void NAME##_for_each_elem(struct NAME *_list, void (*_func)(struct NAME##_elem*))\
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME##_elem *_iter = _list->l_head, *_next; \
        while (_iter) { \
            _next = _iter->le_next; \
            _func(_iter); \
            _iter = _next; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
    }

// define: for_each_elem_data
#define DEFINE_STRUCT_LIST_FOR_EACH_ELEM_DATA(NAME, T, TD)  \
    STRUCT_ATTRIB void NAME##_for_each_elem_data(struct NAME *_list, void (*_func)(struct NAME##_elem*, TD), TD _data)\
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME##_elem *_iter = _list->l_head, *_next; \
        while (_iter) { \
            _next = _iter->le_next; \
            _func(_iter, _data); \
            _iter = _next; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
    }

// define: remove_elem
#define DEFINE_STRUCT_LIST_REMOVE_ELEM(NAME, T) \
    STRUCT_ATTRIB struct NAME##_elem *NAME##_remove_elem(struct NAME *_list, struct NAME##_elem *_elem)\
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        if (_elem == _list->l_head && _list->l_head == _list->l_tail) { \
            _list->l_head = STRUCT_NULL; \
            _list->l_tail = STRUCT_NULL; \
        } else if (_elem == _list->l_head) { \
            _list->l_head = _list->l_head->le_next; \
            _list->l_head->le_prev = STRUCT_NULL; \
            _elem->le_next = STRUCT_NULL; \
        } else if (_elem == _list->l_tail) { \
            _list->l_tail = _list->l_tail->le_prev; \
            _list->l_tail->le_prev = STRUCT_NULL; \
            _elem->le_next = STRUCT_NULL; \
        } else { \
            _elem->le_prev->le_next = _elem->le_next; \
            _elem->le_next->le_prev = _elem->le_prev; \
            _elem->le_next = STRUCT_NULL; \
            _elem->le_prev = STRUCT_NULL; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
        return _elem; \
    }

// define: remove_at
#define DEFINE_STRUCT_LIST_REMOVE_AT(NAME, T) \
    STRUCT_ATTRIB int NAME##_remove_at(struct NAME *_list, T *_data, int _at)\
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        int _count = 0; \
        struct NAME##_elem *_iter = _list->l_head, *_next; \
        while (_iter && _at >= 0) { \
            _next = _iter->le_next; \
            if (_at == 0) { \
                if (_list->l_head == _list->l_tail) { \
                    _list->l_head = STRUCT_NULL; \
                    _list->l_tail = STRUCT_NULL; \
                } else { \
                    if (_iter->le_prev) { \
                        _iter->le_prev->le_next = _iter->le_next; \
                    } else { \
                        _list->l_head = _iter->le_next; \
                        _list->l_head->le_prev = STRUCT_NULL; \
                    } \
                    if (_iter->le_next) { \
                        _iter->le_next->le_prev = _iter->le_prev; \
                    } else { \
                        _list->l_tail = _iter->le_prev; \
                        _list->l_tail->le_next = STRUCT_NULL; \
                    } \
                    if (_data) { \
                        STRUCT_ASSIGN(*_data, _iter->le_data); \
                    } \
                    STRUCT_FREE(_iter); \
                    _count = 1; \
                    break; \
                } \
            } \
            _iter = _next; \
            _at -= 1; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
        return _count; \
    }

// define: remove_elem_at
#define DEFINE_STRUCT_LIST_REMOVE_ELEM_AT(NAME, T) \
    STRUCT_ATTRIB struct NAME##_elem *NAME##_remove_elem_at(struct NAME *_list, int _at)\
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME##_elem *_iter = _list->l_head, *_next; \
        while (_iter && _at >= 0) { \
            _next = _iter->le_next; \
            if (_at == 0) { \
                if (_list->l_head == _list->l_tail) { \
                    _list->l_head = STRUCT_NULL; \
                    _list->l_tail = STRUCT_NULL; \
                } else { \
                    if (_iter->le_prev) { \
                        _iter->le_prev->le_next = _iter->le_next; \
                    } else { \
                        _list->l_head = _iter->le_next; \
                        _list->l_head->le_prev = STRUCT_NULL; \
                    } \
                    if (_iter->le_next) { \
                        _iter->le_next->le_prev = _iter->le_prev; \
                    } else { \
                        _list->l_tail = _iter->le_prev; \
                        _list->l_tail->le_next = STRUCT_NULL; \
                    } \
                    _iter->le_next = STRUCT_NULL; \
                    _iter->le_prev = STRUCT_NULL; \
                    break; \
                } \
            } \
            _iter = _next; \
            _at -= 1; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
        return _iter; \
    }

// define: at
#define DEFINE_STRUCT_LIST_AT(NAME, T) \
    STRUCT_ATTRIB int NAME##_at(struct NAME *_list, T *_data, int _at)\
    { \
        int _count = 0; \
        if (! _data) \
            return _count; \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME##_elem *_iter = _list->l_head; \
        while (_iter && _at >= 0) { \
            if (_at == 0) { \
                STRUCT_ASSIGN(*_data, _iter->le_data); \
                _count = 1; \
                break; \
            } \
            _iter = _iter->le_next; \
            _at -= 1; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
        return _count; \
    } \

// define: elem_at
#define DEFINE_STRUCT_LIST_ELEM_AT(NAME, T) \
    STRUCT_ATTRIB struct NAME##_elem *NAME##_elem_at(struct NAME *_list, int _at)\
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME##_elem *_iter = _list->l_head; \
        while (_iter && _at >= 0) { \
            if (_at == 0) { \
                break; \
            } \
            _iter = _iter->le_next; \
            _at -= 1; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
        return _iter; \
    } \

// define: insert_at
#define DEFINE_STRUCT_LIST_INSERT_AT(NAME, T) \
    STRUCT_ATTRIB void NAME##_insert_at(struct NAME *_list, T _data, int _at)\
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME##_elem *_iter, *_elem; \
        _elem = (struct NAME##_elem*) STRUCT_CALLOC(1, struct NAME##_elem); \
        _elem->le_next = STRUCT_NULL; \
        _elem->le_prev = STRUCT_NULL; \
        STRUCT_ASSIGN(_elem->le_data, _data); \
        if (! _list->l_head) { \
            _list->l_head = _elem; \
            _list->l_tail = _elem; \
        } else if (_at <= 0) { \
            _elem->le_next = _list->l_head; \
            _list->l_head->le_prev = _elem; \
            _list->l_head = _elem; \
        } else { \
            _iter = _list->l_head; \
            while (_iter->le_next && (_at - 1) > 0) { \
                _iter = _iter->le_next; \
                _at -= 1; \
            } \
            if (_iter->le_next) { \
                _iter->le_next->le_prev = _elem; \
            } else { \
                _list->l_tail = _elem; \
            } \
            _elem->le_prev = _iter; \
            _elem->le_next = _iter->le_next; \
            _iter->le_next = _elem; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
    }

// define: insert_elem_at
#define DEFINE_STRUCT_LIST_INSERT_ELEM_AT(NAME, T) \
    STRUCT_ATTRIB void NAME##_insert_elem_at(struct NAME *_list, struct NAME##_elem *_elem, int _at)\
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME##_elem *_iter; \
        _elem->le_next = STRUCT_NULL; \
        _elem->le_prev = STRUCT_NULL; \
        if (! _list->l_head) { \
            _list->l_head = _elem; \
            _list->l_tail = _elem; \
        } else if (_at <= 0) { \
            _elem->le_next = _list->l_head; \
            _list->l_head->le_prev = _elem; \
            _list->l_head = _elem; \
        } else { \
            _iter = _list->l_head; \
            while (_iter->le_next && (_at - 1) > 0) { \
                _iter = _iter->le_next; \
                _at -= 1; \
            } \
            if (_iter->le_next) { \
                _iter->le_next->le_prev = _elem; \
            } else { \
                _list->l_tail = _elem; \
            } \
            _elem->le_prev = _iter; \
            _elem->le_next = _iter->le_next; \
            _iter->le_next = _elem; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
    }

// define: insert_sorted_by
#define DEFINE_STRUCT_LIST_INSERT_SORTED_BY(NAME, T) \
    STRUCT_ATTRIB void NAME##_insert_sorted_by(struct NAME *_list, T _data, int (*_less)(T, T))\
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME##_elem *_iter, *_elem; \
        _elem = (struct NAME##_elem*) STRUCT_CALLOC(1, struct NAME##_elem); \
        _elem->le_next = STRUCT_NULL; \
        _elem->le_prev = STRUCT_NULL; \
        STRUCT_ASSIGN(_elem->le_data, _data); \
        if (! _list->l_head) { \
            _list->l_head = _elem; \
            _list->l_tail = _elem; \
        } else if (_less(_data, _list->l_head->le_data)) { \
            _elem->le_next = _list->l_head; \
            _list->l_head->le_prev = _elem; \
            _list->l_head = _elem; \
        } else { \
            _iter = _list->l_head; \
            while (_iter->le_next && ! _less(_data, _iter->le_next->le_data)) { \
                _iter = _iter->le_next; \
            } \
            if (_iter->le_next) { \
                _iter->le_next->le_prev = _elem; \
            } else { \
                _list->l_tail = _elem; \
            } \
            _elem->le_prev = _iter; \
            _elem->le_next = _iter->le_next; \
            _iter->le_next = _elem; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
    }

// define: insert_sorted_elem_by
#define DEFINE_STRUCT_LIST_INSERT_SORTED_ELEM_BY(NAME, T) \
    STRUCT_ATTRIB void NAME##_insert_sorted_elem_by(struct NAME *_list, struct NAME##_elem *_elem, int (*_less)(T, T))\
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME##_elem *_iter; \
        _elem->le_next = STRUCT_NULL; \
        _elem->le_prev = STRUCT_NULL; \
        if (! _list->l_head) { \
            _list->l_head = _elem; \
            _list->l_tail = _elem; \
        } else if (_less(_elem->le_data, _list->l_head->le_data)) { \
            _elem->le_next = _list->l_head; \
            _list->l_head->le_prev = _elem; \
            _list->l_head = _elem; \
        } else { \
            _iter = _list->l_head; \
            while (_iter->le_next && ! _less(_elem->le_data, _iter->le_next->le_data)) { \
                _iter = _iter->le_next; \
            } \
            if (_iter->le_next) { \
                _iter->le_next->le_prev = _elem; \
            } else { \
                _list->l_tail = _elem; \
            } \
            _elem->le_prev = _iter; \
            _elem->le_next = _iter->le_next; \
            _iter->le_next = _elem; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
    }

// define: insert_elem_after_elem
#define DEFINE_STRUCT_LIST_INSERT_ELEM_AFTER_ELEM(NAME, T) \
    STRUCT_ATTRIB void NAME##_insert_elem_after_elem(struct NAME *_list, struct NAME##_elem *_after, struct NAME##_elem *_elem)\
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        _elem->le_prev = _after; \
        _elem->le_next = _after->le_next; \
        _after->le_next = _elem; \
        if (_after == _list->l_tail) { \
            _list->l_tail = _elem; \
        }  \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
    }


// define: insert_elem_before_elem
#define DEFINE_STRUCT_LIST_INSERT_ELEM_BEFORE_ELEM(NAME, T) \
    STRUCT_ATTRIB void NAME##_insert_elem_before_elem(struct NAME *_list, struct NAME##_elem *_before, struct NAME##_elem *_elem)\
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        _elem->le_next = _before; \
        _elem->le_prev = _before->le_prev; \
        _before->le_prev = _elem; \
        if (_before == _list->l_head) { \
            _list->l_head = _elem; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
    }

// define: sort_by
#define DEFINE_STRUCT_LIST_SORT_BY(NAME, T) \
    STRUCT_ATTRIB void NAME##_sort_by(struct NAME *_list, int (*_less)(T, T))\
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME##_elem *_head = STRUCT_NULL, *_tail = STRUCT_NULL, *_iter = _list->l_head, *_next, *_elem; \
        while (_iter){ \
            _next = _iter->le_next; \
            _elem = _iter; \
            _elem->le_next = STRUCT_NULL; \
            _elem->le_prev  = STRUCT_NULL; \
            if (! _head) { \
                _head = _elem; \
                _tail = _elem; \
            } else if (_less(_elem->le_data, _head->le_data)) { \
                _elem->le_next = _head; \
                _head->le_prev = _elem; \
                _head = _elem; \
            } else { \
                _iter = _head; \
                while (_iter->le_next && ! _less(_elem->le_data, _iter->le_next->le_data)) { \
                    _iter = _iter->le_next; \
                } \
                if (_iter->le_next) { \
                    _iter->le_next->le_prev = _elem; \
                } else { \
                    _tail = _elem; \
                } \
                _elem->le_prev = _iter; \
                _elem->le_next = _iter->le_next; \
                _iter->le_next = _elem; \
            } \
            _iter = _next; \
        } \
        _list->l_head = _head; \
        _list->l_tail = _tail; \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
    }

// define: sort_by_data
#define DEFINE_STRUCT_LIST_SORT_BY_DATA(NAME, T, TD) \
    STRUCT_ATTRIB void NAME##_sort_by_data(struct NAME *_list, int (*_less)(T, T, TD), TD _data)\
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME##_elem *_head = STRUCT_NULL, *_tail = STRUCT_NULL, *_iter = _list->l_head, *_next, *_elem; \
        while (_iter){ \
            _next = _iter->le_next; \
            _elem = _iter; \
            _elem->le_next = STRUCT_NULL; \
            _elem->le_prev  = STRUCT_NULL; \
            if (! _head) { \
                _head = _elem; \
                _tail = _elem; \
            } else if (_less(_elem->le_data, _head->le_data, _data)) { \
                _elem->le_next = _head; \
                _head->le_prev = _elem; \
                _head = _elem; \
            } else { \
                _iter = _head; \
                while (_iter->le_next && ! _less(_elem->le_data, _iter->le_next->le_data, _data)) { \
                    _iter = _iter->le_next; \
                } \
                if (_iter->le_next) { \
                    _iter->le_next->le_prev = _elem; \
                } else { \
                    _tail = _elem; \
                } \
                _elem->le_prev = _iter; \
                _elem->le_next = _iter->le_next; \
                _iter->le_next = _elem; \
            } \
            _iter = _next; \
        } \
        _list->l_head = _head; \
        _list->l_tail = _tail; \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
    }

// define: map
#define DEFINE_STRUCT_LIST_MAP(NAME, T) \
    STRUCT_ATTRIB void NAME##_map(struct NAME *_list, T (*_map)(T))\
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME##_elem *_iter = _list->l_head; \
        while (_iter) { \
            STRUCT_ASSIGN(_iter->le_data, _map(_iter->le_data)); \
            _iter = _iter->le_next; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
    }

// define: map_data
#define DEFINE_STRUCT_LIST_MAP_DATA(NAME, T, TD) \
    STRUCT_ATTRIB void NAME##_map_data(struct NAME *_list, T (*_map)(T, TD), TD _data)\
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME##_elem *_iter = _list->l_head; \
        while (_iter) { \
            STRUCT_ASSIGN(_iter->le_data, _map(_iter->le_data, _data)); \
            _iter = _iter->le_next; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
    }

// define: map_elem
#define DEFINE_STRUCT_LIST_MAP_ELEM(NAME, T) \
    STRUCT_ATTRIB void NAME##_map_elem(struct NAME *_list, void (*_map)(struct NAME##_elem*))\
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME##_elem *_iter = _list->l_head, *_next; \
        while (_iter) { \
            _next = _iter->le_next; \
            _map(_iter); \
            _iter = _next; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
    }

// define: map_elem_data
#define DEFINE_STRUCT_LIST_MAP_ELEM_DATA(NAME, T, TD) \
    STRUCT_ATTRIB void NAME##_map_elem_data(struct NAME *_list, void (*_map)(struct NAME##_elem*, TD), TD _data)\
    { \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME##_elem *_iter = _list->l_head, *_next; \
        while (_iter) { \
            _next = _iter->le_next; \
            _map(_iter, _data); \
            _iter = _next; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
    }

// define: list queue
#define DEFINE_STRUCT_LIST_QUEUE(NAME, T) \
    DEFINE_STRUCT_LIST_ENQUEUE(NAME, T) \
    DEFINE_STRUCT_LIST_DEQUEUE(NAME, T) \
    DEFINE_STRUCT_LIST_ENQUEUE_ELEM(NAME, T) \
    DEFINE_STRUCT_LIST_DEQUEUE_ELEM(NAME, T) \
    DEFINE_STRUCT_LIST_ENQUEUE_VA(NAME, T) \
    DEFINE_STRUCT_LIST_DEQUEUE_VA(NAME, T) \
    DEFINE_STRUCT_LIST_ENQUEUE_ELEM_VA(NAME, T) \
    DEFINE_STRUCT_LIST_DEQUEUE_ELEM_VA(NAME, T)

// define: list stack
#define DEFINE_STRUCT_LIST_STACK(NAME, T) \
    DEFINE_STRUCT_LIST_PUSH(NAME, T) \
    DEFINE_STRUCT_LIST_POP(NAME, T) \
    DEFINE_STRUCT_LIST_PUSH_ELEM(NAME, T) \
    DEFINE_STRUCT_LIST_POP_ELEM(NAME, T) \
    DEFINE_STRUCT_LIST_PUSH_VA(NAME, T) \
    DEFINE_STRUCT_LIST_POP_VA(NAME, T) \
    DEFINE_STRUCT_LIST_PUSH_ELEM_VA(NAME, T) \
    DEFINE_STRUCT_LIST_POP_ELEM_VA(NAME, T)

// define: list double stack
#define DEFINE_STRUCT_LIST_DOUBLE_STACK(NAME, T) \
    DEFINE_STRUCT_LIST_PUSH_HEAD(NAME, T) \
    DEFINE_STRUCT_LIST_PUSH_TAIL(NAME, T) \
    DEFINE_STRUCT_LIST_POP_HEAD(NAME, T) \
    DEFINE_STRUCT_LIST_POP_TAIL(NAME, T) \
    DEFINE_STRUCT_LIST_PUSH_ELEM_HEAD(NAME, T) \
    DEFINE_STRUCT_LIST_PUSH_ELEM_TAIL(NAME, T) \
    DEFINE_STRUCT_LIST_POP_ELEM_HEAD(NAME, T) \
    DEFINE_STRUCT_LIST_POP_ELEM_TAIL(NAME, T) \
    DEFINE_STRUCT_LIST_PUSH_HEAD_VA(NAME, T) \
    DEFINE_STRUCT_LIST_PUSH_TAIL_VA(NAME, T) \
    DEFINE_STRUCT_LIST_POP_HEAD_VA(NAME, T) \
    DEFINE_STRUCT_LIST_POP_TAIL_VA(NAME, T) \
    DEFINE_STRUCT_LIST_PUSH_ELEM_HEAD_VA(NAME, T) \
    DEFINE_STRUCT_LIST_PUSH_ELEM_TAIL_VA(NAME, T) \
    DEFINE_STRUCT_LIST_POP_ELEM_HEAD_VA(NAME, T) \
    DEFINE_STRUCT_LIST_POP_ELEM_TAIL_VA(NAME, T) \

// define: list double queue
#define DEFINE_STRUCT_LIST_DOUBLE_QUEUE(NAME, T) \
    DEFINE_STRUCT_LIST_ENQUEUE_HEAD(NAME, T) \
    DEFINE_STRUCT_LIST_ENQUEUE_TAIL(NAME, T) \
    DEFINE_STRUCT_LIST_DEQUEUE_HEAD(NAME, T) \
    DEFINE_STRUCT_LIST_DEQUEUE_TAIL(NAME, T) \
    DEFINE_STRUCT_LIST_ENQUEUE_ELEM_HEAD(NAME, T) \
    DEFINE_STRUCT_LIST_ENQUEUE_ELEM_TAIL(NAME, T) \
    DEFINE_STRUCT_LIST_DEQUEUE_ELEM_HEAD(NAME, T) \
    DEFINE_STRUCT_LIST_DEQUEUE_ELEM_TAIL(NAME, T) \
    DEFINE_STRUCT_LIST_ENQUEUE_HEAD_VA(NAME, T) \
    DEFINE_STRUCT_LIST_ENQUEUE_TAIL_VA(NAME, T) \
    DEFINE_STRUCT_LIST_DEQUEUE_HEAD_VA(NAME, T) \
    DEFINE_STRUCT_LIST_DEQUEUE_TAIL_VA(NAME, T) \
    DEFINE_STRUCT_LIST_ENQUEUE_ELEM_HEAD_VA(NAME, T) \
    DEFINE_STRUCT_LIST_ENQUEUE_ELEM_TAIL_VA(NAME, T) \
    DEFINE_STRUCT_LIST_DEQUEUE_ELEM_HEAD_VA(NAME, T) \
    DEFINE_STRUCT_LIST_DEQUEUE_ELEM_TAIL_VA(NAME, T) \

// cast: list stack
#define DEFINE_STRUCT_LIST_STACK_CAST(NAME, SUFFIX, TS, TD) \
    DEFINE_STRUCT_LIST_PUSH_CAST(NAME, SUFFIX, TS, TD) \

// backcast: list stack
#define DEFINE_STRUCT_LIST_STACK_BACKCAST(NAME, SUFFIX, TS, TD) \
    DEFINE_STRUCT_LIST_POP_BACKCAST(NAME, SUFFIX, TS, TD)

// define custom: for_each
#define DEFINE_STRUCT_LIST_CUSTOM_FOR_EACH(NAME, T, FNAME, FUNC) \
    STRUCT_ATTRIB void NAME##FNAME(struct NAME *_list)\
    {  \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME##_elem *_iter = _list->l_head, *_next; \
        while (_iter)  { \
            _next = _iter->le_next; \
            FUNC((_iter->le_data)); \
            _iter = _next; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
    }

// define custom: for_each_data
#define DEFINE_STRUCT_LIST_CUSTOM_FOR_EACH_DATA(NAME, T, FNAME, FUNC, TD) \
    STRUCT_ATTRIB void NAME##FNAME(struct NAME *_list, TD _data)\
    {  \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME##_elem *_iter = _list->l_head, *_next; \
        while (_iter)  { \
            _next = _iter->le_next; \
            FUNC((_iter->le_data), (_data)); \
            _iter = _next; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
    }

// define custom: for_each_index
#define DEFINE_STRUCT_LIST_CUSTOM_FOR_EACH_INDEX(NAME, T, FNAME, FUNC) \
    STRUCT_ATTRIB void NAME##FNAME(struct NAME *_list)\
    {  \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME##_elem *_iter = _list->l_head, *_next; \
        int _count = 0; \
        while (_iter)  { \
            _next = _iter->le_next; \
            FUNC((_iter->le_data), (_count)); \
            _iter = _next; \
            _count += 1; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
    }

// define custom: for_each_data_index
#define DEFINE_STRUCT_LIST_CUSTOM_FOR_EACH_DATA_INDEX(NAME, T, FNAME, FUNC, TD) \
    STRUCT_ATTRIB void NAME##FNAME(struct NAME *_list, TD _data)\
    {  \
        STRUCT_MT_SAFE_LOCK(_list->l_mt_safe); \
        struct NAME##_elem *_iter = _list->l_head, *_next; \
        int _count = 0; \
        while (_iter)  { \
            _next = _iter->le_next; \
            FUNC((_iter->le_data), (_data), (_count)); \
            _iter = _next; \
            _count += 1; \
        } \
        STRUCT_MT_SAFE_UNLOCK(_list->l_mt_safe); \
    }

// define: list
#define DEFINE_STRUCT_LIST(NAME, T) \
    DEFINE_STRUCT_LIST_TYPEDEF(NAME, T) \
    DEFINE_STRUCT_LIST_STRUCT(NAME, T) \
    DEFINE_STRUCT_LIST_NEW(NAME, T) \
    DEFINE_STRUCT_LIST_INIT(NAME, T) \
    DEFINE_STRUCT_LIST_DESTROY(NAME, T) \
    DEFINE_STRUCT_LIST_FREE(NAME, T) \
    DEFINE_STRUCT_LIST_HEAD(NAME, T) \
    DEFINE_STRUCT_LIST_TAIL(NAME, T) \
    \
    DEFINE_STRUCT_LIST_LOCK(NAME, T) \
    DEFINE_STRUCT_LIST_UNLOCK(NAME, T) \
    DEFINE_STRUCT_LIST_MT_SAFE_LOCK(NAME, T) \
    DEFINE_STRUCT_LIST_MT_SAFE_UNLOCK(NAME, T) \
    \
    DEFINE_STRUCT_LIST_ELEM_NEW(NAME, T) \
    DEFINE_STRUCT_LIST_ELEM_INIT(NAME, T) \
    DEFINE_STRUCT_LIST_ELEM_DESTROY(NAME, T) \
    DEFINE_STRUCT_LIST_ELEM_FREE(NAME, T) \
    DEFINE_STRUCT_LIST_ELEM_PREV(NAME, T) \
    DEFINE_STRUCT_LIST_ELEM_NEXT(NAME, T) \
    DEFINE_STRUCT_LIST_ELEM_DATA(NAME, T) \
    DEFINE_STRUCT_LIST_ELEM_SET(NAME, T) \
    \
    DEFINE_STRUCT_LIST_EMPTY(NAME, T) \
    DEFINE_STRUCT_LIST_COUNT(NAME, T) \
    DEFINE_STRUCT_LIST_CLONE(NAME, T) \
    DEFINE_STRUCT_LIST_CLEAR(NAME, T) \
    \
    DEFINE_STRUCT_LIST_STACK(NAME, T) \
    DEFINE_STRUCT_LIST_QUEUE(NAME, T) \
    DEFINE_STRUCT_LIST_DOUBLE_STACK(NAME, T) \
    DEFINE_STRUCT_LIST_DOUBLE_QUEUE(NAME, T) \
    \
    DEFINE_STRUCT_LIST_SLICE(NAME, T) \
    DEFINE_STRUCT_LIST_CONCAT(NAME, T) \
    DEFINE_STRUCT_LIST_REVERSE(NAME, T) \
    DEFINE_STRUCT_LIST_TRUNCATE(NAME, T) \
    DEFINE_STRUCT_LIST_CUT(NAME, T) \
    \
    DEFINE_STRUCT_LIST_FOR_EACH(NAME, T) \
    DEFINE_STRUCT_LIST_FOR_EACH_DATA(NAME, T, void*) \
    DEFINE_STRUCT_LIST_FOR_EACH_INDEX(NAME, T) \
    DEFINE_STRUCT_LIST_FOR_EACH_DATA_INDEX(NAME, T, void*) \
    \
    DEFINE_STRUCT_LIST_FOR_EACH_ELEM(NAME, T) \
    DEFINE_STRUCT_LIST_FOR_EACH_ELEM_DATA(NAME, T, void*) \
    \
    DEFINE_STRUCT_LIST_REMOVE_ELEM(NAME, T)\
    DEFINE_STRUCT_LIST_REMOVE_AT(NAME, T) \
    DEFINE_STRUCT_LIST_REMOVE_ELEM_AT(NAME, T) \
    \
    DEFINE_STRUCT_LIST_AT(NAME, T) \
    DEFINE_STRUCT_LIST_ELEM_AT(NAME, T) \
    \
    DEFINE_STRUCT_LIST_INSERT_AT(NAME, T) \
    DEFINE_STRUCT_LIST_INSERT_ELEM_AT(NAME, T) \
    DEFINE_STRUCT_LIST_INSERT_SORTED_BY(NAME, T) \
    DEFINE_STRUCT_LIST_INSERT_SORTED_ELEM_BY(NAME, T) \
    DEFINE_STRUCT_LIST_INSERT_ELEM_AFTER_ELEM(NAME, T) \
    DEFINE_STRUCT_LIST_INSERT_ELEM_BEFORE_ELEM(NAME, T) \
    \
    DEFINE_STRUCT_LIST_SORT_BY(NAME, T) \
    DEFINE_STRUCT_LIST_SORT_BY_DATA(NAME, T, void*) \
    \
    DEFINE_STRUCT_LIST_MAP(NAME, T) \
    DEFINE_STRUCT_LIST_MAP_DATA(NAME, T, void*) \
    DEFINE_STRUCT_LIST_MAP_ELEM(NAME, T) \
    DEFINE_STRUCT_LIST_MAP_ELEM_DATA(NAME, T, void*) \


/*
TODO:
    DEFINE_STRUCT_LIST_FILTER_BY(NAME, T) \
    DEFINE_STRUCT_LIST_FILTER_BY_DATA(NAME, T) \
    DEFINE_STRUCT_LIST_FIND(NAME, T) \
    DEFINE_STRUCT_LIST_FIND_BY(NAME, T) \
    DEFINE_STRUCT_LIST_FIND_BY_DATA(NAME, T) \
    DEFINE_STRUCT_LIST_FIND_ELEM(NAME, T) \
    DEFINE_STRUCT_LIST_FIND_ELEM_BY(NAME, T) \
    DEFINE_STRUCT_LIST_FIND_ELEM_BY_DATA(NAME, T) \
    DEFINE_STRUCT_LIST_DELETE(NAME, T) \
    DEFINE_STRUCT_LIST_DELETE_BY(NAME, T) \
    DEFINE_STRUCT_LIST_DELETE_BY_DATA(NAME, T) \
    DEFINE_STRUCT_LIST_DELETE_ALL(NAME, T) \
    DEFINE_STRUCT_LIST_DELETE_ALL_BY(NAME, T) \
    DEFINE_STRUCT_LIST_DELETE_ALL_BY_DATA(NAME, T)
    DEFINE_STRUCT_LIST_REDUCE(NAME, T)
    DEFINE_STRUCT_LIST_ROTATE(NAME, T)

    SHIFT_LEFT
    SHIFT_RIGHT
    ROTATE_LEFT
    ROTATE_RIGHT
*/
