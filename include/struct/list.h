/**
 * (c) Ivan Janevski
 *
 * Linked List
 */

#include "common.h"

// typedef
#define DEFINE_STRUCT_LIST_TYPEDEF(NAME, T) \
    typedef struct NAME NAME##_t; \
    typedef struct NAME##_elem NAME##_elem_t;

// struct
#define DEFINE_STRUCT_LIST_STRUCT(NAME, T) \
    struct NAME##_elem { \
        struct NAME##_elem *le_prev; \
        struct NAME##_elem *le_next; \
        T le_data; \
    }; \
    struct NAME { \
        struct NAME##_elem *l_head; \
        struct NAME##_elem *l_tail; \
        STRUCT_MUTEX(l_mutex); \
    };

// define list_new
#define DEFINE_STRUCT_LIST_NEW(NAME, T) \
    struct NAME *NAME##_new() { \
        struct NAME *_list = (struct NAME*) STRUCT_CALLOC(1, struct NAME); \
        _list->l_head = STRUCT_NULL; \
        _list->l_tail = STRUCT_NULL; \
        STRUCT_MUTEX_INIT(_list->l_mutex); \
        return _list; \
    }

// define list_init
#define DEFINE_STRUCT_LIST_INIT(NAME, T) \
    struct NAME *NAME##_init(struct NAME *_list) { \
        _list->l_head = STRUCT_NULL; \
        _list->l_tail = STRUCT_NULL; \
        STRUCT_MUTEX_INIT(_list->l_mutex); \
        return _list; \
    }

// define list_destroy
#define DEFINE_STRUCT_LIST_DESTROY(NAME, T) \
    void NAME##_destroy(struct NAME *_list) { \
        struct NAME##_elem *_iter = _list->l_head, *_next; \
        while (_iter) { \
            _next = _iter->le_next; \
            STRUCT_FREE(_iter); \
            _iter = _next; \
        } \
        _list->l_head = STRUCT_NULL; \
        _list->l_tail = STRUCT_NULL; \
    }

// define list_free
#define DEFINE_STRUCT_LIST_FREE(NAME, T) \
    void NAME##_free(struct NAME *_list) { \
        struct NAME##_elem *_iter = _list->l_head, *_next; \
        while (_iter) { \
            _next = _iter->le_next; \
            STRUCT_FREE(_iter); \
            _iter = _next; \
        } \
        STRUCT_FREE(_list); \
    }

// define list_head
#define DEFINE_STRUCT_LIST_HEAD(NAME, T) \
    struct NAME##_elem *NAME##_head(struct NAME *_list) { \
        return _list->l_head; \
    }

// define list_tail
#define DEFINE_STRUCT_LIST_TAIL(NAME, T) \
    struct NAME##_elem *NAME##_tail(struct NAME *_list) { \
        return _list->l_tail; \
    }

// define list_elem_new
#define DEFINE_STRUCT_LIST_ELEM_NEW(NAME, T) \
    struct NAME##_elem *NAME##_elem_new(T _data) { \
        struct NAME##_elem *_elem = (struct NAME##_elem*) STRUCT_CALLOC(1, struct NAME##_elem); \
        _elem->le_next = STRUCT_NULL; \
        _elem->le_prev = STRUCT_NULL; \
        STRUCT_ASSIGN(_elem->le_data, _data); \
        return _elem; \
    }

// define list_elem_init
#define DEFINE_STRUCT_LIST_ELEM_INIT(NAME, T) \
    void NAME##_elem_init(struct NAME##_elem *_elem , T _data) { \
        _elem->le_next = STRUCT_NULL; \
        _elem->le_prev = STRUCT_NULL; \
        STRUCT_ASSIGN(_elem->le_data, _data); \
    }

// define list_elem_destroy
#define DEFINE_STRUCT_LIST_ELEM_DESTROY(NAME, T) \
    void NAME##_elem_destroy(struct NAME##_elem *_elem) { \
        _elem->le_prev = STRUCT_NULL; \
        _elem->le_next = STRUCT_NULL; \
    }

// define list_elem_free
#define DEFINE_STRUCT_LIST_ELEM_FREE(NAME, T) \
    void NAME##_elem_free(struct NAME##_elem *_elem) { \
        _elem->le_prev = STRUCT_NULL; \
        _elem->le_next = STRUCT_NULL; \
        STRUCT_FREE(_elem); \
    }

// define list_elem_prev
#define DEFINE_STRUCT_LIST_ELEM_PREV(NAME, T) \
    struct NAME##_elem *NAME##_elem_prev(struct NAME##_elem *_elem) { \
        return _elem->le_prev; \
    }

