#ifndef __VECTOR_H__
#define __VECTOR_H__


typedef void* vector_data_t;
typedef struct vector vector_t;

struct vector { 
    vector_data_t *v_data;
    int v_count;
    int v_capacity;
} vector_t;

vector_t *vector_new(int);

#endif
