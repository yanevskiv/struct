#include <array.h>
#include <stdlib.h>

array_t* array_new(int capacity)
{
    array_t* array = (array_t*)calloc(capacity, sizeof(array_t));
    if (! array)  {
        return NULL;
    }
    array->a_count = 0;
    array->a_capacity = capacity;
    array->a_data = (array_data_t*)calloc(capacity, sizeof(array_data_t));
    if (! array->a_data) {
        free(array);
        return NULL;
    }
    return array;
}

void array_free(array_t* array)
{
    free(array->a_data);
    free(array);
}

void array_set(array_t* array, int at, array_data_t data)
{
    array->a_data[at] = data;
}

