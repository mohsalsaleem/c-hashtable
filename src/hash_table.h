typedef struct {
	char* key;
	char* value;
} ht_pair;

typedef struct {
	int size;
	int count;
	ht_pair** pairs;
} ht_hash_table;