#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>


struct ArrayList
{
	int currentsize;
	int elementsize;
	int capacity;
	char ** arr;
};

struct ArrayList* ArrayList_init(size_t capacity, size_t elementsize)
{
	struct ArrayList * a = (struct ArrayList *) calloc(1, sizeof(struct ArrayList) * capacity);
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

void ArrayList_destroy(struct ArrayList * a)
{
	if(a)
	{
		if(a-> arr)
		{
			free(a->arr);
		}
		free(a);
	}
}

int ArrayList_size(struct ArrayList * a)
{
	return a->currentsize;
}

char * ArrayList_get(int pos, struct ArrayList * a)
{
	if(a == NULL)
	{
		return NULL;
	}
	char * c = a->arr[pos];
	return c;
}

int ArrayList_capacity(struct ArrayList * a)
{
	return a->capacity;
}

char ** growArray(struct ArrayList * a)
{
	int i;
	int newCapacity = a->capacity * 2;
	char ** newArr = (char **) malloc(newCapacity * sizeof(char *));

	if(newArr == NULL)
	{
		return NULL;
	}

	for(i = 0; i< newCapacity; i++)
	{
		newArr[i] = (char *) malloc(a->elementsize * sizeof(char));

		if(i < a->capacity)
		{
			strcpy(newArr[i], a->arr[i]);
			free(a->arr[i]);
		}
	}
	free(a->arr);

	return newArr;
}

void ensure_capacity(struct ArrayList * a)
{
	int i;

	if(a == NULL)
	{
		return;
	}

	if(a->currentsize >= (a->capacity / 2))
	{
		char ** newArr = NULL;

		newArr = growArray(a);

		a->arr = newArr;
		a->capacity = a->capacity * 2;
	}
}

void ArrayList_add(char * string, struct ArrayList * a)
{
	if(a != NULL)
	{
    	ensure_capacity(a);
		strcpy(a->arr[a->currentsize], string);
		a->currentsize++;
	}
}

void ArrayList_remove(int pos, struct ArrayList * a)
{
	int i;
	char * tmp;

	if(pos < 0 || pos >= ArrayList_capacity(a))
	{
		return;
	}

	memset(a->arr[pos], 0, a->elementsize * sizeof(char));

	for(i= pos + 1; i < ArrayList_size(a); i++)
	{
		tmp = a->arr[i];
		strcpy(a->arr[i-1], tmp);
		memset(a->arr[i], 0, a->elementsize * sizeof(char));
	}

	a->currentsize--;
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
	int i;

	for(i=0; i< currentsize; i++)
	{
		memset(a->arr[i], 0, a->elementsize);
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
		printf("TMP1 is: %s\n", tmp1);
		if(strcmp(tmp1, str) == 0)
		{
			return 1;
		}
	}
	return 0;
}

int main()
{
	struct ArrayList * a = ArrayList_init(3, 10);
	printf("Capacity is: %d\n", a->capacity);



	ArrayList_add("Brian", a);
	ArrayList_add("Bahar", a);
	ArrayList_add("Ayanna", a);
	ArrayList_add("Sarah", a);
	ArrayList_add("Jidtree", a);
	ArrayList_add("Leia", a);
	ArrayList_add("Levi", a);

	ArrayList_remove1(0, a);

	ArrayList_add("Brian", a);

	ArrayList_clear(a);

	assert(ArrayList_isEmpty);

	assert(ArrayList_size(a) == 0);

	ArrayList_add("Melinda", a);

	assert(ArrayList_size(a) == 1);

	assert(ArrayList_contains("Melinda", a));

	printf("a->arr[0] is: %s\n", a->arr[0]);

	ArrayList_destroy(a);

}
