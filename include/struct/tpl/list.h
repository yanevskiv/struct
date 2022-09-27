#include "common.h"

#define DEFINE_STRUCT_LIST_STRUCT(NAME, T) \
    typedef struct NAME##_elem { \
        struct NAME##_elem *prev, *next; \
        T data; \
    } NAME##_elem_t; \
    typedef struct NAME { \
        struct NAME##_elem *head, *tail; \
    } NAME##_t;

#define DEFINE_STRUCT_LIST_NEW(NAME, T) \
    struct NAME *NAME##_new() \
    { \
        struct NAME *_struct = STRUCT_CALLOC(1, sizeof(struct NAME)); \
        _struct->head = 0; \
        _struct->tail = 0; \
        return _struct; \
    }

#define DEFINE_STRUCT_LIST_FREE(NAME, T)\
    void NAME##_free(struct NAME* _struct) \
    { \
        struct NAME##_elem *_iter = _struct->head, *_next; \
        while (_iter) { \
            _next = _iter->next; \
            STRUCT_FREE(_iter); \
            _iter = _next; \
        } \
        STRUCT_FREE(_struct); \
    }

#define DEFINE_STRUCT_LIST_PUSH(NAME, T) \
    void NAME##_push(struct NAME *_struct, T _data) \
    { \
        struct NAME##_elem *_elem = STRUCT_CALLOC(1, sizeof(struct NAME##_elem)); \
        _elem->next = 0; \
        _elem->prev = 0; \
        _elem->data = STRUCT_CONSTRUCTOR(_data); \
        if (_struct->head) { \
            _elem->prev = _struct->tail; \
            _struct->tail->next = _elem; \
            _struct->tail = _elem; \
        } else { \
            _struct->head = _elem; \
            _struct->tail = _elem; \
        } \
    }

#define DEFINE_STRUCT_LIST_PUSH_VA(NAME, T) \
    void NAME##_push_va(struct NAME *_struct, ...) \
    { \
        va_list _args; \
        va_start(_args, _struct); \
        do { \
            T _arg = va_arg(_args, T); \
            if (STRUCT_CMP(_arg, STRUCT_DEFAULT) == 0) \
                break; \
            NAME##_push(_struct, _arg); \
        } while (STRUCT_TRUE); \
        va_end(_args); \
    }

#define DEFINE_STRUCT_LIST_PUSH_TAIL(NAME, T)\
    void NAME##_push_tail(struct NAME *_struct, T _data) \
    { \
        struct NAME##_elem *_elem = STRUCT_CALLOC(1, sizeof(struct NAME##_elem)); \
        _elem->next = 0; \
        _elem->prev = 0; \
        _elem->data = STRUCT_CONSTRUCTOR(_data); \
        if (_struct->head) { \
            _elem->prev = _struct->tail; \
            _struct->tail->next = _elem; \
            _struct->tail = _elem; \
        } else { \
            _struct->head = _elem; \
            _struct->tail = _elem; \
        } \
    }

#define DEFINE_STRUCT_LIST_PUSH_TAIL_VA(NAME, T) \
    void NAME##_push_tail_va(struct NAME *_struct, T _key, ...) \
    { \
        va_list _args; \
        va_start(_args, _key); \
        do { \
            T _arg = va_arg(_args, T); \
            if (STRUCT_CMP(_arg, _key) == 0) \
                break; \
            NAME##_push_tail(_struct, _arg); \
        } while (STRUCT_TRUE); \
        va_end(_args); \
    }

#define DEFINE_STRUCT_LIST_PUSH_HEAD(NAME, T)\
    void NAME##_push_head(struct NAME *_struct, T _data) \
    { \
        struct NAME##_elem *_elem = STRUCT_CALLOC(1, sizeof(struct NAME##_elem)); \
        _elem->next = 0; \
        _elem->prev = 0; \
        _elem->data = STRUCT_CONSTRUCTOR(_data); \
        if (_struct->tail) { \
            _struct->head->prev = _elem; \
            _elem->next = _struct->head; \
            _struct->head = _elem; \
        } else { \
            _struct->head = _elem; \
            _struct->tail = _elem; \
        } \
    }


#define DEFINE_STRUCT_LIST_PUSH_HEAD_VA(NAME, T) \
    void NAME##_push_head_va(struct NAME *_struct, T _key, ...) \
    { \
        va_list _args; \
        va_start(_args, _key); \
        do { \
            T _arg = va_arg(_args, T); \
            if (STRUCT_CMP(_arg, _key) == 0) \
                break; \
            NAME##_push_head(_struct, _arg); \
        } while (STRUCT_TRUE); \
        va_end(_args); \
    }

#define DEFINE_STRUCT_LIST_POP(NAME, T)
#define DEFINE_STRUCT_LIST_POP_VA(NAME, T)

