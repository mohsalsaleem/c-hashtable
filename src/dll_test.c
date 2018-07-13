#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"

int main() {
    dll* list = (dll*)dll_new();
    Node* node = dll_search(list, "h");
    if(node != NULL) {
        printf("Search: %s\n", node->pair->value);
    } else {
        printf("Not found\n");
    }
    dll_insert(list, "a", "b");
    dll_insert(list, "b", "b");
    dll_insert(list, "c", "b");
    dll_insert(list, "d", "d");
    dll_insert(list, "e", "b");
    dll_insert(list, "f", "b");
    dll_insert(list, "g", "a");
    dll_insert(list, "h", "b");
    dll_insert(list, "h", "v");
    dll_print(list);
    dll_delete_by_key(list, "g");
    dll_delete_by_key(list, "d");

    node = dll_search(list, "h");
    if(node != NULL) {
        printf("Search: %s\n", node->pair->value);
    } else {
        printf("Not found\n");
    }

    printf("Deleting 1..\n");
    dll_print(list);
    printf("Deleting..\n");
    dll_delete_list(list);
    return 0;
}