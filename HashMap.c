#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_KEYSIZE 2000

int hashcode(char * str)
{
	int i;
	int v=0;

	if(strlen(str) > 0)
	{
		for(i=0; i< strlen(str); i++)
		{
			v = 31 * v + str[i];
		}
	}
	return v;
}

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

struct HashElement * createHashElement()
{
	struct HashElement * tmp = (struct HashElement *) malloc(sizeof(struct HashElement));
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
	struct HashElement * nextElement = hm;

	while(nextElement != NULL)
	{
		curr = nextElement;
		nextElement = curr->next;
	}
	return curr;
}

void cleancontents(struct HashMap * hm) 
{
	int i;
	for(i=0; i< hm->capacity; i++)
	{
		struct HashElement * curr = hm->contents[i];

		while(curr != NULL)
		{
			struct HashElement * prev = curr;
			curr = curr->next;
			free(prev);
		}
	}

	/*
	struct HashElement * element;
	struct HashElement * next = NULL;
	int i;

	if(contents == NULL)
	{
		return -1;
	}

	for(i=0; i< capacity; i++)
	{
		element = contents[i];

		while(element != NULL)
		{
			next = NULL;
			next = element->next;
			if(next != NULL)
			{
				free(next);
			}
			element = next;
		}
		free(contents[i]);
		//To-do: free key and value
	}
	free(contents);	
	return 0;
	*/
}


int resize(int size, struct HashMap * hm)
{
	if(size <= hm->capacity)
	{
		return -1;
	}

	int i;
	int count;
	int k;
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
				continue;
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

	hm->contents = (struct HashElement **) calloc(capacity, sizeof(struct Hashmap *));

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
	int i;
	struct HashElement * next = NULL;
	
	if(hm)
	{
		/*
		if(hm-> contents)
		
		{

			
			for(i=0; i< hm->capacity; i++)
			{
				while((next = hm->contents[i]->next) != NULL)
				{
					free(next);
				}
				free(hm->contents[i]);
			}
		free(hm->contents);
		}
		*/
		cleancontents(hm);
		free(hm);

	}
}

//To-Do: How do I determine if the positions are full...
//char * v2 = HashMap_put("031602", "JidtreeSis", m);
char * HashMap_put(char * key, char * val, struct HashMap * hm)
{
	if(key == NULL || val == NULL)
	{
		return NULL;
	}

	if(sizeof(key) > MAX_KEYSIZE)
	{
		fprintf(stderr, "KEY IS TOO LARGE, CANNOT ENTER INTO MAP");
		return NULL;
	}

	if(sizeof(val) > MAX_KEYSIZE)
	{
		fprintf(stderr, "VALUE IS TOO LARGE, CANNOT ENTER INTO MAP");
		return NULL;
	}

	int i = 0;
	int k;
	int count = 0;
	int hash = hashcode(key);
	int index = hash % hm->capacity;
	struct HashElement * tmp = hm->contents[index];
	struct HashElement * tmp1 = hm->contents[index];
	struct HashElement * curr;
	struct HashElement * nextElement;
	struct HashElement * setter;
	char * oldVal;




	if((tmp->next == NULL) && (tmp->key == NULL) && (tmp-> value == NULL) && (tmp->hashcode == 0))
	{
		tmp->key = key;
		tmp->value = val;
		tmp->hashcode = hash;
		return NULL;
	}

	do
	{
		
		curr = tmp1;
		nextElement = curr->next;
		tmp1 = tmp1->next;

		if(curr->hashcode == hash)
		{
			if(strcmp(curr->key, key) == 0)
			{
				oldVal = curr->value;
				curr->value = val;
				return oldVal;
			}
		}
		count++;

	}
	while(nextElement != NULL);

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

	int i;
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

int main()
{
	int runs = 0;
	int maxruns = 100;


		
		struct HashMap * m = HashMap_init(8);

		char * u = HashMap_put("789990", "Marcel", m);

		char * u1 = HashMap_put("77748", "Lucas", m);

		char * v = HashMap_put("031602", "Brian", m);

		assert(v == NULL);

		char * v1 = HashMap_put("040174", "Bahar", m);

		assert(v1 == NULL);

		char * v2 = HashMap_put("031602", "JidtreeSis", m);

		assert(strcmp(v2, "Brian") == 0);

		char * v3 = HashMap_put("031602", "Brian", m);

		assert(strcmp(v3, "JidtreeSis") == 0); 


		
		
		while(runs++ < maxruns)
		{
			struct HashMap * m1 = HashMap_init(10);

			char * str = "09890";

			char * j = HashMap_put(str, "Ronald", m1);
			assert(j == NULL);

			char * k = HashMap_put(str, "Brian", m1);
			assert(strcmp(k, "Ronald") == 0);

			char * l = HashMap_put(str, "Ronald", m1);

			assert(strcmp(l, "Brian") == 0);

			HashMap_destroy(m1);
		}
		
		
		char * a = HashMap_get("031602", m);
		printf("a is: %s\n", a);
		char * b = HashMap_get("123456", m);
		printf("b is %s\n", b);

		//resize(100, m);

		//assert(m->capacity == 100);
		char * c = HashMap_get("031602", m);
		assert(strcmp(c, "Brian") == 0);
		
		HashMap_destroy(m);

		

	}
