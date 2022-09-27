#include "defs.h"
#define DEFINE_STRUCT_TREE_STRUCT(NAME, T) \
    struct NAME { \
        T data; \
        struct NAME *parent; \
        struct NAME *prev; \
        struct NAME *next; \
        struct NAME *first; \
        struct NAME *last; \
    };
#define DEFINE_STRUCT_TREE_TYPEDEF_STRUCT(NAME, T) \
    typedef struct NAME { \
        T data; \
        struct NAME *parent; \
        struct NAME *prev; \
        struct NAME *next; \
        struct NAME *first; \
        struct NAME *last; \
    } NAME##_t;
#define DEFINE_STRUCT_TREE_NEW(NAME, T) \
    struct NAME *NAME##_new(T _data) \
    { \
        struct NAME *_node = STRUCT_CALLOC(1, sizeof(struct NAME)); \
        _node->data = _data; \
        _node->parent = 0; \
        _node->prev = 0; \
        _node->next = 0 ; \
        _node->first = 0; \
        _node->last = 0; \
        return _node; \
    }
#define DEFINE_STRUCT_TREE_FREE(NAME, T) \
    void NAME##_free(struct NAME *_node) \
    { \
        STRUCT_FREE(_node); \
    }
#define DEFINE_STRUCT_TREE_DATA(NAME, T) \
    struct NAME *NAME##_data(struct NAME *_node) \
    { \
        return _node->data; \
    }
#define DEFINE_STRUCT_TREE_PARENT(NAME, T) \
    struct NAME *NAME##_parent(struct NAME *_node) \
    { \
        return _node->parent; \
    }
#define DEFINE_STRUCT_TREE_PREV(NAME, T) \
    struct NAME *NAME##_prev(struct NAME *_node) \
    { \
        return _node->prev; \
    }
#define DEFINE_STRUCT_TREE_NEXT(NAME, T) \
    struct NAME *NAME##_next(struct NAME *_node) \
    { \
        return _node->next; \
    }
#define DEFINE_STRUCT_TREE_FIRST(NAME, T) \
    struct NAME *NAME##_first(struct NAME *_node) \
    { \
        return _node->first; \
    }
#define DEFINE_STRUCT_TREE_LAST(NAME, T) \
    struct NAME *NAME##_last(struct NAME *_node) \
    { \
        return _node->last; \
    }
#define STRUCT_TREE_REMOVE(NODE) \
    do { \
        if ((NODE)->prev) { \
            (NODE)->prev->next = (NODE)->next; \
        } else if ((NODE)->parent) { \
            (NODE)->parent->first = (NODE)->next; \
            if ((NODE)->parent->first == 0) \
                (NODE)->parent->last = 0; \
        } \
        if ((NODE)->next) { \
            (NODE)->next->prev = (NODE)->prev; \
        } else if ((NODE)->parent) { \
            (NODE)->parent->last = (NODE)->prev; \
            if ((NODE)->parent->last == 0) \
                (NODE)->parent->first = 0; \
        } \
        (NODE)->prev = 0; \
        (NODE)->next = 0; \
        (NODE)->parent = 0; \
    } while (0)

#define STRUCT_TREE_ATTACH_AFTER(NODE, ATTACH) \
    do { \
        if (! (NODE)->next && (NODE)->parent) \
            (NODE)->parent->last = (ATTACH); \
        (ATTACH)->next = (NODE)->next; \
        (ATTACH)->prev = (NODE); \
        (ATTACH)->parent = (NODE)->parent; \
        (NODE)->next = (ATTACH); \
    } while (0)
#define STRUCT_TREE_ATTACH_BEFORE(NODE, ATTACH) \
    do { \
        if (! (NODE)->prev && (NODE)->parent) \
            (NODE)->parent->first = (ATTACH); \
        (ATTACH)->prev = (NODE)->prev; \
        (ATTACH)->next = (NODE); \
        (ATTACH)->parent = (NODE)->parent; \
        (NODE)->prev = (ATTACH); \
    } while (0)
#define STRUCT_TREE_ATTACH_LAST(NODE, ATTACH) \
    do { \
        (ATTACH)->parent = (NODE); \
        if ((NODE)->last) { \
            (ATTACH)->prev = (NODE)->last; \
            (ATTACH)->next = 0; \
            (NODE)->last->next = (ATTACH); \
        } else { \
            (ATTACH)->prev = 0; \
            (ATTACH)->next = 0; \
            (NODE)->first = (ATTACH); \
            (NODE)->last = (ATTACH); \
        } \
    } while (0)
