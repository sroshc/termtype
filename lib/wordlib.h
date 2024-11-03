/* wordlib.h */
#ifndef WORDLIB_H
#define WORDLIB_H

enum levels{
	english_100,
	english_1000,
	english_10000
};

/* Structures */
struct s_word {
	char word[21];
	struct s_word *next;
};
typedef struct s_word Word;


struct s_word_list{
	Word *next;
	Word *curr;
};
typedef struct s_word_list WordLinkedList;


struct array_word{
	char* word;
	unsigned short int wordlen;
};
typedef struct array_word array_word;

struct word_array{
	array_word *words;
	int numwords;
	int level;
}; 
typedef struct word_array WordArray;

/* Functions*/
void initarr(WordArray *arr, int words);

void initlist(WordLinkedList *list);

void addword(WordLinkedList *list, char *word);

void rmfirst(WordLinkedList *list);

void rmall(WordLinkedList *list);

void lswords(WordLinkedList *list);

void cleanup(WordLinkedList *list);



#endif