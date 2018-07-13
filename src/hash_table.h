typedef struct {
	char* key;
	char* value;
} ht_pair;

typedef struct Node {
	ht_pair* pair;
	struct Node* previous;
	struct Node* next;
} Node;

typedef struct {
	Node* head;
	Node* tail;
	int size;
} dll;

typedef struct {
	int base_size;
	int size;
	int count;
	dll** list;
} ht_hash_table;

void dll_insert(dll *list, const char* key, const char* value);

void ht_insert(ht_hash_table* hash_table, const char* key, const char* value);
char* ht_get(ht_hash_table* hash_table, const char* key);
void ht_delete(ht_hash_table* hash_table, const char* key);