#define STRUCT_TREE_ATTACH_FIRST(NODE, ATTACH) \
    do { \
        (ATTACH)->parent = (NODE); \
        if ((NODE)->first) { \
            (ATTACH)->prev = 0; \
            (ATTACH)->next = (NODE)->first; \
            (NODE)->first->prev = (ATTACH); \
        } else { \
            (ATTACH)->prev = 0; \
            (ATTACH)->next = 0; \
            (NODE)->first = (ATTACH); \
            (NODE)->last = (ATTACH); \
        } \
    } while (0)

    

#define DEFINE_STRUCT_TREE_REMOVE(NAME, T) \
    void NAME##_remove(struct NAME *_node) \
    { \
        STRUCT_TREE_REMOVE(_node); \
    }
#define DEFINE_STRUCT_TREE_ATTACH(NAME, T) \
    void NAME##_attach(struct NAME *_node, struct NAME *_attach, int _dir, int _child_dir) \
    { \
        if (_dir == 0 && _child_dir == 0) \
            return; \
        STRUCT_TREE_REMOVE(_node); \
        if (_dir == 0) { \
            if (_child_dir > 0)  { \
                STRUCT_TREE_ATTACH_LAST(_node, _attach); \
            } else if (_child_dir < 0) { \
                STRUCT_TREE_ATTACH_FIRST(_node, _attach); \
            } \
        } else if (_dir > 0) { \
            STRUCT_TREE_ATTACH_AFTER(_node, _attach); \
        } else if (_dir < 0) { \
            STRUCT_TREE_ATTACH_BEFORE(_node, _attach); \
        } \
    }
#define DEFINE_STRUCT_TREE_ATTACH_AFTER(NAME, T) \
    void NAME##_attach_after(struct NAME *_node, struct NAME *_attach) \
    { \
        STRUCT_TREE_REMOVE(_attach); \
        STRUCT_TREE_ATTACH_AFTER(_node, _attach); \
    }
#define DEFINE_STRUCT_TREE_ATTACH_BEFORE(NAME, T) \
    void NAME##_attach_before(struct NAME *_node, struct NAME *_attach) \
    { \
        STRUCT_TREE_REMOVE(_attach); \
        STRUCT_TREE_ATTACH_BEFORE(_node, _attach); \
    }
#define DEFINE_STRUCT_TREE_ATTACH_FIRST(NAME, T) \
    void NAME##_attach_first(struct NAME *_node, struct NAME *_attach) \
    { \
        STRUCT_TREE_REMOVE(_attach); \
        STRUCT_TREE_ATTACH_FIRST(_node, _attach); \
    }
#define DEFINE_STRUCT_TREE_ATTACH_LAST(NAME, T) \
    void NAME##_attach_last(struct NAME *_node, struct NAME *_attach) \
    { \
        STRUCT_TREE_REMOVE(_attach); \
        STRUCT_TREE_ATTACH_LAST(_node, _attach); \
    }

#define DEFINE_STRUCT_TREE(NAME, T) \
    DEFINE_STRUCT_TREE_TYPEDEF_STRUCT(NAME, T) \
    DEFINE_STRUCT_TREE_NEW(NAME, T) \
    DEFINE_STRUCT_TREE_PARENT(NAME, T) \
    DEFINE_STRUCT_TREE_PREV(NAME, T) \
    DEFINE_STRUCT_TREE_NEXT(NAME, T) \
    DEFINE_STRUCT_TREE_FIRST(NAME, T) \
    DEFINE_STRUCT_TREE_LAST(NAME, T) \
    DEFINE_STRUCT_TREE_REMOVE(NAME, T) \
    DEFINE_STRUCT_TREE_ATTACH(NAME, T) \
    DEFINE_STRUCT_TREE_ATTACH_AFTER(NAME, T) \
    DEFINE_STRUCT_TREE_ATTACH_BEFORE(NAME, T) \
    DEFINE_STRUCT_TREE_ATTACH_FIRST(NAME, T) \
    DEFINE_STRUCT_TREE_ATTACH_LAST(NAME, T) \
    DEFINE_STRUCT_TREE_FREE(NAME, T)

#include "undef.h"