// define list_elem_next
#define DEFINE_STRUCT_LIST_ELEM_NEXT(NAME, T) \
    struct NAME##_elem *NAME##_elem_next(struct NAME##_elem *_elem) { \
        return _elem->le_next; \
    }

// define list_elem_data
#define DEFINE_STRUCT_LIST_ELEM_DATA(NAME, T) \
    T NAME##_elem_data(struct NAME##_elem *_elem) { \
        return _elem->le_data; \
    }

// define list_elem_set
#define DEFINE_STRUCT_LIST_ELEM_SET(NAME, T) \
    void NAME##_elem_set(struct NAME##_elem *_elem, T _data) { \
        STRUCT_ASSIGN(_elem->le_data, _data); \
    }

// define double_stack
#if defined(STRUCT_DOUBLE_STACK)
    // define list_push_head
    #define DEFINE_STRUCT_LIST_PUSH_HEAD(NAME, T) \
        void NAME##_push_head(struct NAME *_list, T _data) { \
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
        }

    // define list_push_tail
    #define DEFINE_STRUCT_LIST_PUSH_TAIL(NAME, T) \
        void NAME##_push_tail(struct NAME *_list, T _data) { \
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
        }

    // define list_pop_tail
    #define DEFINE_STRUCT_LIST_POP_TAIL(NAME, T)\
        int NAME##_pop_tail(struct NAME *_list, T *_return) { \
            struct NAME##_elem *_elem = _list->l_tail; \
            if (! _elem) { \
                return STRUCT_FAILURE; \
            } else if (_list->l_tail == _list->l_head) { \
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
            return STRUCT_SUCCESS; \
        }

    // define list_pop_head
    #define DEFINE_STRUCT_LIST_POP_HEAD(NAME, T) \
        int NAME##_pop_head(struct NAME *_list, T *_return) { \
            struct NAME##_elem *_elem = _list->l_head; \
            if (! _elem) { \
                return STRUCT_FAILURE; \
            } else if (_list->l_tail == _list->l_head)  { \
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
            return STRUCT_SUCCESS; \
        }

    // define list_push_elem_head
    #define DEFINE_STRUCT_LIST_PUSH_ELEM_HEAD(NAME, T) \
        void NAME##_push_elem_head(struct NAME *_list, struct NAME##_elem *_elem) { \
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
        }

    // define list_push_elem_tail
    #define DEFINE_STRUCT_LIST_PUSH_ELEM_TAIL(NAME, T) \
        void NAME##_push_elem_tail(struct NAME *_list, struct NAME##_elem *_elem) { \
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
        }

    // define list_pop_elem_tail
    #define DEFINE_STRUCT_LIST_POP_ELEM_TAIL(NAME, T)\
        struct NAME##_elem *NAME##_pop_elem_tail(struct NAME *_list) { \
            struct NAME##_elem *_elem = _list->l_tail; \
            if (! _elem) { \
                return STRUCT_NULL; \
            } \
            if (_list->l_tail == _list->l_head) { \
                _list->l_tail = STRUCT_NULL; \
                _list->l_head = STRUCT_NULL; \
            } else { \
                _list->l_tail = _list->l_tail->le_prev; \
                _list->l_tail->le_next = STRUCT_NULL; \
            } \
            return _elem; \
        }

    // define list_pop_elem_head
    #define DEFINE_STRUCT_LIST_POP_ELEM_HEAD(NAME, T) \
        struct NAME##_elem *NAME##_pop_elem_head(struct NAME *_list) { \
            struct NAME##_elem *_elem = _list->l_head; \
            if (! _elem) { \
                return STRUCT_NULL; \
            } \
            if (_list->l_tail == _list->l_head)  { \
                _list->l_tail = STRUCT_NULL; \
                _list->l_head = STRUCT_NULL; \
            } else { \
                _list->l_head = _list->l_head->le_next; \
                _list->l_head->le_prev = STRUCT_NULL; \
            } \
            return _elem; \
        }

    
    // define double_stack_va
    #if defined(STRUCT_VA)
        // define list_push_head_va
        #define DEFINE_STRUCT_LIST_PUSH_HEAD_VA(NAME, T) \
            void NAME##_push_head_va(struct NAME *_list, T _key, ...) { \
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
            }

        // define list_pop_tail_va
        #define DEFINE_STRUCT_LIST_PUSH_TAIL_VA(NAME, T) \
            void NAME##_push_tail_va(struct NAME *_list, T _key, ...) { \
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
            }

        // define list_pop_head_va
        #define DEFINE_STRUCT_LIST_POP_HEAD_VA(NAME, T) \
            int NAME##_pop_head_va(struct NAME *_list, ...) { \
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
                return _count; \
            }

        // define list_pop_tail_va
        #define DEFINE_STRUCT_LIST_POP_TAIL_VA(NAME, T) \
            int NAME##_pop_tail_va(struct NAME *_list, ...) { \
                int _count = 0; \
                va_list _va; \
                va_start(_va, _list); \
                while (_list->l_head)  { \
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
                return _count; \
            }
    #else
        // define list_push_head_va
        #define DEFINE_STRUCT_LIST_PUSH_HEAD_VA(NAME, T)
        // define list_push_tail_va
        #define DEFINE_STRUCT_LIST_PUSH_TAIL_VA(NAME, T)
        // define list_pop_head_va
        #define DEFINE_STRUCT_LIST_POP_HEAD_VA(NAME, T)
        // define list_pop_tail_va
        #define DEFINE_STRUCT_LIST_POP_TAIL_VA(NAME, T)
    #endif

