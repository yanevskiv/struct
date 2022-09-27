/* 
 * MIT License
 * 
 * Copyright (c) 2022 Ivan Janevski
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __LIST_H__
#define __LIST_H__

// structure definition
typedef void* list_data_t;
typedef struct list list_t;
typedef struct list_elem list_elem_t;

struct list_elem {
    list_data_t le_data;
    list_elem_t* le_prev;
    list_elem_t* le_next;
};

struct list {
    list_elem_t* l_head;
    list_elem_t* l_tail;
};

// Memory allocation
list_elem_t* list_elem_new(list_data_t);
void list_elem_free(list_elem_t*);
list_t* list_new();
void list_free(list_t*);

// Appending and Prepending
void list_push_head(list_t*, list_data_t);
void list_push_tail(list_t*, list_data_t);
list_data_t list_pop_head(list_t*);
list_data_t list_pop_tail(list_t*);

// Insertion of an element
void list_insert_after(list_t*, list_elem_t*, list_elem_t*);
void list_insert_before(list_t*, list_elem_t*, list_elem_t*);
void list_remove(list_t*, list_elem_t*);
list_elem_t* list_find_elem(list_t*, int (*)(list_data_t, list_data_t));

// Finding data
list_data_t list_fetch_at(list_t*, int);
list_data_t list_remove_at(list_t*, int);
void list_insert_at(list_t*, list_data_t, int);
int list_index_of(list_t*, int (*)(list_data_t, list_data_t));

// Useful list operations
int list_empty(list_t*);
int list_count(list_t*);
void list_clear(list_t*);
void list_reverse(list_t*);
void list_concat(list_t*, list_t*);
list_t* list_slice(list_t*, int, int);
list_t* list_clone(list_t*);

// Misc
void list_for_each(list_t*, void (*)(list_data_t));
void list_map(list_t*, list_data_t (*)(list_data_t));
list_t* list_filter(list_t*, int(*)(list_data_t));
list_data_t list_reduce(list_t*, list_data_t (*)(list_data_t, list_data_t));
void list_insert_sorted(list_t*, list_data_t, int (*)(list_data_t, list_data_t));
void list_sort(list_t*, int (*)(list_data_t, list_data_t));

#endif
