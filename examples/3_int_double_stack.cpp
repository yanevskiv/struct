/**
 * (c) Ivan Janevski
 *
 * Double stack example
 *
 * Expected result: 8 2 4 1 9 3
 */
#define STRUCT_DOUBLE_STACK
#include <struct/list.h>
#include <stdio.h>

DEFINE_STRUCT_LIST(list, int)

int main() {
    list_t *list = list_new();

    // head - beginning
    // tail - end
    list_push_head(list, 4);
    list_push_head(list, 2);
    list_push_head(list, 8);
    list_push_tail(list, 1);
    list_push_tail(list, 9);
    list_push_tail(list, 3);

    // pop everything and print
    //
    // equivalnt code:
    //
    //    int data;
    //    while (list_pop_head(list, &data)) {
    //        printf("%d ", data);
    //    }
    //
    list_elem_t *iter = list_head(list);
    while ((iter = list_pop_elem_head(list))) {
        int data = list_elem_data(iter);
        printf("%d ", data);
        list_elem_free(iter);
    }

    list_free(list);
    return 0;
}
