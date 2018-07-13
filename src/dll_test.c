#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"

int main() {
    dll* list = (dll*)dll_new();
    dll_insert(list, "a", "b");
    dll_insert(list, "b", "b");
    dll_insert(list, "c", "b");
    dll_insert(list, "d", "b");
    dll_insert(list, "e", "b");
    dll_insert(list, "f", "b");
    dll_insert(list, "g", "b");
    dll_insert(list, "h", "b");
    dll_print(list);
    printf("Deleting 1..\n");
    dll_print(list);
    printf("Deleting..\n");
    dll_delete(list);
    return 0;
}