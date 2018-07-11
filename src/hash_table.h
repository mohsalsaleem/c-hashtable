typedef struct {
	char* key;
	char* value;
} ht_pair;

typedef struct {
	int base_size;
	int size;
	int count;
	ht_pair** pairs;
} ht_hash_table;

void ht_insert(ht_hash_table* hash_table, const char* key, const char* value);
char* ht_get(ht_hash_table* hash_table, const char* key);
void ht_delete(ht_hash_table* hash_table, const char* key);