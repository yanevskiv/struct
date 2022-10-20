/**
 * (c) Ivan Janevski
 *
 * String list
 *
 * Expected result:
 *     Hello world! How is everybody?
 */
#define STRUCT_VA
#define STRUCT_QUEUE
#include <struct/list.h>
#include <stdio.h>
#include <string.h>

DEFINE_STRUCT_LIST(sl, const char*)

int main() {
    sl_t *sl = sl_new();
    // words
    sl_enqueue(sl, "Hello");
    sl_enqueue(sl, "World!");
    sl_enqueue(sl, "How");
    sl_enqueue(sl, "is");
    sl_enqueue(sl, "everybody");
    sl_enqueue(sl, "?");
    const char *iter;
    while (sl_dequeue(sl, &iter) > 0)  {
        printf("%s ", iter); // adds space between words
    }
    sl_free(sl);
    return 0;
}