#else
    // define list_push_head
    #define DEFINE_STRUCT_LIST_PUSH_HEAD(NAME, T)
    // define list_push_tail
    #define DEFINE_STRUCT_LIST_PUSH_TAIL(NAME, T)
    // define list_pop_head
    #define DEFINE_STRUCT_LIST_POP_HEAD(NAME, T)
    // define list_pop_tail
    #define DEFINE_STRUCT_LIST_POP_TAIL(NAME, T)

    // define list_push_elem_head
    #define DEFINE_STRUCT_LIST_PUSH_ELEM_HEAD(NAME, T)
    // define list_push_elem_tail
    #define DEFINE_STRUCT_LIST_PUSH_ELEM_TAIL(NAME, T)
    // define list_pop_elem_head
    #define DEFINE_STRUCT_LIST_POP_ELEM_HEAD(NAME, T)
    // define list_pop_elem_tail
    #define DEFINE_STRUCT_LIST_POP_ELEM_TAIL(NAME, T)

    // define list_push_head_va
    #define DEFINE_STRUCT_LIST_PUSH_HEAD_VA(NAME, T)
    // define list_push_tail_va
    #define DEFINE_STRUCT_LIST_PUSH_TAIL_VA(NAME, T)
    // define list_pop_head_va
    #define DEFINE_STRUCT_LIST_POP_HEAD_VA(NAME, T)
    // define list_pop_tail_va
    #define DEFINE_STRUCT_LIST_POP_TAIL_VA(NAME, T)
#endif


