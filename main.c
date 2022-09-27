#include <stdio.h>
#include <struct/tpl/list.h>


DEFINE_STRUCT_LIST(list, int)

int main() {
    list_t *list = list_new();
    list_push(list, 4);
    list_push(list, 6);
    list_push(list, 6);
    list_push(list, 8);
    list_push(list, 7);
    list_free(list);
    return 0;
}
