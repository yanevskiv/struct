#ifndef __BUFFER_H__
#define __BUFFER_H__

typedef struct buffer buffer_t;

struct buffer {
    int b_head;
    int b_tail;
    int b_size;
    int b_count;
    void *b_data;
};

buffer_t* buffer_new(int);
void buffer_free(buffer_t*);

void buffer_push_head(buffer_t*, void*);
void buffer_push_tail(buffer_t*, void*);
void* buffer_pop_head(buffer_t*);
void* buffer_pop_tail(buffer_t*);

void* buffer_get(buffer_t*);
void buffer_set(buffer_t*);
int buffer_count(buffer_t*);
int buffer_size(buffer_t*);
int buffer_insert_at(buffer_t*, int, void*);

int buffer_empty(buffer_t*);
int buffer_count(buffer_t*);
void buffer_clear(buffer_t*);
void buffer_reverse(buffer_t*);
void buffer_concat(buffer_t*, buffer_t*);
buffer_t* buffer_slice(buffer_t*, int, int);
buffer_t* buffer_clonce(buffer_t*);

void buffer_for_each(buffer_t*, void (*)(void*));
void buffer_map(buffer_t*, void* (*)(void*));
buffer_t* buffer_filter(buffer_t*, int(*)(void*));
void* buffer_reduce(buffer_t*, void* (*)(void*, void*));
void buffer_sort(buffer_t*, int(*)(void*, void*));


#endif