#if defined(STRUCT_QUEUE)
    // define list_enqueue
    #define DEFINE_STRUCT_LIST_ENQUEUE(NAME, T) \
        void NAME##_enqueue(struct NAME *_list, T _data) { \
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
        }

    // define list_dequeue
    #define DEFINE_STRUCT_LIST_DEQUEUE(NAME, T) \
        int NAME##_dequeue(struct NAME *_list, T *_return) { \
            struct NAME##_elem *_elem = _list->l_head; \
            if (! _elem) { \
                return STRUCT_FAILURE; \
            } else if (_list->l_tail == _list->l_head)  { \
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
            return STRUCT_SUCCESS; \
        }

    // define list_enqueue_elem
    #define DEFINE_STRUCT_LIST_ENQUEUE_ELEM(NAME, T) \
        void NAME##_enqueue_elem(struct NAME *_list, struct NAME##_elem *_elem) { \
            if (_list->l_tail) { \
                _elem->le_prev = _list->l_tail; \
                _list->l_tail->le_next = _elem; \
                _list->l_tail = _elem; \
            } else { \
                _list->l_tail = _elem; \
                _list->l_head = _elem; \
            } \
        }

    // define list_dequeue_elem
    #define DEFINE_STRUCT_LIST_DEQUEUE_ELEM(NAME, T) \
        struct NAME##_elem *NAME##_dequeue_elem(struct NAME *_list) { \
            struct NAME##_elem *_elem = _list->l_head; \
            if (! _elem) { \
                return STRUCT_NULL; \
            } else if (_list->l_tail == _list->l_head)  { \
                _list->l_tail = STRUCT_NULL; \
                _list->l_head = STRUCT_NULL; \
            } else { \
                _list->l_head = _list->l_head->le_next; \
                _list->l_head->le_prev = STRUCT_NULL; \
            } \
            return _elem; \
        }
    #if defined(STRUCT_VA)
        // define list_enqueue_va
        #define DEFINE_STRUCT_LIST_ENQUEUE_VA(NAME, T) \
            void NAME##_enqueue_va(struct NAME *_list, T _key, ...) { \
                va_list _va; \
                va_start(_va, _key); \
                T _data = va_arg(_va, T); \
                while (! (STRUCT_EQUALS(_key, _data))) { \
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
            }

        // define list_dequeue_va
        #define DEFINE_STRUCT_LIST_DEQUEUE_VA(NAME, T) \
            int NAME##_dequeue_va(struct NAME *_list, ...) { \
                va_list _va; \
                va_start(_va, _list); \
                int _count = 0; \
                T *_return = va_arg(_va, T*); \
                while (_return)  { \
                    if (_list->l_head) { \
                        struct NAME##_elem *_elem = _list->l_head; \
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
                        _count += 1; \
                    } \
                    _return = va_arg(_va, T*); \
                } \
                va_end(_va); \
                return _count; \
            }

        // define list_enqueue_elem_va
        #define DEFINE_STRUCT_LIST_ENQUEUE_ELEM_VA(NAME, T) \
            void NAME##_enqueue_elem_va(struct NAME *_list, ...) { \
                va_list _va; \
                va_start(_va, _list); \
                struct NAME##_elem **_return = va_arg(_va, struct NAME##_elem**), *_elem; \
                while (_return) { \
                    _elem = _list->l_head; \
                    if (_list->l_tail) { \
                        _elem->le_prev = _list->l_tail; \
                        _list->l_tail->le_next = _elem; \
                        _list->l_tail = _elem; \
                    } else { \
                        _list->l_tail = _elem; \
                        _list->l_head = _elem; \
                    } \
                    _return = va_arg(_va, struct NAME##_elem**); \
                } \
                va_end(_va); \
            }

        // define list_dequeue_elem_va
        #define DEFINE_STRUCT_LIST_DEQUEUE_ELEM_VA(NAME, T) \
            int NAME##_dequeue_elem_va(struct NAME *_list, ...) { \
                va_list _va; \
                va_start(_va, _list); \
                struct NAME##_elem *_elem, **_return; \
                int _count = 0; \
                _return = va_arg(_va, struct NAME##_elem**); \
                while (_return) { \
                    if (_list->l_head) { \
                        _elem = _list->l_head; \
                        if (_list->l_tail == _list->l_head)  { \
                            _list->l_tail = STRUCT_NULL; \
                            _list->l_head = STRUCT_NULL; \
                        } else { \
                            _list->l_head = _list->l_head->le_next; \
                            _list->l_head->le_prev = STRUCT_NULL; \
                        } \
                        *_return = _elem; \
                        _count += 1; \
                    } else { \
                        *_return = STRUCT_NULL; \
                    } \
                    _return = va_arg(_va, struct NAME##_elem**); \
                } \
                return _count; \
            }

    #endif
#else
    // define list_enqueue
    #define DEFINE_STRUCT_LIST_ENQUEUE(NAME, T)
    // define list_dequeue
    #define DEFINE_STRUCT_LIST_DEQUEUE(NAME, T)
    // define list_enqueue_elem
    #define DEFINE_STRUCT_LIST_ENQUEUE_ELEM(NAME, T)
    // define list_dequeue_elem
    #define DEFINE_STRUCT_LIST_DEQUEUE_ELEM(NAME, T)

    // define list_enqueue_va
    #define DEFINE_STRUCT_LIST_ENQUEUE_VA(NAME, T)
    // define list_dequeue_va
    #define DEFINE_STRUCT_LIST_DEQUEUE_VA(NAME, T)
    // define list_enqueue_elem_va
    #define DEFINE_STRUCT_LIST_ENQUEUE_ELEM_VA(NAME, T)
    // define list_dequeue_elem_va
    #define DEFINE_STRUCT_LIST_DEQUEUE_ELEM_VA(NAME, T)

#endif

#if defined(STRUCT_STACK)
   // define list_push
   #define DEFINE_STRUCT_LIST_PUSH(NAME, T) \
       void NAME##_push(struct NAME *_list, T _data) { \
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
        }

    // define list_pop
    #define DEFINE_STRUCT_LIST_POP(NAME, T) \
        int NAME##_pop(struct NAME *_list, T *_return) { \
            struct NAME##_elem *_elem = _list->l_head; \
            if (! _elem) { \
                return STRUCT_FAILURE; \
            } else if (_list->l_tail == _list->l_head)  { \
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
            return STRUCT_SUCCESS; \
        }

   // define list_push_elem
   #define DEFINE_STRUCT_LIST_PUSH_ELEM(NAME, T) \
       void NAME##_push_elem(struct NAME *_list, struct NAME##_elem *_elem) { \
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
        }

    // define list_pop_elem
    #define DEFINE_STRUCT_LIST_POP_ELEM(NAME, T) \
        struct NAME##_elem *NAME##_pop_elem(struct NAME *_list) { \
            struct NAME##_elem *_elem = _list->l_head; \
            if (! _elem) { \
                return STRUCT_NULL; \
            } else if (_list->l_tail == _list->l_head)  { \
                _list->l_tail = STRUCT_NULL; \
                _list->l_head = STRUCT_NULL; \
            } else { \
                _list->l_head = _list->l_head->le_next; \
                _list->l_head->le_prev = STRUCT_NULL; \
            } \
            return _elem; \
        }

    #if defined(STRUCT_VA)
        // define list_push_va
        #define DEFINE_STRUCT_LIST_PUSH_VA(NAME, T) \
            void NAME##_push_va(struct NAME *_list, T _key, ...) { \
                 va_list _va; \
                 va_start(_va, _key); \
                 T _data = va_arg(_va, T); \
                 while (! (STRUCT_EQUALS(_key, _data))) { \
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
             }

         // define list_pop_va
         #define DEFINE_STRUCT_LIST_POP_VA(NAME, T) \
             int NAME##_pop_va(struct NAME *_list, ...) { \
                 va_list _va; \
                 va_start(_va, _list); \
                 T *_return = va_arg(_va, T*); \
                 int _count = 0; \
                 while (_return) { \
                     struct NAME##_elem *_elem = _list->l_head; \
                     if (! _elem) { \
                         return STRUCT_FAILURE; \
                     } else if (_list->l_tail == _list->l_head)  { \
                         _list->l_tail = STRUCT_NULL; \
                         _list->l_head = STRUCT_NULL; \
                     } else { \
                         _list->l_head = _list->l_head->le_next; \
                         _list->l_head->le_prev = STRUCT_NULL; \
                     } \
                     STRUCT_ASSIGN(*_return, _elem->le_data); \
                     STRUCT_FREE(_elem); \
                     _return = va_arg(_va, T*); \
                     _count += 1; \
                 } \
                 va_end(_va); \
                 return _count; \
             }

        // define list_push_elem_va
        #define DEFINE_STRUCT_LIST_PUSH_ELEM_VA(NAME, T) \
            void NAME##_push_elem_va(struct NAME *_list, ...) { \
                 va_list _va; \
                 va_start(_va, _list); \
                 struct NAME##_elem *_elem = va_arg(_va, struct NAME##_elem*); \
                 while (_elem) { \
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
                     _elem = va_arg(_va, struct NAME##_elem*); \
                 } \
                 va_end(_va); \
             }

         // define list_pop_elem_va
         #define DEFINE_STRUCT_LIST_POP_ELEM_VA(NAME, T) \
             int NAME##_pop_elem_va(struct NAME *_list, ...) { \
                 va_list _va; \
                 va_start(_va, _list); \
                 int _count = 0; \
                 struct NAME##_elem *_elem, **_return = va_arg(_va, struct NAME##_elem**); \
                 while (_return) { \
                     if (_list->l_head) { \
                         _elem = _list->l_head; \
                         if (! _elem) { \
                             return _count; \
                         } else if (_list->l_tail == _list->l_head)  { \
                             _list->l_tail = STRUCT_NULL; \
                             _list->l_head = STRUCT_NULL; \
                         } else { \
                             _list->l_head = _list->l_head->le_next; \
                             _list->l_head->le_prev = STRUCT_NULL; \
                         } \
                         *_return = _elem; \
                         _count += 1; \
                     } else { \
                        *_return = STRUCT_NULL; \
                     } \
                     _return = va_arg(_va, struct NAME##_elem**); \
                 } \
                 return _count; \
             }
        
    #else
        // define list_push_va
        #define DEFINE_STRUCT_LIST_PUSH_VA(NAME, T)
        // define list_pop_va
        #define DEFINE_STRUCT_LIST_POP_VA(NAME, T)
        // define list_push_elem_va
        #define DEFINE_STRUCT_LIST_PUSH_ELEM_VA(NAME, T)
        // define list_pop_elem_va
        #define DEFINE_STRUCT_LIST_POP_ELEM_VA(NAME, T)
    #endif
