/**
 * B+ Tree 
 */

#include "common.h"


// define: typedef
#define DEFINE_STRUCT_BPTREE_TYPEDEF(NAME, K, V) \
    typedef struct NAME##_node_st NAME##_node_t; \
    typedef struct NAME##_st NAME##_t;

// node types
#undef STRUCT_BPTREE_NODE_INTERNAL
#undef STRUCT_BPTREE_NODE_LEAF
#define STRUCT_BPTREE_NODE_INTERNAL 0
#define STRUCT_BPTREE_NODE_LEAF 1

// define: struct
#define DEFINE_STRUCT_BPTREE_STRUCT(NAME, K, V) \
    struct NAME##_node_st { \
        int    bptn_type; \
        int    bptn_count; \
        K     *bptn_keys; \
        void **bptn_ptrs; \
        struct NAME##_node_st *bptn_parent; \
    }; \
    struct NAME##_st { \
        int bpt_order;  \
        struct NAME##_node_st *bpt_root; \
        STRUCT_MUTEX_DECLARE(bpt_mutex); \
        STRUCT_MT_SAFE_DECLARE(bpt_mt_safe); \
    };

// define: new
#define DEFINE_STRUCT_BPTREE_NEW(NAME, K, V) \
    STRUCT_ATTRIB struct NAME##_st *NAME##_new(int _order) \
    { \
        struct NAME##_st *_tree = (struct NAME##_st*) STRUCT_CALLOC(1, struct NAME##_st); \
        _tree->bpt_order = _order; \
        _tree->bpt_root = STRUCT_NULL; \
        STRUCT_MUTEX_INIT(_tree->bpt_mutex) \
        STRUCT_MT_SAFE_INIT(_tree->bpt_mutex) \
        return _tree; \
    }

// define: free
#define DEFINE_STRUCT_BPTREE_FREE(NAME, K, V) \
    STRUCT_ATTRIB void NAME##_free(struct NAME##_st *_tree) \
    { \
        STRUCT_MUTEX_DESTROY(_tree->bpt_mutex); \
        STRUCT_MT_SAFE_DESTROY(_tree->bpt_mt_safe); \
        STRUCT_FREE(_tree); \
    }

// define: root
#define DEFINE_STRUCT_BPTREE_ROOT(NAME, K, V) \
    STRUCT_ATTRIB struct NAME##_node_st *NAME##_root(struct NAME##_st *_tree) \
    { \
        STRUCT_MT_SAFE_LOCK(_tree->bpt_mt_safe) \
        struct NAME##_node_st *_root = _tree->bpt_root; \
        STRUCT_MT_SAFE_UNLOCK(_tree->bpt_mt_safe) \
        return _root; \
    }

// define: order
#define DEFINE_STRUCT_BPTREE_ORDER(NAME, K, V) \
    STRUCT_ATTRIB int NAME##_order(struct NAME##_st *_tree) \
    { \
        STRUCT_MT_SAFE_LOCK(_tree->bpt_mt_safe) \
        int _order = _tree->bpt_order; \
        STRUCT_MT_SAFE_UNLOCK(_tree->bpt_mt_safe) \
        return _order; \
    }

// define: lock
#define DEFINE_STRUCT_BPTREE_LOCK(NAME, K, V) \
    STRUCT_ATTRIB void NAME##_lock(struct NAME##_st *_tree) \
    { \
        STRUCT_MUTEX_LOCK(_tree->bpt_mutex); \
    }

// define: unlock
#define DEFINE_STRUCT_BPTREE_UNLOCK(NAME, K, V) \
    STRUCT_ATTRIB void NAME##_unlock(struct NAME##_st *_tree) \
    { \
        STRUCT_MUTEX_LOCK(_tree->bpt_mutex); \
    }

