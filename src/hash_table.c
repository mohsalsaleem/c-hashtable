#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

static ht_pair* ht_new_pair(const char* k, const char* v) {
	ht_pair* pair = malloc(sizeof(ht_pair));
	pair->key = strdup(k);
	pair->value = strdup(v);

	return pair;
}

ht_hash_table* ht_new() {
	ht_hash_table* hash_table = malloc(sizeof(ht_hash_table*));

	hash_table->size = 53;
	hash_table->count = 0;
	hash_table->pairs = calloc((size_t)hash_table->size, sizeof(ht_pair*));

	return hash_table;
}

static void ht_delete_pair(ht_pair* pair) {
	free(pair->key);
	free(pair->value);
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
		hash += (long)pow(a, string_length - ( i + 1)) * s[i];
		hash = hash % m;
	}

	return (int)hash;
}

static int ht_get_hash(const char* str, const int bucket_size, const int attempts) {
	const int hash_a = ht_hash(str, HT_PRIME_1, bucket_size);
	const int hash_b = ht_hash(str, HT_PRIME_2, bucket_size);
	return ( hash_a + i * ( hash_b + 1 ) ) % num_buckets;
}