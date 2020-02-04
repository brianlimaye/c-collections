//Last edited on 12-6-19
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>
#include "HashMap.h"

#define MAX_KEYSIZE 2000

int hashcode(char * str)
{
	int i;
	int v = 0;
	int len = strlen(str);

	if(len > 0)
	{
		for(i=0; i< len; i++)
		{
			v = 31 * v + str[i];
		}
	}
	return abs(v);
}

struct HashElement * createHashElement()
{
	struct HashElement * tmp = (struct HashElement *) malloc(sizeof(struct HashElement));

	if(tmp == NULL)
	{
		fprintf(stderr, "Memory is not able to be allocated..");
		exit(1);
	}
	return tmp;
}

struct HashElement * findLastElement(struct HashElement * hm)
{
	if(hm == NULL)
	{
		return NULL;
	}

	if(hm->next == NULL)
	{
		return hm;
	}

	struct HashElement * curr = hm;

	while(curr->next != NULL)
	{
		curr = curr->next;
	}
	return curr;
}

static void cleancontents(struct HashMap * hm) 
{
	int i;
	struct HashElement * prev;
	struct HashElement * curr;
	for(i=0; i< hm->capacity; i++)
	{
		curr = hm->contents[i];

		while(curr != NULL)
		{
			prev = curr;
			curr = curr->next;
			free(prev);
		}
		hm->contents[i] = NULL;
	}
	free(hm->contents);

}


int resize(int size, struct HashMap * hm)
{
	if(size <= hm->capacity)
	{
		return -1;
	}

	int i;
	int count;
	struct HashElement * curr;
	struct HashElement * newcurr;
	struct HashElement * lastElement;
	int index;
	struct HashElement ** tmp = (struct HashElement **) malloc(sizeof(struct HashElement *) * size);

	if(tmp == NULL)
	{
		return -2;
	}

	for(i=0; i< size; i++)
	{
		tmp[i] = createHashElement();
		tmp[i]->value = NULL;
		tmp[i]->key = NULL;
		tmp[i]->next = NULL;
		tmp[i]->hashcode = 0;
	}


	for(i=0; i< hm->capacity; i++)
	{
		count = 0;
		curr = hm->contents[i];

		do
		{
			index = curr->hashcode % size;
			newcurr = tmp[index];
			
			if(newcurr->hashcode == 0 && newcurr->next == NULL && newcurr->key == NULL && newcurr->value == NULL)
			{
				newcurr->hashcode = curr->hashcode;
				newcurr->key = curr->key;
				newcurr->value = curr->value;
				break;
			}
			
			lastElement = findLastElement(newcurr);

			lastElement->next = createHashElement();
			lastElement->next->hashcode = curr->hashcode;
			lastElement->next->value = curr->value;
			lastElement->key = curr->key;
			count++;
		}
		while((curr = curr->next) != NULL);	

	}
		cleancontents(hm);
		
		// set all vars
		hm->contents = tmp;
		hm->capacity = size;
		return 0;

}

struct HashMap * HashMap_init(int capacity)
{
	if(capacity < 1)
	{
		fprintf(stderr, "INVALID CAPACITY..\n");
		return NULL;
	}

	int i;
	struct HashMap * hm = (struct HashMap *) malloc(1 * sizeof(struct HashMap));
	hm->capacity = capacity;
	hm->currentSize = 0;

	hm->contents = (struct HashElement **) malloc(capacity * sizeof(struct Hashmap *));

	for(i=0; i< capacity; i++)
	{
		hm->contents[i] = createHashElement();
		hm->contents[i]->value = NULL;
		hm->contents[i]->key = NULL;
		hm->contents[i]->next = NULL;
		hm->contents[i]->hashcode = 0;
	}
	

	return hm;
}

void HashMap_destroy(struct HashMap * hm)
{	
	if(hm)
	{
		if(hm->contents)
		{
			cleancontents(hm);
		}
		free(hm);
	}
}

int HashMap_size(struct HashMap * hm)
{
	if(hm == NULL)
	{
		return -1;
	}
	return hm->currentSize;
}

//To-Do: How do I determine if the positions are full...
//char * v2 = HashMap_put("031602", "JidtreeSis", m);
char * HashMap_put(char * key, char * val, struct HashMap * hm)
{

	if(key == NULL || val == NULL)
	{
		return NULL;
	}

	if(strlen(key) > MAX_KEYSIZE)
	{
		fprintf(stderr, "KEY IS TOO LARGE, CANNOT ENTER INTO MAP");
		return NULL;
	}

	if(strlen(val) > MAX_KEYSIZE)
	{
		fprintf(stderr, "VALUE IS TOO LARGE, CANNOT ENTER INTO MAP");
		return NULL;
	}

	if(HashMap_size(hm) == hm->capacity - 1)
	{
		resize(hm->capacity * 2, hm);
	}

	int count = 0;
	int hash = hashcode(key);
	int index = hash % hm->capacity;
	struct HashElement * tmp = hm->contents[index];
	struct HashElement * tmp1 = hm->contents[index];
	struct HashElement * curr;
	char * oldVal;




	if((tmp->next == NULL) && (tmp->key == NULL) && (tmp-> value == NULL) && (tmp->hashcode == 0))
	{
		tmp->key = key;
		tmp->value = val;
		tmp->hashcode = hash;
		hm->currentSize++;
		return NULL;
	}

	do
	{
		
		curr = tmp1;
		tmp1 = tmp1->next;

		if(curr->hashcode == hash)
		{
			if(strcmp(curr->key, key) == 0)
			{
				oldVal = curr->value;
				curr->value = val;
				hm->currentSize++;
				return oldVal;
			}
		}
		count++;

	}
	while(curr->next != NULL);

	if(tmp1 == NULL)
	{
		return NULL;
	}

	tmp1->next = createHashElement();

	if(tmp1->next == NULL)
	{
		return NULL;
	}

	tmp1->value = val;
	tmp1->key = key;
	tmp1->hashcode = hash;

	hm->currentSize++;

	return NULL;

}

char * HashMap_get(char * key, struct HashMap * hm)
{
	if(key == NULL || hm == NULL)
	{
		return NULL;
	}

	if(sizeof(key) > MAX_KEYSIZE)
	{
		fprintf(stderr, "KEY IS TOO LARGE");
		return NULL;
	}

	struct HashElement * curr;
	int hash = hashcode(key);
	int index = hash % hm->capacity;

	curr = hm->contents[index];

	do
	{
		if(hash == curr->hashcode)
		{
			if(strcmp(curr->key, key) == 0)
			{
				return curr->value;
			}
		}
	}
	while((curr = curr->next) != NULL);

	return NULL;
}

void HashMap_Iterator(struct HashMap * hm, void (*ptr)(char *, char *))
{
			int i;
			for(i=0; i< hm->capacity; i++)
			{
				struct HashElement * he = hm->contents[i];
				while(he != NULL)
				{
					ptr(he->key, he->value);
					he = he->next;
				}
			}
}
