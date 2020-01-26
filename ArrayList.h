struct ArrayList
{
	int currentsize;
	int elementsize;
	int capacity;
	char ** arr;
};

struct ArrayList * ArrayList_init(int capacity, int elementsize);

void ArrayList_destroy(struct ArrayList * a);

int ArrayList_size(struct ArrayList * a);

char * ArrayList_get(int pos, struct ArrayList * a);

int ArrayList_capacity(struct ArrayList * a);

int ArrayList_add(char * string, struct ArrayList * a);

int ArrayList_remove(int pos, struct ArrayList * a);

void ArrayList_clear(struct ArrayList * a);

int ArrayList_isEmpty(struct ArrayList * a);

int ArrayList_contains(char * str, struct ArrayList * a);
