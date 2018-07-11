#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"

int main() {
	ht_hash_table* hash_table = (ht_hash_table*)ht_new(500);

	// Insert
	ht_insert(hash_table, "name", "saleem");
	ht_insert(hash_table, "phone", "+1234567890");
	ht_insert(hash_table, "address", "mars");

	// Search
	char* _string = (char*)ht_search(hash_table, "name");
	printf("%s\n", (_string != NULL) ? _string : "Not found" );

	ht_insert(hash_table, "name", "Mohamed Saleem");

	_string = (char*)ht_search(hash_table, "name");
	printf("%s\n", (_string != NULL) ? _string : "Not found" );

	_string = (char*)ht_search(hash_table, "phone");
	printf("%s\n", (_string != NULL) ? _string : "Not found" );

	_string = (char*)ht_search(hash_table, "address");
	printf("%s\n", (_string != NULL) ? _string : "Not found" );

	// Delete
	ht_delete(hash_table, "name");
	_string = (char*)ht_search(hash_table, "name");
	printf("%s\n", (_string != NULL) ? _string : "Not found" );

	ht_delete_hash_table(hash_table);
	return 0;
}