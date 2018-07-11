#include "hash_table.h"

int main() {
	ht_hash_table* hash_table = ht_new();
	ht_delete_hash_table(hash_table);
	return 0;
}