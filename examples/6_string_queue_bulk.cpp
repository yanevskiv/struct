/**
 * (c) Ivan Janevski
 *
 * Variadic enqueue + string list + for each
 *
 * Expected result (word "red" colored in red):
 *
 *    This is supposed to be red!
 */
#define STRUCT_VA
#define STRUCT_QUEUE
#define STRUCT_BULK
#include <struct/list.h>
#include <stdio.h>
#include <string.h>
#define RED_COLOR "\x1b[31m"
#define NO_COLOR "\x1b[0m"

//
// Function to check if two strings are equal
//
int streq(const char *str1, const char *str2) 
{
    return str1 == str2 || (str1 != NULL && str2 != NULL && strcmp(str1, str2) == 0);
}

//
// List with a custom defined `==` operator
//
#undef STRUCT_EQUALS
#define STRUCT_EQUALS(arg1, arg2) streq((arg1), (arg2))
DEFINE_STRUCT_LIST(sl, const char*)

//
// Print word with space after the word, except the word "red"
//
void print_str(const char* str) {
    if (streq(str, "red")) {
        printf(RED_COLOR "red" NO_COLOR);
    } else {
        printf("%s ", str);
    }
}

int main() {
    sl_t sl;
    sl_init(&sl);
    sl_enqueue_va(&sl, "(key)", "This", "is", "supposed", "to be", "red", "!", "(key)");
    sl_for_each(&sl, print_str);
    sl_destroy(&sl);
    return 0;
}