#else
    // define list_push
    #define DEFINE_STRUCT_LIST_PUSH(NAME, T)
    // define list_pop
    #define DEFINE_STRUCT_LIST_POP(NAME, T)
    // define list_push_elem
    #define DEFINE_STRUCT_LIST_PUSH_ELEM(NAME, T)
    // define list_pop_elem
    #define DEFINE_STRUCT_LIST_POP_ELEM(NAME, T)
 
    // define list_push_va
    #define DEFINE_STRUCT_LIST_PUSH_VA(NAME, T)
    // define list_pop_va
    #define DEFINE_STRUCT_LIST_POP_VA(NAME, T)
    // define list_push_elem_va
    #define DEFINE_STRUCT_LIST_PUSH_ELEM_VA(NAME, T)
    // define list_pop_elem_va
    #define DEFINE_STRUCT_LIST_POP_ELEM_VA(NAME, T)
#endif



// define list_empty
#define DEFINE_STRUCT_LIST_EMPTY(NAME, T) \
    int NAME##_empty(struct NAME *_list) { \
        return _list->l_head == STRUCT_NULL; \
    }

// define list_count
#define DEFINE_STRUCT_LIST_COUNT(NAME, T) \
    int NAME##_count(struct NAME *_list) { \
        struct NAME##_elem *_iter = _list->l_head; \
        int _count = 0; \
        while (_iter) { \
            _count += 1; \
            _iter = _iter->le_next; \
        } \
        return _count; \
    }