// define: mt_safe_lock
#define DEFINE_STRUCT_BPTREE_MT_SAFE_LOCK(NAME, K, V) \
    STRUCT_ATTRIB void NAME##_mt_safe_lock(struct NAME##_st *_tree) \
    { \
        STRUCT_MT_SAFE_LOCK(_tree->bpt_mt_safe); \
    }

// define: mt_safe_unlock
#define DEFINE_STRUCT_BPTREE_MT_SAFE_UNLOCK(NAME, K, V) \
    STRUCT_ATTRIB void NAME##_mt_safe_unlock(struct NAME##_st *_tree) \
    { \
        STRUCT_MT_SAFE_UNLOCK(_tree->bpt_mt_safe); \
    }

// define: node_new
#define DEFINE_STRUCT_BPTREE_NODE_NEW(NAME, K, V) \
    STRUCT_ATTRIB struct NAME##_node_st *NAME##_node_new(int _order, int _type)  \
    { \
        struct NAME##_node_st *_node = STRUCT_CALLOC(1, sizeof(struct NAME##_node_st)); \
        _node->bptn_type = _type; \
        _node->bptn_count = 0; \
        _node->bptn_keys = STRUCT_CALLOC(_order - 1, sizeof(K)); \
        _node->bptn_ptrs = STRUCT_CALLOC(_order, sizeof(void*)); \
        _node->bptn_parent = STRUCT_NULL; \
        return _node; \
    }
    
// define: node_free
#define DEFINE_STRUCT_BPTREE_NODE_FREE(NAME, K, V) \
    STRUCT_ATTRIB void NAME##_node_free(struct NAME##_node_st *_node) \
    { \
        STRUCT_FREE(_node->bptn_keys); \
        STRUCT_FREE(_node->bptn_ptrs); \
        STRUCT_FREE(_node); \
    }

// define: node_count
#define DEFINE_STRUCT_BPTREE_NODE_COUNT(NAME, K, V) \
    STRUCT_ATTRIB int NAME##_node_count(struct NAME##_node_st *_node) \
    { \
        return _node->bptn_count; \
    }

// define: node_is_leaf
#define DEFINE_STRUCT_BPTREE_NODE_IS_LEAF(NAME, K, V) \
    STRUCT_ATTRIB int NAME##_node_is_leaf(struct NAME##_node_st *_node) \
    { \
        return _node->bptn_type == STRUCT_BPTREE_NODE_LEAF; \
    }

// define: node_is_internal
#define DEFINE_STRUCT_BPTREE_NODE_IS_INTERNAL(NAME, K, V) \
    STRUCT_ATTRIB int NAME##_node_is_internal(struct NAME##_node_st *_node) \
    { \
        return _node->bptn_type == STRUCT_BPTREE_NODE_INTERNAL; \
    }

// define: node_is_root
#define DEFINE_STRUCT_BPTREE_NODE_IS_ROOT(NAME, K, V) \
    STRUCT_ATTRIB int NAME##_node_is_root(struct NAME##_node_st *_node) \
    { \
        return _node->bptn_parent == STRUCT_NULL; \
    }

// define: node_ptrs
#define DEFINE_STRUCT_BPTREE_NODE_PTRS(NAME, K, V) \
    STRUCT_ATTRIB void **NAME##_node_ptrs(struct NAME##_node_st *_node) \
    { \
        return _node->bptn_ptrs; \
    }

// define: node_keys
#define DEFINE_STRUCT_BPTREE_NODE_KEYS(NAME, K, V) \
    STRUCT_ATTRIB K *NAME##_node_keys(struct NAME##_node_st *_node) \
    { \
        return _node->bptn_keys; \
    }

// define: node_parent
#define DEFINE_STRUCT_BPTREE_NODE_PARENT(NAME, K, V) \
    STRUCT_ATTRIB struct NAME##_node_st *NAME##_node_parent(struct NAME##_node_st *_node) \
    { \
        return _node->bptn_parent; \
    }

