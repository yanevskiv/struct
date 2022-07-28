#include <stdlib.h>
#include "list.h"

list_elem_t* list_elem_new(list_data_t data)
{
    list_elem_t* elem = (list_elem_t*) calloc(1, sizeof(list_elem_t));
    elem->le_data = data;
    elem->le_prev = NULL;
    elem->le_next = NULL;
    return elem;
}

void list_elem_free(list_elem_t* elem)
{
    free(elem);
}

list_t* list_new() 
{
    list_t *list = (list_t*)calloc(1, sizeof(list_t));
    list->l_head = NULL;
    list->l_tail = NULL;
    return list;
}

void list_free(list_t* list) 
{
    free(list);
}

void list_insert_after(list_t* list, list_elem_t* after, list_elem_t* elem)
{
    elem->le_prev = NULL;
    elem->le_next = NULL;
    if (after == NULL) {
        elem->le_next = list->l_head;
        list->l_head->le_prev = elem;
        list->l_head = elem;
    } else if (after == list->l_tail) {
        list->l_tail->le_next = elem;
        elem->le_prev = list->l_tail;
        list->l_tail = elem;
    } else {
        elem->le_next = after->le_next;
        elem->le_prev = after;
        after->le_next->le_prev = elem;
        after->le_next = elem;
    }
}

void list_insert_before(list_t* list, list_elem_t* before, list_elem_t* elem)
{
    elem->le_prev = NULL;
    elem->le_next = NULL;
    if (before == NULL) {
        elem->le_prev = list->l_tail;
        list->l_tail->le_next = elem;
        list->l_tail = elem;
    } else if (before == list->l_head) {
        list->l_head->le_prev = elem;
        elem->le_next = list->l_head;
        list->l_head = elem;
    } else {
        elem->le_next = before;
        elem->le_prev = before->le_prev;
        before->le_prev->le_next = elem;
        before->le_prev = elem;
    }
}

void list_remove_elem(list_t* list, list_elem_t* elem)
{
    if (elem->le_prev) {
        elem->le_prev->le_next = elem->le_prev;
    } else {
        list->l_head = elem->le_next;
        list->l_head->le_prev = NULL;
    }
    if (elem->le_next) {
        elem->le_next->le_prev = elem->le_next;
    } else {
        list->l_tail = elem->le_prev;
        list->l_tail->le_next = NULL;
    }
    elem->le_prev = NULL;
    elem->le_next = NULL;
}


void list_push_head(list_t* list, list_data_t data)
{
    list_elem_t* elem = list_elem_new(data);
    if (! list->l_head) { 
        list->l_head = elem;
        list->l_tail = elem;
    } else {
        elem->le_next = list->l_head;
        list->l_head->le_prev = elem;
        list->l_head = elem;
    }
}

void list_push_tail(list_t* list, list_data_t data)
{
    list_elem_t* elem = list_elem_new(data);
    if (! list->l_tail) {
        list->l_head = elem;
        list->l_tail = elem;
    } else {
        elem->le_prev = list->l_tail;
        list->l_tail->le_next = elem;
        list->l_tail = elem;
    }
}

list_data_t list_pop_head(list_t* list)
{
    if (list->l_head) {
        list_elem_t* elem = list->l_head;
        if (list->l_head == list->l_tail) {
            list->l_head = NULL;
            list->l_tail = NULL;
        } else {
            list->l_head = list->l_head->le_next;
            list->l_head->le_prev = NULL;
        }
        list_data_t data = elem->le_data;
        list_elem_free(elem);
        return data;
    }
}

list_data_t list_pop_tail(list_t* list)
{
    if (list->l_tail != NULL) {
        list_elem_t* elem = list->l_tail;
        if (list->l_head == list->l_tail) {
            list->l_head = NULL;
            list->l_tail = NULL;
        } else {
            list->l_tail = list->l_tail->le_prev;
            list->l_tail->le_next = NULL;
        }
        list_data_t data = elem->le_data;
        list_elem_free(elem);
        return data;
    }
}

list_data_t list_fetch_at(list_t* list, int at)
{
    list_elem_t* iter = list->l_head;
    while (iter) {
        if (at == 0) {
            return iter->le_data;
        }
        at -= 1;
        iter = iter->le_next;
    }
}

list_data_t list_remove_at(list_t* list, int at)
{
    list_elem_t* iter = list->l_head;
    while (iter) {
        if (at == 0) {
            if (iter->le_prev) {
                iter->le_prev->le_next = iter->le_next;
            } else {
                list->l_head = iter->le_next;
                list->l_head->le_prev = NULL;
            }

            if (iter->le_next) {
                iter->le_next->le_prev = iter->le_prev;
            } else {
                list->l_tail = iter->le_prev;
                list->l_tail->le_next = NULL;
            }

            list_data_t data = iter->le_data;
            list_elem_free(iter);
            return data;
        }
        at -= 1;
        iter = iter->le_next;
    }
}
    
void list_insert_at(list_t* list, list_data_t data, int at)
{
    list_elem_t *elem = list_elem_new(data);
    if (! list->l_head) {
        list->l_head = elem;
        list->l_tail = elem;
    } else if (at <= 0){
        elem->le_next = list->l_head;
        list->l_head->le_prev = elem;
        list->l_head = elem;
    } else {
        list_elem_t *after = list->l_head;
        while ((at - 1) > 0 && after->le_next) {
            after = after->le_next;
            at -= 1;
        }
        elem->le_next = after->le_next;
        elem->le_prev = after;
        if (after == list->l_tail) {
            list->l_tail = elem;
        } else {
            after->le_next->le_prev = elem;
        }
        after->le_next = elem;
    }
}

