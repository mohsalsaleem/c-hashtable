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

ht_hash_table* ht_new_sized(const int base_size) {
	ht_hash_table* hash_table = malloc(sizeof(ht_hash_table));

	hash_table->base_size = base_size;
	hash_table->size = next_prime(hash_table->base_size);

	hash_table->count = 0;
	hash_table->pairs = calloc((size_t)hash_table->size, sizeof(ht_pair*));

	return hash_table;
}

ht_hash_table* ht_new() {
	return ht_new_sized(HT_INTIAL_BASE_SIZE);
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

void ht_delete_hash_table(ht_hash_table* hash_table) {
	int i;
	for (i = 0; i < hash_table->size; i++) {
		ht_pair* pair =  hash_table->pairs[i];
		if(pair != NULL) {
			ht_delete_pair(pair);
		}
	}
	free(hash_table->pairs);
	free(hash_table);
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

static void ht_resize(ht_hash_table* hash_table, const int base_size) {
	if(base_size < HT_INTIAL_BASE_SIZE) {
		return;
	}

	ht_hash_table* new_hash_table = ht_new_sized(base_size);
	int i = 0;
	for(i; i< hash_table->size; i++) {
		ht_pair* pair = hash_table->pairs[i];
		if(pair != NULL && pair != &HT_DELETED_PAIR) {
			ht_insert(new_hash_table, pair->key, pair->value);
		}
	}

	hash_table->base_size = new_hash_table->base_size;
	hash_table->count = new_hash_table->count;

	const int tmp_size = hash_table->size;
	hash_table->size = new_hash_table->size;
	new_hash_table->size = tmp_size;

	ht_pair** tmp_pairs = hash_table->pairs;
	hash_table->pairs = new_hash_table->pairs;
	new_hash_table->pairs = tmp_pairs;

	ht_delete_hash_table(new_hash_table);
}

static void ht_resize_up(ht_hash_table* hash_table) {
	ht_resize(hash_table, hash_table->base_size * 2);
}

static void ht_resize_down(ht_hash_table* hash_table) {
	ht_resize(hash_table, hash_table->base_size / 2);
}

static int ht_get_hash(const char* str, const int bucket_size, const int attempts) {
	const int hash_a = ht_hash(str, HT_PRIME_1, bucket_size);
	const int hash_b = ht_hash(str, HT_PRIME_2, bucket_size);
	return ( hash_a + attempts * ( hash_b + 1 ) ) % bucket_size;
}

void ht_insert(ht_hash_table* hash_table, const char* key, const char* value) {

	const int load = hash_table->count * 100 / hash_table->size;
	if(load > 70) {
		ht_resize_up(hash_table);
	}

	ht_pair* new_pair = ht_new_pair(key, value);

	// Initially, get a new index for the newly created pair
	int index = ht_get_hash(new_pair->key, hash_table->size, 0);

	// Get the pair at position index;
	ht_pair* current_pair = hash_table->pairs[index];
	
	int i = 1;
	// Loop until we find an empty bucket.
	while(current_pair != NULL && current_pair != &HT_DELETED_PAIR) {
		// Incase the key already exists
		if(strcmp(current_pair->key, key) == 0) {
			break;
		}
		index = ht_get_hash(new_pair->key, hash_table->size, i);
		current_pair = hash_table->pairs[index];
		i++;
	}

	// Delete in case the bucket is already not empty for a specific key
	ht_delete_pair(current_pair);
	hash_table->pairs[index] = new_pair;
	hash_table->count += 1;
}

char* ht_search(ht_hash_table* hash_table, const char* key) {

	// Get the initial index
	int index = ht_get_hash(key, hash_table->size, 0);
	ht_pair* current_pair = hash_table->pairs[index];

	int i = 1;
	while(current_pair != NULL) {

		if(current_pair != &HT_DELETED_PAIR) {
			// Check if the pair's key and requested key match if yes, return value
			if(strcmp(current_pair->key, key) == 0) {
				return current_pair->value;
			}
		}
		// More attempts at finding the bucket
		index = ht_get_hash(key, hash_table->size, i);
		current_pair = hash_table->pairs[index];
		i++;
	}
	return NULL;
}

void ht_delete(ht_hash_table* hash_table, const char* key) {

	const int load = hash_table->count * 100 / hash_table->size;
	if(load < 10) {
		ht_resize_down(hash_table);
	}

	// Get the initial index
	int index = ht_get_hash(key, hash_table->size, 0);
	ht_pair* current_pair = hash_table->pairs[index];

	int i = 1;
	while(current_pair != NULL) {
		if(current_pair != &HT_DELETED_PAIR) {
			// Check if the pair's key and requested key match if yes, delete the pair and decrement the count
			if(strcmp(current_pair->key, key) == 0) {
				ht_delete_pair(current_pair);
				hash_table->pairs[index] = &HT_DELETED_PAIR;
				hash_table->count--;
			}
		}
		// More attempts at finding the bucket
		index = ht_get_hash(key, hash_table->size, i);
		current_pair = hash_table->pairs[index];
		i++;
	}
}
