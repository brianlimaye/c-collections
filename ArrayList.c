#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "ArrayList.h"

struct ArrayList* ArrayList_init(int capacity, int elementsize)
{
	struct ArrayList * a = (struct ArrayList *) malloc(sizeof(struct ArrayList));
	int i;
	
	if(a == NULL)
	{
		return NULL;
	}

	a->arr = (char **) malloc(capacity * sizeof(char *));

	for(i=0; i< capacity; i++)
	{
		a->arr[i] = (char *) malloc(elementsize * sizeof(char));
	}
	a->currentsize = 0;
	a->elementsize = elementsize;
	a->capacity = capacity;

	return a;
}

static void cleancontents(char ** contents, int capacity)
{
	int i;

	if(contents)
	{
		for(i=0; i< capacity; i++)
		{
			if(contents[i])
			{
				free(contents[i]);
			}
		}
		free(contents);
	}
}

void ArrayList_destroy(struct ArrayList * a)
{
	if(a)
	{
		cleancontents(a->arr, a->capacity);
		free(a);
	}
}

int ArrayList_size(struct ArrayList * a)
{
	return a->currentsize;
}



char * ArrayList_get(int pos, struct ArrayList * a)
{
	if(pos < 0 || pos > a->capacity || a == NULL)
	{
		return NULL;
	}

	return a->arr[pos];
}

int ArrayList_capacity(struct ArrayList * a)
{
	return a->capacity;
}

void growArray(struct ArrayList * a)
{
	int i;
	int newCapacity = a->capacity * 2;

	char ** newArr = (char **) malloc(newCapacity * sizeof(char *));

	if(newArr == NULL)
	{
		return;
	}

	for(i = 0; i< newCapacity; i++)
	{
		newArr[i] = (char *) malloc(a->elementsize * sizeof(char));

		if(i < a->currentsize)
		{
			strcpy(newArr[i], a->arr[i]);
		}
	}
	cleancontents(a->arr, a->capacity);
	
	a->capacity = newCapacity;
	a->arr = newArr;
}

int ensure_capacity(struct ArrayList * a)
{

	if (a == NULL)
	{
		return -1;
	}
	if(a->currentsize >= (a->capacity / 2))
	{
		growArray(a);
	}
	return 0;
}

int ArrayList_add(char * string, struct ArrayList * a)
{
	if(strlen(string) > a->elementsize)
	{
		return -1;
	}

	int result = ensure_capacity(a);

	if(result != -1)
	{
		strcpy(a->arr[a->currentsize], string);
		a->currentsize++;
		return 1;
	}
	return -1;
}

int ArrayList_remove(int pos, struct ArrayList * a)
{
	int i;
	char * tmp;

	if(pos < 0 || pos >= ArrayList_size(a))
	{
		return -1;
	}



	memset(a->arr[pos], 0, a->elementsize * sizeof(char));

	for(i= pos + 1; i < ArrayList_size(a); i++)
	{
		tmp = a->arr[i];
		strcpy(a->arr[i-1], tmp);
		memset(a->arr[i], 0, a->elementsize * sizeof(char));
	}

	a->currentsize--;
	return 1;
}

void ArrayList_remove1(int pos, struct ArrayList * a)
{

	int i;

	if(pos < 0 || pos >= ArrayList_capacity(a))
	{
		return;
	}

	if(pos == ArrayList_capacity(a) - 1)
	{
		free(a->arr[pos]);
		return;
	}
	
	char * tmp = a->arr[pos + 1];
	free(a->arr[pos]);

	for(i = pos + 1; i < ArrayList_size(a); i++) 
	{ 
		a->arr[i - 1] = tmp;
		tmp++;
	}
	a->currentsize--;

}

void ArrayList_clear(struct ArrayList * a)
{
	int currentsize = ArrayList_size(a);
	int elementsize = a->elementsize;

	int i;

	for(i=0; i< currentsize; i++)
	{
		//memset(a->arr[i], 0, elementsize);
		free(a->arr[i]);
		a->arr[i] = (char *) malloc(elementsize * sizeof(char));	
	}

	a->currentsize = 0;
}

int ArrayList_isEmpty(struct ArrayList * a)
{
	return a->currentsize == 0;
}

int ArrayList_contains(char * str, struct ArrayList * a)
{
	int i = 0;
	char ** tmp = a->arr;
	char * tmp1;

	while(i < a->currentsize)
	{
		tmp1 = tmp[i++];
		if(strcmp(tmp1, str) == 0)
		{
			return 1;
		}
	}
	return 0;
}