// define list_clone
#define DEFINE_STRUCT_LIST_CLONE(NAME, T)  \
    struct NAME *NAME##_clone(struct NAME *_list) { \
        struct NAME *_clone = (struct NAME*) STRUCT_CALLOC(1, struct NAME); \
        _clone->l_head = STRUCT_NULL; \
        _clone->l_tail = STRUCT_NULL; \
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
        return _clone; \
    }

// define list_clear
#define DEFINE_STRUCT_LIST_CLEAR(NAME, T) \
    void NAME##_clear(struct NAME *_list) { \
        struct NAME##_elem *_iter = _list->l_head, *_next = STRUCT_NULL; \
        while (_iter) { \
            _next = _iter->le_next; \
            STRUCT_FREE(_iter); \
            _iter = _next; \
        } \
        _list->l_head = STRUCT_NULL; \
        _list->l_tail = STRUCT_NULL; \
    }

// define list_slice
#define DEFINE_STRUCT_LIST_SLICE(NAME, T) \
    struct NAME *NAME##_slice(struct NAME *_list, int _from, int _to) { \
        struct NAME *_slice = (struct NAME*) STRUCT_CALLOC(1, struct NAME); \
        _slice->l_head = STRUCT_NULL; \
        _slice->l_tail = STRUCT_NULL; \
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
        return _slice; \
    }

// define list_concat
#define DEFINE_STRUCT_LIST_CONCAT(NAME, T) \
    void NAME##_concat(struct NAME *_dest, struct NAME *_src) { \
        struct NAME##_elem *_iter = _dest->l_head, *_elem = STRUCT_NULL; \
        while (_iter) { \
            _elem = (struct NAME##_elem*) STRUCT_CALLOC(1, struct NAME##_elem); \
            _elem->le_next = STRUCT_NULL; \
            _elem->le_next = STRUCT_NULL; \
            STRUCT_ASSIGN(_elem->le_data, _iter->le_data); \
            if (_dest->l_tail) { \
                _elem->le_prev = _dest->l_tail; \
                _dest->l_tail->le_next = _elem; \
                _dest->l_tail = _elem; \
            } else { \
                _dest->l_head = _elem; \
                _dest->l_tail = _elem; \
            } \
            _iter = _iter->le_next; \
        } \
    }

// define list_reverse
#define DEFINE_STRUCT_LIST_REVERSE(NAME, T) \
    void NAME##_reverse(struct NAME *_list) { \
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
    }

// define list_trunacte
#define DEFINE_STRUCT_LIST_TRUNCATE(NAME, T) \
    void NAME##_truncate(struct NAME* _list, int _head, int _tail) { \
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
    }

// define list_cut
#define DEFINE_STRUCT_LIST_CUT(NAME, T) \
    struct NAME* NAME##_cut(struct NAME* _list, int _from, int _to) { \
        struct NAME *_cut = (struct NAME*) STRUCT_CALLOC(1, struct NAME); \
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
        return _cut; \
    }

// define list_for_each
#define DEFINE_STRUCT_LIST_FOR_EACH(NAME, T)  \
    void NAME##_for_each(struct NAME *_list, void (*_func)(T)) { \
        struct NAME##_elem *_iter = _list->l_head; \
        while (_iter) { \
            _func(_iter->le_data); \
            _iter = _iter->le_next; \
        } \
    }

// define list_for_each_elem
#define DEFINE_STRUCT_LIST_FOR_EACH_ELEM(NAME, T)  \
    void NAME##_for_each_elem(struct NAME *_list, void (*_func)(struct NAME##_elem*)) { \
        struct NAME##_elem *_iter = _list->l_head, *_next; \
        while (_iter) { \
            _next = _iter->le_next; \
            _func(_iter); \
            _iter = _next; \
        } \
    }

// define list_for_each_data
#define DEFINE_STRUCT_LIST_FOR_EACH_DATA(NAME, T, TD)  \
    void NAME##_for_each_data(struct NAME *_list, void (*_func)(T, TD), TD _data) { \
        struct NAME##_elem *_iter = _list->l_head; \
        while (_iter) { \
            _func(_iter->le_data, _data); \
            _iter = _iter->le_next; \
        } \
    }

