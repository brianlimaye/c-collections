#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

//#include "Dict.h"
#include "HashMap.h"
#include "ArrayList.h"
#include "StringBuilder.h"

struct HashMap * dict_load()
{
	struct HashMap * hm = HashMap_init(100);
	char * tmp;

	FILE * f = fopen("/usr/share/dict/words", "r");

	if(f == NULL)
	{
		fprintf(stderr, "CANNOT LOAD FILE");
		return NULL;
	}
	char * str = (char *) malloc(46 * sizeof(char));

	if(str == NULL)
	{
		fprintf(stderr, "CANNOT ALLOCATE ENOUGH MEMORY.");
		return NULL;
	}

	char * line = NULL;
	size_t len = 0;
	char * copy;

	while(getline(&line, &len, f) != -1)
	{
		
		copy = strdup(line);
		//int line_len = strlen(line);
		//char s[line_len];
		//strncpy(s, line, line_len);
		//s[line_len - 1] = '\0';

		HashMap_put(copy, copy, hm);
		
	}
	//free(line);
	fclose(f);
	return hm;
}

struct ArrayList * getSimilarities(char * word, char * word1, struct HashMap * hm)
{
	
	int len = strlen(word);
	struct ArrayList * mutations = ArrayList_init(5, len);	
	int i;
	char c;
	char buffer[len + 1];
	sprintf(buffer, "%s", word);
	buffer[len] = '\0';
	char * cpy = word1;


	while(strcmp(buffer, word1) != 0)
	{
		for(i = 0; i< len; i++)
		{
			c = buffer[i];
			if(c != *cpy)
			{
				buffer[i] = *cpy;
				if(HashMap_get(buffer, hm) == NULL)
				{
					buffer[i] = c;
				}
				else
				{
					ArrayList_add(buffer, mutations);
				}
				cpy++;
			}
			printf("Buffer is: %s\n", buffer);
		}
		cpy -= len;
	}

	return mutations;
}

int main()
{
	struct HashMap * hm = dict_load();
	printf("hm size is: %d\n", hm->currentSize);
	//struct ArrayList * a = getSimilarities("cat", "dog", hm);
}
