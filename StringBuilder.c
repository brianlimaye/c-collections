#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct StringBuilder
{
	char * string;
};

struct StringBuilder * StringBuilder_init()
{
	struct StringBuilder * sb = (struct StringBuilder *) malloc(sizeof(struct StringBuilder));

	if(sb == NULL)
	{
		fprintf(stderr, "CANNOT ALLOCATE MEMORY");
		return NULL;
	}
	sb->string = (char *) malloc(sizeof(char));

	if(sb->string == NULL)
	{
		fprintf(stderr, "CANNOT ALLOCATE MEMORY");
		free(sb);
		return NULL;
	}
	return sb;
}

void StringBuilder_append(struct StringBuilder * sb, char * str)
{
	if((str == NULL) || (strcmp(str, "") == 0) || (sb == NULL))
	{
		return;
	}
	
	int prevLen = strlen(sb->string);
	int newLen = strlen(str) + prevLen + 1;
	
	sb->string = (char *) realloc(sb->string, newLen * sizeof(char));

	if(sb->string == NULL)
	{
		fprintf(stderr, "CANNOT ALLOCATE MEMORY.");
		exit(1);
	}

	strcat(sb->string, str);
}

void StringBuilder_clear(struct StringBuilder * sb)
{
	if(sb == NULL || strcmp(sb->string, "") == 0)
	{
		fprintf(stderr, "CANNOT CLEAR");
		return;
	}
	memset(sb->string, '\0' , strlen(sb->string));
}

char * StringBuilder_toString(struct StringBuilder * sb)
{
	if(sb == NULL)
	{
		return NULL;
	}
	return sb->string;
}

void StringBuilder_destroy(struct StringBuilder * sb)
{
	if(sb == NULL)
	{
		return;
	}

	free(sb->string);
	free(sb);
}

int main()
{
	struct StringBuilder * sb = StringBuilder_init();
	StringBuilder_append(sb, "John");
	StringBuilder_append(sb, "ny");
	StringBuilder_append(sb, " Bravo");
	assert(strcmp(StringBuilder_toString(sb), "Johnny Bravo") == 0);
	StringBuilder_clear(sb);
	assert(strcmp(StringBuilder_toString(sb), "") == 0);
	StringBuilder_append(sb, "Harry");
	StringBuilder_append(sb, " Potter");
	assert(strcmp(StringBuilder_toString(sb), "Harry Potter") == 0);
	StringBuilder_destroy(sb);

}
