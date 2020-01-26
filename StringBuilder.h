struct StringBuilder
{
	char * string;
};

struct StringBuilder * StringBuilder_init();

void StringBuilder_append(struct StringBuilder * sb, char * str);

void StringBuilder_clear(struct StringBuilder * sb);

char * StringBuilder_toString(struct StringBuilder * sb);

void StringBuilder_destroy(struct StringBuilder * sb);
