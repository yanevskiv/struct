/**
 * Tree
 */

#define DEFINE_STRUCT_TREE_STRUCT(NAME, T) \
    struct NAME { \
        T t_data; \
        struct NAME##_node *t_parent; \
        struct NAME##_node *t_prev; \
        struct NAME##_node *t_next; \
        struct NAME##_node *t_head; \
        struct NAME##_node *t_tail; \
    };

#define DEFINE_STRUCT_TREE(NAME, T)
    
