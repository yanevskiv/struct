#ifndef __ARRAY_H__
#define __ARRAY_H__

typedef void* array_data_t;
typedef struct array array_t;

struct array {
    int a_count;
    array_data_t* a_data;
};

array_t* array_new(int);
void array_free(array_t*);

void array_set(array_t*, int, array_data_t);
void array_get(array_t*, int);
void array_insert(array_t*, int, array_data_t);
void array_delete_at(array_t*, int);
void array_index_of(array_t*, int (*)(array_data_t, array_data_t));

void array_push_head(array_t*, array_data_t);
void array_push_tail(array_t*, array_data_t);
array_data_t array_pop_head(array_t*);
array_data_t array_pop_tail(array_t*);

void array_trim(array_t*);
int array_capacity(array_t*);

int array_empty(array_t*);
int array_full(array_t*);
int array_count(array_t*);
void array_clear(array_t*);
void array_reverse(array_t*);
array_t* array_concat(array_t*);
array_t* array_slice(array_t*, int, int);
array_t* array_clone(array_t*);

void array_for_each(array_t*, void (*)(array_data_t));
void array_map(array_t*, array_data_t (*)(array_data_t));
array_t* list_filter(array_t*, int (*)(array_data_t));
void array_sort(array_t*, int (*)(array_data_t, array_data_t));


#endif

