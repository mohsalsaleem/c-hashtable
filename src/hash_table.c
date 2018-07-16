#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "hash_table.h"
#include "prime.h"

#define HT_PRIME_1 151
#define HT_PRIME_2 157
#define HT_INTIAL_BASE_SIZE 50

static ht_pair HT_DELETED_PAIR = {NULL, NULL};
static dll DL_EMPTY_LIST = {NULL, NULL};

static void log_char(const char* str) {
	printf("%s\n", str);
}

static void log_bool(const bool boolean) {
	printf("%d\n", boolean);
}

static void log_int(const int x) {
	printf("%d\n", x);
}

static ht_pair* ht_new_pair(const char* k, const char* v) {
	ht_pair* pair = malloc(sizeof(ht_pair));
	pair->key = strdup(k);
	pair->value = strdup(v);

	return pair;
}

static int ht_hash(const char* str, const int a, const int m) {
	long hash = 0;
	int string_length = strlen(str);
	int i = 0;
	for(i; i < string_length; i++) {
		hash += (long)pow(a, string_length - ( i + 1)) * str[i];
		hash = hash % m;
	}

	return (int)hash;
}

void dll_print(dll* list) {
	Node* node = list->head;
	do {
		if(node != NULL) {
			printf("Key: %s, Value: %s\n", node->pair->key, node->pair->value);
			node = node->next;
		}
	} while(node != NULL);
}

dll* dll_new() {
	dll* new_dll = malloc(sizeof(dll));
	new_dll->head = NULL;
	new_dll->tail = NULL;
}

Node* dll_search(dll* list, const char* key) {
	if(list == NULL || list == &DL_EMPTY_LIST)
		return NULL;
	Node* currentNode = list->head;
	while(currentNode != NULL) {
		if(strcmp(currentNode->pair->key, key) == 0) {
			return currentNode;
		}
		currentNode = currentNode->next;
	}
	return NULL;
}

void dll_insert(dll* list, const char* key, const char* value) {
	ht_pair* new_pair = ht_new_pair(key, value);
	
	Node* new_node = malloc(sizeof(Node));
	new_node->pair = new_pair;
	new_node->next = NULL;
	
	if(list->head == NULL) {
		new_node->previous = NULL;
		list->head = new_node;
		list->tail = new_node;
		list->size += 1;
	} else {
		Node* foundNode = dll_search(list, key);
		if(foundNode != NULL) {
			foundNode->pair = new_pair;
		} else {
			list->tail->next = new_node;
			new_node->previous = list->tail;
			list->tail = new_node;
			list->size += 1;
		}
	}
}

ht_hash_table* ht_new_sized(const int base_size) {
	ht_hash_table* hash_table = malloc(sizeof(ht_hash_table));

	hash_table->base_size = base_size;
	hash_table->size = next_prime(hash_table->base_size);

	hash_table->count = 0;
	hash_table->list = calloc((size_t)hash_table->size, sizeof(dll*));

	return hash_table;
}

ht_hash_table* ht_new(const int size) {
	return ht_new_sized(size);
}

static void ht_delete_pair(ht_pair* pair) {
	if(pair == NULL || pair == &HT_DELETED_PAIR) {
		return;
	}
	if(pair->key != NULL) {
		free(pair->key);
	}
	if(pair->value != NULL) {
		free(pair->value);
	}
	free(pair);
}

static void dll_delete_node(Node* node) {
	if(node->previous != NULL) {
		node->previous->next = node->next;
	}
	if(node->next != NULL) {
		node->next->previous = node->previous;
	}
	free(node);
}

void dll_delete_by_key(dll *list, const char* key) {
	Node* node = list->head;
	Node* next_node;

	while(node != NULL) {
		next_node = node->next;
		if(strcmp(node->pair->key, key) == 0) {
			dll_delete_node(node);
			list->size -= 1;
			return;
		}
		node = next_node;
	}
}