void list_insert_sorted(list_t* list, list_data_t data, int (*compare_func)(list_data_t, list_data_t))
{
    list_elem_t* elem = list_elem_new(data);
    if (! list->l_head) {
        list->l_head = elem;
        list->l_tail = elem;
    } else if (compare_func(list->l_head->le_data, data) <= 0) {
        list->l_head->le_prev = elem;
        elem->le_next = list->l_head;
        list->l_head = elem;
    } else {
        list_elem_t* iter = list->l_head;
        while (iter->le_next)  {
            if (compare_func(iter->le_next->le_data, data) <= 0) 
                break;
            iter = iter->le_next;
        }
        elem->le_prev = iter;
        elem->le_next = iter->le_next; 
        if (iter == list->l_tail) {
            list->l_tail = iter;
        } else {
            iter->le_next->le_prev = elem;
        }
        iter->le_next = elem;
    }
}

void list_sort(list_t* list, int (*compare_func)(list_data_t, list_data_t))
{
    list_elem_t *iter = list->l_head, *head = NULL, *tail = NULL;
    while (iter) {
        list_elem_t* next = iter->le_next;
        iter->le_prev = NULL;
        iter->le_next = NULL;
        if (! head) {
            head = iter;
            tail = iter;
        } else if (compare_func(head->le_data, iter->le_data) <= 0) {
            head->le_prev = iter;
            iter->le_next = head;
            head = iter;
        } else {
            list_elem_t* after = head;
            while (after->le_next) {
                if (compare_func(after->le_next->le_data, iter->le_data) <= 0)
                    break;
                after = after->le_next;
            }
            iter->le_prev = after;
            iter->le_next = after->le_next;
            if (after == tail) {
                tail = iter;
            } else {
                after->le_next->le_prev = iter;
            }
            after->le_next = iter;
        }
        iter = next;
    }
    list->l_head = head;
    list->l_tail = tail;
}

int list_empty(list_t* list)
{
    return (list->l_head == NULL);
}

int list_count(list_t* list)
{
    list_elem_t* iter = list->l_head;
    int count = 0;
    while (iter) {
        count += 1;
        iter = iter->le_next;
    }
    return count;
}

void list_clear(list_t* list)
{
    list_elem_t *iter = list->l_head, *next = NULL;
    while (iter) {
        next = iter->le_next;
        list_elem_free(iter);
        iter = next;
    }
    list->l_head = NULL;
    list->l_tail = NULL;
}

void list_for_each(list_t* list, void (*callback_func)(list_data_t))
{
    list_elem_t* iter = list->l_head;
    while (iter) {
        callback_func(iter->le_data);
        iter = iter->le_next;
    }
}

void list_reverse(list_t* list)
{
    list_elem_t *iter = list->l_head, *head = NULL, *tail = NULL;
    while (iter) {
        list_elem_t* next = iter->le_next;
        iter->le_next = NULL;
        iter->le_prev = NULL;
        if (! head) {
            head = iter;
            tail = iter;
        } else {
            iter->le_next = head;
            head->le_prev = iter;
            head = iter;
        }
        iter = next;
    } 
    list->l_head = head;
    list->l_tail = tail;
}

void list_concat(list_t* list1, list_t* list2)
{
    if (list1->l_tail)
        list1->l_tail->le_next = list2->l_head;
    if (list2->l_head)
        list2->l_head->le_prev = list1->l_tail;
    list1->l_tail = list2->l_tail;
    list2->l_head = list1->l_head;
}

list_t* list_slice(list_t* list, int from, int to)
{
    list_t* result = list_new();
    list_elem_t* iter = list->l_head;
    while (from > 0 && iter) {
        iter = iter->le_next;
        from -= 1;
        to -= 1;
    }
    while (to > 0 && iter) {
        list_push_tail(result, iter->le_data);
        iter = iter->le_next;
        to -= 1;
    }
    return result;
}


list_t* list_clone(list_t* list)
{
    list_t* result = list_new();
    list_elem_t* iter = list->l_head;
    while (iter) {
        list_push_tail(result, iter->le_data);
        iter = iter->le_next;
    }
    return result;
}


list_t* list_filter(list_t* list, int (*filter_func)(list_data_t))
{
    list_t* result = list_new();
    list_elem_t* iter = list->l_head;
    while (iter) {
        if (filter_func(iter->le_data))
            list_push_tail(result, iter->le_data);
        iter = iter->le_next;
    }
    return result;
}

list_data_t list_reduce(list_t* list, list_data_t (*reduce_func)(list_data_t, list_data_t))
{
    if (list->l_head)  {
        list_data_t result = list->l_head->le_data;
        list_elem_t* iter = list->l_head->le_next;
        while (iter) {
            result = reduce_func(result, iter->le_data);
            iter = iter->le_next;
        }
    }
}

void list_map(list_t* list, list_data_t (*map_func)(list_data_t))
{
    list_elem_t* iter = list->l_head;
    while (iter) {
        iter->le_data = map_func(iter->le_data);
        iter = iter->le_next;
    }
}
