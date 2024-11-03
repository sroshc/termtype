#include "lib/wordlib.h"
#include "lib/termlib.h"
#include "lib/worddblib.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


void s_print(char *buf){
    write(STDOUT_FILENO, buf, strlen(buf));
    return;
}

int main(int argc, char *argv[]){
    WordList* wlist = NULL;
    char* dbname = "english_words.db";
    int range = 100;
    int num = 10;

    if(argc >= 2){
        sscanf(argv[1], "%d", &num);    
    }
    if(argc >= 3){
        sscanf(argv[2], "%d", &range);    

    }

    ///restore_screen();
    switch_alternate_screen();
    disable_canonical();
    move_cursor(1, 1);
    clear_screen();

    if(!doesfileexist(dbname)){
        printf("word database doesn't exist\n");
        restore_term_settings();
        return 0;
    }


    initwordlist(&wlist, range, num, dbname);

    char buf[256]; 

    for(int i = 0; wlist->word_list[i]; i++){
        //snprintf(buf, sizeof(buf), wlist->word_list[i]);
        //strcat(buf, " ");
        s_print(wlist->word_list[i]);
        s_print(" ");
    }


    move_cursor(1, 1);
    set_text_color(47);


    char* curr_word = wlist->word_list[0];
    move_cursor(10, 1);
    s_print(wlist->word_list[1]);
    int icurr_word = 0;
    int icurr_char = 0;
    int mistakes = 0;
    move_cursor(1, 1);
    
    for(char i = getchar(); i != BACKSPACE; i = getchar()){
        if(icurr_char >= strlen(curr_word) && i == ' '){
            s_print(" ");
            icurr_word++;
            if(icurr_word == num){
                break;
            }
            icurr_char = 0;
            curr_word = wlist->word_list[icurr_word];
             
        }
        else if(i != '\n' && i != ' ' && i == curr_word[icurr_char]){
            icurr_char++;
            snprintf(buf, sizeof(buf), "%c", i);
            s_print(buf);
        }
        else{
            mistakes++;
        }
    }


    freewordlist(wlist);
    

    restore_screen();
    restore_term_settings();

    printf("Mistakes: %d\n", mistakes);

    return 0;

}
/*
int main() {
    WordList list;
    initlist(&list);

    addword(&list, "hello");

    restore_screen();
    disable_canonical();
    switch_alternate_screen();
    clear_screen();

    int a = read_key();
    fflush(stdout);
    char buf[10];
    snprintf(buf, sizeof(buf), "%c\n", a);
 

    if(a == 127){
        s_print("Hello");
    }else{

        s_print(buf);

    
    }

    read_key();
    
    restore_term_settings();
    
    return 0; 
}
*/