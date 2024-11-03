#include "wordlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initarr(WordArray *arr, int words){
    return; // make later ig idk if this is necessary
}

void initlist(WordLinkedList *list) {
    list->next = NULL;
    list->curr = NULL;
}

void addword(WordLinkedList *list, char *word) {
    Word *new = malloc(sizeof(Word));
    if (!new) {
        exit(EXIT_FAILURE);
    }
    strncpy(new->word, word, sizeof(new->word) - 1);
    new->word[sizeof(new->word) - 1] = '\0';
    new->next = NULL;

    if (!list->next) {
        list->next = new;
    } else {
        Word *b;
        for (b = list->next; b->next; b = b->next);
        b->next = new;
    }
}

void rmfirst(WordLinkedList *list) {
    if (!list->next) {
        return;    
    }
    Word *temp = list->next;
    list->next = list->next->next;
    free(temp);
}

void rmall(WordLinkedList *list) {
    Word *prev, *p = list->next;
    while (p) {
        prev = p;
        p = p->next;
        free(prev);
    }
    list->next = NULL;
}

void lswords(WordLinkedList *list) {
    Word *b = list->next;
    while (b) {
        printf("%s\n", b->word);
        b = b->next;
    }
}

void cleanup(WordLinkedList *list) {
    rmall(list);

	return;
}

char *nextword(WordLinkedList *list) {
    if (!list->curr) {
        return NULL;
    }
    char *word = list->curr->word; 
    list->curr = list->curr->next; 
    return word;
}


void resetcurr(WordLinkedList *list){
	if(!list->next){
		list->curr = NULL;
		return;
	}

	list->curr = list->next;
	return;
	

}