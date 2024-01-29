#define STRUCT_STACK
#include <stdio.h>
#include <struct/list.h>

DEFINE_STRUCT_LIST(list, int)

int main() {
    list_t *list = list_new();
    list_push(list, 4);
    list_push(list, 4);
    list_push(list, 4);
    list_push(list, 4);
    while (! list_empty(list)) {
        int data;
        list_pop(list, &data);
        printf("%d ", data);
    }
    list_free(list);
    return 0;
}