#define DEFINE_STRUCT_LIST_POP_HEAD(NAME, T) \
    T NAME##_pop_head(struct NAME *_struct) \
    { \
        if (_struct->head) { \
            T _data = _struct->tail->data; \
            struct NAME##_elem *_elem = _struct->head; \
            NAME##_remove_elem(_struct, _struct->head); \
            STRUCT_FREE(_elem); \
            return _data; \
        } \
        return STRUCT_DEFAULT; \
    }

#define DEFINE_STRUCT_LIST_POP_TAIL(NAME, T) \
    T NAME##_pop_tail(struct NAME *_struct) \
    { \
        if (_struct->tail) { \
            T _data = _struct->tail->data; \
            struct NAME##_elem *_elem = _struct->tail; \
            NAME##_remove_elem(_struct, _struct->tail); \
            STRUCT_FREE(_elem); \
            return _data; \
        } \
        return STRUCT_DEFAULT; \
    }

#define DEFINE_STRUCT_LIST_POP_HEAD_VA(NAME, T)\
    void NAME##_pop_head_va(struct NAME *_struct, ...) \
    {\
        va_list _args; \
        va_start(_args, _struct); \
        do { \
            if (_struct->head) { \
                T *_arg = va_arg(_args, T*); \
                if (! _arg) \
                    break; \
                *_arg = NAME##_pop_head(_struct); \
            } else break; \
        } while (STRUCT_TRUE); \
        va_end(_args); \
    }

#define DEFINE_STRUCT_LIST_POP_TAIL_VA(NAME, T)\
    void NAME##_pop_tail_va(struct NAME *_struct, ...) \
    {\
        va_list _args; \
        va_start(_args, _struct); \
        do { \
            if (_struct->tail) { \
                T *_arg = va_arg(_args, T*); \
                if (! _arg) \
                    break; \
                *_arg = NAME##_pop_tail(_struct); \
            } else break; \
        } while (STRUCT_TRUE); \
        va_end(_args); \
    }

#define DEFINE_STRUCT_LIST_HEAD(NAME, T) \
    struct NAME##_elem *NAME##_head(struct NAME *_struct) \
    { \
        return _struct->head; \
    }

#define DEFINE_STRUCT_LIST_TAIL(NAME, T) \
    struct NAME##_elem *NAME##_tail(struct NAME *_struct) \
    { \
        return _struct->tail; \
    }

#define DEFINE_STRUCT_LIST_ELEM_PREV(NAME, T) \
    struct NAME##_elem *NAME##_elem_prev(struct NAME##_elem *_elem) \
    { \
        return _elem->prev; \
    }

#define DEFINE_STRUCT_LIST_ELEM_NEXT(NAME, T) \
    struct NAME##_elem *NAME##_elem_next(struct NAME##_elem *_elem) \
    { \
        return _elem->next; \
    }

#define DEFINE_STRUCT_LIST_ELEM_DATA(NAME, T) \
    T NAME##_elem_data(struct NAME##_elem *_elem) \
    { \
        return _elem->data; \
    }

#define DEFINE_STRUCT_LIST_EMPTY(NAME, T) \
    int NAME##_empty(struct NAME *_struct) \
    { \
        return (_struct->head == 0); \
    }

#define DEFINE_STRUCT_LIST_COUNT(NAME, T) \
    size_t NAME##_count(struct NAME *_struct) \
    { \
        size_t _count = 0; \
        struct NAME##_elem *_iter = _struct->head; \
        while (_iter) { \
            _count += 1; \
            _iter = _iter->next; \
        } \
        return _count; \
    }

#define DEFINE_STRUCT_LIST_FOR_EACH(NAME, T) \
    void NAME##_for_each(struct NAME *_struct, void (*_cb)(T)) \
    { \
        struct NAME##_elem *_iter = _struct->head; \
        while (_iter) { \
            _cb(_iter->data); \
            _iter = _iter->next; \
        } \
    }

#define DEFINE_STRUCT_LIST_FOR_EACH_ELEM(NAME, T) \
    void NAME##_for_each_elem(struct NAME *_struct, void (*_cb)(struct NAME##_elem*)) \
    { \
        struct NAME##_elem *_iter = _struct->head; \
        while (_iter) { \
            _cb(_iter); \
            _iter = _iter->next; \
        } \
    }

#define DEFINE_STRUCT_LIST_AT(NAME, T) \
    T NAME##_at(struct NAME *_struct, size_t _at) \
    { \
        size_t _count = 0; \
        struct NAME##_elem *_iter = _struct->head; \
        while (_iter) { \
            if (_count == _at) \
                return _iter->data; \
            _count += 1; \
            _iter = _iter->next; \
        } \
        return STRUCT_DEFAULT; \
    }

#define DEFINE_STRUCT_LIST_ELEM_AT(NAME, T) \
    struct NAME##_elem NAME##_at(struct NAME *_struct, size_t _at) \
    { \
        size_t _count = 0; \
        struct NAME##_elem *_iter = _struct->head; \
        while (_iter) { \
            if (_count == _at) \
                return _iter; \
            _count += 1; \
            _iter = _iter->next; \
        } \
        return 0; \
    }