// define: insert
#define DEFINE_STRUCT_BPTREE_INSERT(NAME, K, V) \
    STRUCT_ATTRIB void NAME##_insert(struct NAME##_st *_tree, K _key, V _data) \
    { \
        STRUCT_MT_SAFE_LOCK(_tree->bpt_mt_safe) \
        STRUCT_MT_SAFE_UNLOCK(_tree->bpt_mt_safe) \
    } \

// define: delete
#define DEFINE_STRUCT_BPTREE_DELETE(NAME, K, V) \
    STRUCT_ATTRIB void NAME##_delete(struct NAME##_st *_tree, K _Key) \
    { \
        STRUCT_MT_SAFE_LOCK(_tree->bpt_mt_safe) \
        STRUCT_MT_SAFE_UNLOCK(_tree->bpt_mt_safe) \
    } \

// define: search
#define DEFINE_STRUCT_BPTREE_SEARCH(NAME, K, V) \
    STRUCT_ATTRIB void NAME##_search(struct NAME##_st *_tree, K _key) \
    { \
        STRUCT_MT_SAFE_LOCK(_tree->bpt_mt_safe) \
        STRUCT_MT_SAFE_UNLOCK(_tree->bpt_mt_safe) \
    } \

// define: for_each
#define DEFINE_STRUCT_BPTREE_FOR_EACH(NAME, K, V) \
    STRUCT_ATTRIB void NAME##_for_each(struct NAME##_st *_tree, void (*_func)(V)) \
    { \
        STRUCT_MT_SAFE_LOCK(_tree->bpt_mt_safe) \
        STRUCT_MT_SAFE_UNLOCK(_tree->bpt_mt_safe) \
    } \

// define: bptree
#define DEFINE_STRUCT_BPTREE(NAME, K, V) \
    DEFINE_STRUCT_BPTREE_TYPEDEF(NAME, K, V) \
    DEFINE_STRUCT_BPTREE_STRUCT(NAME, K, V) \
    DEFINE_STRUCT_BPTREE_NEW(NAME, K, V) \
    DEFINE_STRUCT_BPTREE_FREE(NAME, K, V) \
    DEFINE_STRUCT_BPTREE_ROOT(NAME, K, V) \
    DEFINE_STRUCT_BPTREE_ORDER(NAME, K, V) \
    DEFINE_STRUCT_BPTREE_LOCK(NAME, K, V) \
    DEFINE_STRUCT_BPTREE_UNLOCK(NAME, K, V) \
    DEFINE_STRUCT_BPTREE_MT_SAFE_LOCK(NAME, K, V) \
    DEFINE_STRUCT_BPTREE_MT_SAFE_UNLOCK(NAME, K, V) \
    \
    DEFINE_STRUCT_BPTREE_NODE_NEW(NAME, K, V) \
    DEFINE_STRUCT_BPTREE_NODE_FREE(NAME, K, V) \
    DEFINE_STRUCT_BPTREE_NODE_COUNT(NAME, K, V) \
    DEFINE_STRUCT_BPTREE_NODE_IS_LEAF(NAME, K, V) \
    DEFINE_STRUCT_BPTREE_NODE_IS_INTERNAL(NAME, K, V) \
    DEFINE_STRUCT_BPTREE_NODE_IS_ROOT(NAME, K, V) \
    DEFINE_STRUCT_BPTREE_NODE_PTRS(NAME, K, V) \
    DEFINE_STRUCT_BPTREE_NODE_KEYS(NAME, K, V) \
    DEFINE_STRUCT_BPTREE_NODE_PARENT(NAME, K, V) \
    \
    DEFINE_STRUCT_BPTREE_INSERT(NAME, K, V) \
    DEFINE_STRUCT_BPTREE_DELETE(NAME, K, V) \
    DEFINE_STRUCT_BPTREE_SEARCH(NAME, K, V) \
    \
    DEFINE_STRUCT_BPTREE_FOR_EACH(NAME, K, V)



