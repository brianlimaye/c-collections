
struct HashElement
{
	char * key;
	struct HashElement * next;
	int hashcode;
	char * value;
};

struct HashMap
{
	int capacity;
	int currentSize;
	struct HashElement ** contents;
};

struct HashMap * HashMap_init(int capacity);

void HashMap_destroy(struct HashMap * hm);

char * HashMap_put(char * key, char * val, struct HashMap * hm);

void HashMap_Iterator(struct HashMap * hm, void (*ptr)(char *, char *));

char * HashMap_get(char * key, struct HashMap * hm);

int HashMap_size(struct HashMap * hm);