void dll_delete_list(dll *list) {
	Node* node = list->head;
	Node* next_node;

	while(node != NULL) {
		next_node = node->next;
		dll_delete_node(node);
		list->size -= 1;
		node = next_node;
		if(node != NULL)
			node->previous = NULL;
	}

	list->head = NULL;
	list->tail = NULL;
	free(list);
}

void ht_delete_hash_table(ht_hash_table* hash_table) {
	int i;
	for (i = 0; i < hash_table->size; i++) {
		dll* list =  hash_table->list[i];
		if(list != NULL) {
			dll_delete_list(list);
		}
	}
	free(hash_table->list);
	free(hash_table);
}

// static void ht_resize(ht_hash_table* hash_table, const int base_size) {
// 	if(base_size < HT_INTIAL_BASE_SIZE) {
// 		return;
// 	}

// 	ht_hash_table* new_hash_table = ht_new_sized(base_size);
// 	int i = 0;
// 	for(i; i< hash_table->size; i++) {
// 		ht_pair* pair = hash_table->pairs[i];
// 		if(pair != NULL && pair != &HT_DELETED_PAIR) {
// 			ht_insert(new_hash_table, pair->key, pair->value);
// 		}
// 	}

// 	hash_table->base_size = new_hash_table->base_size;
// 	hash_table->count = new_hash_table->count;

// 	const int tmp_size = hash_table->size;
// 	hash_table->size = new_hash_table->size;
// 	new_hash_table->size = tmp_size;

// 	ht_pair** tmp_pairs = hash_table->pairs;
// 	hash_table->pairs = new_hash_table->pairs;
// 	new_hash_table->pairs = tmp_pairs;

// 	ht_delete_hash_table(new_hash_table);
// }

// static void ht_resize_up(ht_hash_table* hash_table) {
// 	ht_resize(hash_table, hash_table->base_size * 2);
// }

// static void ht_resize_down(ht_hash_table* hash_table) {
// 	ht_resize(hash_table, hash_table->base_size / 2);
// }

static int ht_get_hash(const char* str, const int bucket_size, const int attempts) {
	const int hash_a = ht_hash(str, HT_PRIME_1, bucket_size);
	const int hash_b = ht_hash(str, HT_PRIME_2, bucket_size);
	return ( hash_a + attempts * ( hash_b + 1 ) ) % bucket_size;
}

void ht_insert(ht_hash_table* hash_table, const char* key, const char* value) {

	if(hash_table->count == hash_table->size) {
		return;
	}
	// const int load = hash_table->count * 100 / hash_table->size;
	// if(load > 70) {
	// 	ht_resize_up(hash_table);
	// }

	// Initially, get a new index for the newly created pair
	int index = ht_get_hash(key, hash_table->size, 0);
	// Get the list at position index;
	dll* list = hash_table->list[index];
	if(list == NULL) {
		list = dll_new();
	}
	dll_insert(list, key, value);
	hash_table->list[index] = list;
	hash_table->count += 1;
}

int ht_size(ht_hash_table* hash_table) {
	return hash_table->count;
}

char* ht_search(ht_hash_table* hash_table, const char* key) {

	// Get the initial index
	int index = ht_get_hash(key, hash_table->size, 0);
	dll* list = hash_table->list[index];
	if(list == NULL) {
		return NULL;
	}
	Node* foundNode = dll_search(list, key);
	if(foundNode != NULL) {
		return foundNode->pair->value;
	}
	return NULL;
}

void ht_delete(ht_hash_table* hash_table, const char* key) {

	// const int load = hash_table->count * 100 / hash_table->size;
	// if(load < 10) {
	// 	ht_resize_down(hash_table);
	// }

	// Get the initial index
	int index = ht_get_hash(key, hash_table->size, 0);
	dll* list = hash_table->list[index];
	if(list == NULL) {
		return;
	}
	dll_delete_by_key(list, key);
	if(list->size == 0) {
		list = &DL_EMPTY_LIST;
		hash_table->list[index] = list;
	}
}
