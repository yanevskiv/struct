/**
 * (c) Ivan Janevski
 *
 * Queue example
 *
 * Expected result: 1 3 9 2
 */
#define STRUCT_QUEUE
#include <struct/list.h>
#include <stdio.h>

DEFINE_STRUCT_LIST(list, int)

int main() {
    list_t *list = list_new();
    list_enqueue(list, 1);
    list_enqueue(list, 3);
    list_enqueue(list, 9);
    list_enqueue(list, 2);
    int x;
    while (list_dequeue(list, &x)) {
        printf("%d ", x);
    }
    list_free(list);
    return 0;
}
