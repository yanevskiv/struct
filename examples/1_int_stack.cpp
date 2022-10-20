/**
 * (c) Ivan Janevski
 *
 * Stack example
 *
 * Expected result: 2 9 3 1
 */
#define STRUCT_STACK
#include <struct/list.h>
#include <stdio.h>

DEFINE_STRUCT_LIST(list, int)

int main() {
    list_t *list = list_new();
    list_push(list, 1);
    list_push(list, 3);
    list_push(list, 9);
    list_push(list, 2);
    int x;
    while (list_pop(list, &x)) {
        printf("%d ", x);
    }
    list_free(list);
    return 0;
}