// define list_for_each_elem_data
#define DEFINE_STRUCT_LIST_FOR_EACH_ELEM_DATA(NAME, T, TD)  \
    void NAME##_for_each_elem_data(struct NAME *_list, void (*_func)(struct NAME##_elem*, TD), TD _data) { \
        struct NAME##_elem *_iter = _list->l_head, *_next; \
        while (_iter) { \
            _next = _iter->le_next; \
            _func(_iter, _data); \
            _iter = _next; \
        } \
    }

// define list_remove_elem
#define DEFINE_STRUCT_LIST_REMOVE_ELEM(NAME, T) \
    struct NAME##_elem *NAME##_remove_elem(struct NAME *_list, struct NAME##_elem *_elem) { \
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
        return _elem; \
    }

// define list_remove_at
#define DEFINE_STRUCT_LIST_REMOVE_AT(NAME, T) \
    int NAME##_remove_at(struct NAME *_list, T *_data, int _at) { \
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
                    return STRUCT_SUCCESS; \
                } \
            } \
            _iter = _next; \
            _at -= 1; \
        } \
        return STRUCT_FAILURE; \
    }

// define list_remove_elem_at
#define DEFINE_STRUCT_LIST_REMOVE_ELEM_AT(NAME, T) \
    struct NAME##_elem *NAME##_remove_elem_at(struct NAME *_list, int _at) { \
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
                    return _iter;\
                } \
            } \
            _iter = _next; \
            _at -= 1; \
        } \
        return STRUCT_NULL; \
    }

// define list_insert_at
#define DEFINE_STRUCT_LIST_INSERT_AT(NAME, T) \
    void NAME##_insert_at(struct NAME *_list, T _data, int _at) { \
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
    }

// define list_insert_elem_at
#define DEFINE_STRUCT_LIST_INSERT_ELEM_AT(NAME, T) \
    void NAME##_insert_elem_at(struct NAME *_list, struct NAME##_elem *_elem, int _at) { \
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
    }

// define list_insert_sorted_by
#define DEFINE_STRUCT_LIST_INSERT_SORTED_BY(NAME, T) \
    void NAME##_insert_sorted_by(struct NAME *_list, T _data, int (*_less)(T, T)) { \
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
    }

// define list_insert_sorted_elem_by
#define DEFINE_STRUCT_LIST_INSERT_SORTED_ELEM_BY(NAME, T) \
    void NAME##_insert_sorted_elem_by(struct NAME *_list, struct NAME##_elem *_elem, int (*_less)(T, T)) { \
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
    }

// define list_insert_elem_after_elem
#define DEFINE_STRUCT_LIST_INSERT_ELEM_AFTER_ELEM(NAME, T) \
    void NAME##_insert_elem_after_elem(struct NAME *_list, struct NAME##_elem *_after, struct NAME##_elem *_elem) { \
        _elem->le_prev = _after; \
        _elem->le_next = _after->le_next; \
        _after->le_next = _elem; \
        if (_after == _list->l_tail) { \
            _list->l_tail = _elem; \
        }  \
    }


// define list_insert_elem_before_elem
#define DEFINE_STRUCT_LIST_INSERT_ELEM_BEFORE_ELEM(NAME, T) \
    void NAME##_insert_elem_before_elem(struct NAME *_list, struct NAME##_elem *_before, struct NAME##_elem *_elem) { \
        _elem->le_next = _before; \
        _elem->le_prev = _before->le_prev; \
        _before->le_prev = _elem; \
        if (_before == _list->l_head) { \
            _list->l_head = _elem; \
        } \
    }

// define list_sort_by
#define DEFINE_STRUCT_LIST_SORT_BY(NAME, T) \
    void NAME##_sort_by(struct NAME *_list, int (*_less)(T, T)) { \
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
    }

// define list_sort_by_data
#define DEFINE_STRUCT_LIST_SORT_BY_DATA(NAME, T, TD) \
    void NAME##_sort_by_data(struct NAME *_list, int (*_less)(T, T, TD), TD _data) { \
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
    }

// define list_map
#define DEFINE_STRUCT_LIST_MAP(NAME, T) \
    void NAME##_map(struct NAME *_list, T (*_map)(T)) { \
        struct NAME##_elem *_iter = _list->l_head; \
        while (_iter) { \
            STRUCT_ASSIGN(_iter->le_data, _map(_iter->le_data)); \
            _iter = _iter->le_next; \
        } \
    }

