/* worddblib.h */
#ifndef WORDDBLIB_H
#define WORDDBLIB_H
#include <sqlite3.h>

typedef struct wordlist{
    sqlite3 *db;
    char **word_list;
    int length;
} WordList;


/* No reason to define these in header */
//static int callback(void *NotUesd, int argc, char **argv, char **azColName);

//static int get_word(void *result, int argc, char **argv, char **azColName);     

sqlite3 *create_database(char* name);

int insert_word(sqlite3 *db, char *word);

int print_all(sqlite3 *db);

int select_nth_word(sqlite3 *db, int n, char *result);

int get_words_from_json(sqlite3 *db, char *json, char *output);

int get_n_random_words(sqlite3 *db, int range, int num, char ***res);

int initwordlist(WordList** wordlist, int range, int num, char* filename);

int freewordlist(WordList* wordlist);

int doesfileexist(char* filename);

#endif