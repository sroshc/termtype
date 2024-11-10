#include "worddblib.h"
#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

static int callback(void *NotUesd, int argc, char **argv, char **azColName){
       int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

static int get_word(void *result, int argc, char **argv, char **azColName){
    if(argc >0){
        strncpy((char *)result, argv[0], 255);
        ((char *)result)[255] = '\0';
    } 

    return 0;
}

sqlite3 *create_database(char* name){
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char *sql;

    rc = sqlite3_open(name, &db);

    if(rc){
        fprintf(stderr, "Can't open database %s\n", sqlite3_errmsg(db));
        return NULL;
                
    }

    sql = "CREATE TABLE IF NOT EXISTS WORDS("  \
      "ID INTEGER PRIMARY KEY     AUTOINCREMENT," \
      "WORD           TEXT    NOT NULL," \
      "LEN            INT     NOT NULL);";
    
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if(rc != SQLITE_OK){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return NULL;
    }

    sql = "CREATE INDEX IF NOT EXISTS idx_words_id on WORDS(ID);";

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error while creating index: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return NULL;
    }

    sqlite3_free(zErrMsg);
    return db;
}


int insert_word(sqlite3 *db, char *word){
    char *zErrMsg = 0;
    int rc;
    char *sql;

    asprintf(&sql, "INSERT INTO WORDS(WORD, LEN) VALUES ('%s', %lu);", word, strlen(word));
    
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if(rc != SQLITE_OK){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        free(sql);
        return 1;
    }

    free(sql);
    sqlite3_free(zErrMsg);
    return 0;


}

int print_all(sqlite3 *db){
    char *zErrMsg = 0;
    int rc;
    char *sql;
    const char* data = "Callback function called";

    sql = "SELECT * from WORDS";

    rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);

    if(rc != SQLITE_OK){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return 1;
    }


    return 0;

}

int select_nth_word(sqlite3 *db, int n, char *result){
    char *zErrMsg = 0;
    int rc;
    char *sql;

    asprintf(&sql, "SELECT WORD FROM WORDS WHERE ID = %d;", n);

    rc = sqlite3_exec(db, sql, get_word, (void *)result, &zErrMsg);

    if(rc != SQLITE_OK){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        free(sql);
        return 1;
    }
    
    free(sql);
    sqlite3_free(zErrMsg);
    return 0;
}

int get_words_from_json(sqlite3 *db, char *json, char *output){
    FILE *output_file;
    FILE *json_file = fopen(json, "r");

    if(output){
        output_file = fopen("output.txt", "w");
   }
    if(!json_file){
        perror("Failed to open JSON file");
        return 1;
    }
    

    char line[256];
    int in_words_section = 0;

    while(fgets(line, sizeof(line), json_file)){
        if(strstr(line, "\"words\": [")){
            in_words_section = 1;
            fgets(line, sizeof(line), json_file);
        }

        if(in_words_section){
            if(strchr(line, ']')){
                char *end = strchr(line, ']');
                *end = '\0';
            }

            char *token = strtok(line,",");

            while(token){
                while(*token == ' ' || *token == '\"' || *token == '\n'|| *token == '}') token ++;
                char *ptr = token + strlen(token) - 1;
                while(ptr > token && (*ptr == ' '|| *ptr == '\"' || *ptr == '\n')){
                    *ptr = '\0';
                    ptr--;
                }
                if(*token && strcmp(token, "") != 0){
                    if(db){
                        insert_word(db, token);
                    }
                    if(output){
                        fprintf(output_file, strcat(token, "\n"));
                    }
                }
                token = strtok(NULL, ",");

            }

            if(strchr(line, ']')){
                break;
            }


        }
    }
    if(output){
        fclose(output_file);
    }
    fclose(json_file);

    return 0;

}


int get_n_random_words(sqlite3 *db, int range, int num, char ***res){
    srand((unsigned)getpid());

    char randword[256];
    *res = malloc((num + 1) * sizeof(char *));

    if(*res == NULL){
        return 1;
    }

    for(int i = 0; i < num; i++){
        int randindex = rand() % range + 1;

        if(select_nth_word(db, randindex, randword) == 1){
            free(*res);
            return 1;
        }

        (*res)[i] = malloc(strlen(randword) + 1);

        if((*res)[i] == NULL){
            free(*res);
            return 1;
        }

        strcpy((*res)[i], randword);



    }

    (*res)[num] = NULL;

    return 0;

        
}

int initwordlist(WordList** wordlist, int range, int num, char* filename){
    *wordlist = malloc(sizeof(WordList));

    if(*wordlist == NULL){
        return 1;
    }

    (*wordlist)->db = create_database(filename);

    if((*wordlist)->db == NULL){
        return 1;
    }

    (*wordlist)->word_list = NULL;
    if(get_n_random_words((*wordlist)->db, range, num, &(*wordlist)->word_list) != 0){
        free(*wordlist);
        return 1;
    }

    (*wordlist)->length = num;

    return 0;
}

int freewordlist(WordList* wordlist){
    if(wordlist == NULL) return 0;

    sqlite3_close(wordlist->db);


    if(wordlist->word_list){
        for(int i = 0; wordlist->word_list[i] != NULL; i++){
            free(wordlist->word_list[i]);
        } 
        free(wordlist->word_list);
    }


    free(wordlist);

    return 0;
}

int doesfileexist(char* filename){
    DIR *dir;
    struct dirent *entry;
    
    dir = opendir(".");
    if(dir == NULL){
        perror("opendir");
        return -1;
    }

    while((entry = readdir(dir)) != NULL){
        if(strcmp(filename, entry->d_name) == 0){
            closedir(dir);
            return 1;
        }
    }

    closedir(dir);
    return 0;

}