// define list_map_data
#define DEFINE_STRUCT_LIST_MAP_DATA(NAME, T, TD) \
    void NAME##_map_data(struct NAME *_list, T (*_map)(T, TD), TD _data) { \
        struct NAME##_elem *_iter = _list->l_head; \
        while (_iter) { \
            STRUCT_ASSIGN(_iter->le_data, _map(_iter->le_data, _data)); \
            _iter = _iter->le_next; \
        } \
    }

// define list_map_elem
#define DEFINE_STRUCT_LIST_MAP_ELEM(NAME, T) \
    void NAME##_map_elem(struct NAME *_list, void (*_map)(struct NAME##_elem*)) { \
        struct NAME##_elem *_iter = _list->l_head, *_next; \
        while (_iter) { \
            _next = _iter->le_next; \
            _map(_iter); \
            _iter = _next; \
        } \
    }

// define list_map_elem_data
#define DEFINE_STRUCT_LIST_MAP_ELEM_DATA(NAME, T, TD) \
    void NAME##_map_elem_data(struct NAME *_list, void (*_map)(struct NAME##_elem*, TD), TD _data) { \
        struct NAME##_elem *_iter = _list->l_head, *_next; \
        while (_iter) { \
            _next = _iter->le_next; \
            _map(_iter, _data); \
            _iter = _next; \
        } \
    }

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
    DEFINE_STRUCT_LIST_ELEM_NEW(NAME, T) \
    DEFINE_STRUCT_LIST_ELEM_INIT(NAME, T) \
    DEFINE_STRUCT_LIST_ELEM_DESTROY(NAME, T) \
    DEFINE_STRUCT_LIST_ELEM_FREE(NAME, T) \
    DEFINE_STRUCT_LIST_ELEM_PREV(NAME, T) \
    DEFINE_STRUCT_LIST_ELEM_NEXT(NAME, T) \
    DEFINE_STRUCT_LIST_ELEM_DATA(NAME, T) \
    DEFINE_STRUCT_LIST_ELEM_SET(NAME, T) \
    \
    DEFINE_STRUCT_LIST_PUSH(NAME, T) \
    DEFINE_STRUCT_LIST_POP(NAME, T) \
    DEFINE_STRUCT_LIST_PUSH_ELEM(NAME, T) \
    DEFINE_STRUCT_LIST_POP_ELEM(NAME, T) \
    DEFINE_STRUCT_LIST_PUSH_VA(NAME, T) \
    DEFINE_STRUCT_LIST_POP_VA(NAME, T) \
    DEFINE_STRUCT_LIST_PUSH_ELEM_VA(NAME, T) \
    DEFINE_STRUCT_LIST_POP_ELEM_VA(NAME, T) \
    \
    DEFINE_STRUCT_LIST_ENQUEUE(NAME, T) \
    DEFINE_STRUCT_LIST_DEQUEUE(NAME, T) \
    DEFINE_STRUCT_LIST_ENQUEUE_ELEM(NAME, T) \
    DEFINE_STRUCT_LIST_DEQUEUE_ELEM(NAME, T) \
    DEFINE_STRUCT_LIST_ENQUEUE_VA(NAME, T) \
    DEFINE_STRUCT_LIST_DEQUEUE_VA(NAME, T) \
    DEFINE_STRUCT_LIST_ENQUEUE_ELEM_VA(NAME, T) \
    DEFINE_STRUCT_LIST_DEQUEUE_ELEM_VA(NAME, T) \
    \
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
    \
    DEFINE_STRUCT_LIST_EMPTY(NAME, T) \
    DEFINE_STRUCT_LIST_COUNT(NAME, T) \
    \
    DEFINE_STRUCT_LIST_CLONE(NAME, T) \
    DEFINE_STRUCT_LIST_CLEAR(NAME, T) \
    DEFINE_STRUCT_LIST_SLICE(NAME, T) \
    DEFINE_STRUCT_LIST_CONCAT(NAME, T) \
    DEFINE_STRUCT_LIST_REVERSE(NAME, T) \
    DEFINE_STRUCT_LIST_TRUNCATE(NAME, T) \
    DEFINE_STRUCT_LIST_CUT(NAME, T) \
    \
    DEFINE_STRUCT_LIST_FOR_EACH(NAME, T) \
    DEFINE_STRUCT_LIST_FOR_EACH_DATA(NAME, T, void*) \
    DEFINE_STRUCT_LIST_FOR_EACH_ELEM(NAME, T) \
    DEFINE_STRUCT_LIST_FOR_EACH_ELEM_DATA(NAME, T, void*) \
    \
    DEFINE_STRUCT_LIST_REMOVE_ELEM(NAME, T)\
    DEFINE_STRUCT_LIST_REMOVE_AT(NAME, T) \
    DEFINE_STRUCT_LIST_REMOVE_ELEM_AT(NAME, T) \
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
*/
