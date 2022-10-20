/**
 * (c) Ivan Janevski
 *
 * Variadic arguments example
 *
 * Expected result:
 *   9 5 2 3 1
 *   9 5 2 3 1
 */
#define STRUCT_VA
#define STRUCT_STACK
#include <struct/list.h>
#include <stdio.h>

DEFINE_STRUCT_LIST(list, int)

int main() {
    list_t *list = list_new();

    //
    // second argument of `list_push_va` is the `key` value used
    // to signify when the variadic arguments stop
    //
    // `list_pop_va` returns the number of arguments successfully popped
    // last argument of `list_pop_va` must be `NULL`
    int x;
    list_push_va(list, -1, 1, 3, 2, 5, 9, -1);
    while (list_pop_va(list, &x, NULL) > 0) {
        printf("%d ", x);
    }
    printf("\n");

    //
    // same example, using `elem` functions
    // last argument of `list_push_elem_va` must be `NULL`
    // last argument of `list_pop_elem_va` also must be `NULL`
    //
    // like `list_pop_va`, `list_pop_elem_va` returns the number of elements
    // successfully popped.
    list_push_elem_va(list,
        list_elem_new(1),
        list_elem_new(3),
        list_elem_new(2),
        list_elem_new(5),
        list_elem_new(9),
        NULL
    );
    list_elem_t *iter;
    while (list_pop_elem_va(list, &iter, NULL) > 0) {
        printf("%d ", list_elem_data(iter));
    }
    printf("\n");
    list_free(list);
    return 0;
}