#define DEFINE_STRUCT_LIST_REMOVE_ELEM(NAME, T)\
    void NAME##_remove_elem(struct NAME *_struct, struct NAME##_elem *_elem) \
    { \
        if (_struct->head == _struct->tail) { \
            _struct->head = 0; \
            _struct->tail = 0; \
        } else if (_elem == _struct->head) { \
            _struct->head = _struct->head->next; \
            _struct->head->prev = 0; \
        } else if (_elem == _struct->tail) { \
            _struct->tail = _struct->tail->prev; \
            _struct->tail->next = 0; \
        } else { \
            _elem->prev->next = _elem->next; \
            _elem->next->prev = _elem->prev; \
        } \
        _elem->prev = 0; \
        _elem->next = 0; \
    }

#define DEFINE_STRUCT_LIST_DELETE_ELEM(NAME, T) \
    void NAME##_delete_elem(struct NAME *_struct, struct NAME##_elem _elem) \
    { \
        NAME##_remove_elem(_struct, _elem); \
        STRUCT_DESTRUCTOR(_elem->data); \
        STRUCT_FREE(_elem); \
    }

#define DEFINE_STRUCT_LIST_DELETE_AT(NAME, T) \
    void NAME##_delete_at(struct NAME *_struct, size_t _at) \
    { \
        size_t _count = 0; \
        struct NAME##_elem *_iter = _struct->head; \
        while (_iter) { \
            if (_count == _at) { \
                NAME##_delete_elem(_struct, _iter); \
                return; \
            } \
            _count += 1; \
            _iter = _iter->next; \
        } \
    }

#define DEFINE_STRUCT_LIST_DELETE(NAME, T) \
    void NAME##_delete(struct NAME *_struct, T _data) \
    { \
        struct NAME##_elem *_iter = _struct->head, _next; \
        while (_iter) { \
            _next = _iter->next; \
            if (STRUCT_CMP(_iter->data, _data) == 0) { \
                NAME##_delete_elem(_struct, _iter); \
            } \
            _iter = _next; \
        } \
    }

#define DEFINE_STRUCT_LIST_CLONE(NAME, T) \
    struct NAME *NAME##_clone(struct NAME *_struct) \
    { \
        struct NAME *_clone = NAME##_new(); \
        struct NAME##_elem *_iter = _struct->head; \
        while (_iter) { \
            NAME##_push_tail(_clone, _iter->data); \
            _iter = _iter->next; \
        } \
    }

#define DEFINE_STRUCT_LIST_ITERDATA(NAME, FUNCNAME, ITERFUNC) \
    void NAME##_##FUNCNAME(struct NAME *_struct) \
    { \
        struct NAME##_elem *_iter = _struct->head, *_next; \
        while (_iter) { \
            _next = _iter->next; \
            ITERFUNC(_iter->data); \
            _iter = _next; \
        } \
    }

#define DEFINE_STRUCT_LIST(NAME, T) \
    DEFINE_STRUCT_LIST_STRUCT(NAME, T) \
    DEFINE_STRUCT_LIST_NEW(NAME, T) \
    DEFINE_STRUCT_LIST_FREE(NAME, T) \
    DEFINE_STRUCT_LIST_REMOVE_ELEM(NAME, T) \
    \
    DEFINE_STRUCT_LIST_PUSH(NAME, T) \
    DEFINE_STRUCT_LIST_PUSH_VA(NAME, T) \
    DEFINE_STRUCT_LIST_PUSH_HEAD(NAME, T) \
    DEFINE_STRUCT_LIST_PUSH_TAIL(NAME, T) \
    DEFINE_STRUCT_LIST_PUSH_HEAD_VA(NAME, T) \
    DEFINE_STRUCT_LIST_PUSH_TAIL_VA(NAME, T) \
    \
    DEFINE_STRUCT_LIST_POP_HEAD(NAME, T) \
    DEFINE_STRUCT_LIST_POP_TAIL(NAME, T) \
    DEFINE_STRUCT_LIST_POP_HEAD_VA(NAME, T) \
    DEFINE_STRUCT_LIST_POP_TAIL_VA(NAME, T) \
    \
    DEFINE_STRUCT_LIST_HEAD(NAME, T) \
    DEFINE_STRUCT_LIST_TAIL(NAME, T) \
    DEFINE_STRUCT_LIST_AT(NAME, T) \
    DEFINE_STRUCT_LIST_COUNT(NAME, T) \
    \
    DEFINE_STRUCT_LIST_ELEM_PREV(NAME, T) \
    DEFINE_STRUCT_LIST_ELEM_NEXT(NAME, T) \
    DEFINE_STRUCT_LIST_ELEM_DATA(NAME, T) \
    DEFINE_STRUCT_LIST_CLONE(NAME, T) \
    DEFINE_STRUCT_LIST_FOR_EACH(NAME, T) \
    DEFINE_STRUCT_LIST_FOR_EACH_ELEM(NAME, T)

