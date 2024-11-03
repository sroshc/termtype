#include "lib/wordlib.h"
#include "lib/termlib.h"
#include "lib/worddblib.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

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

    set_text_color(BLUE);

    for(int i = 0; wlist->word_list[i]; i++){
        //snprintf(buf, sizeof(buf), wlist->word_list[i]);
        //strcat(buf, " ");
        s_print(wlist->word_list[i]);
        s_print(" ");
    }


    move_cursor(1, 1);
    set_text_color(BACKGROUND_BLACK);
    set_text_color(GREEN);

    char* curr_word = wlist->word_list[0];
    int icurr_word = 0;
    int icurr_char = 0;
    int mistakes = 0;

    int chars_typed = 0;

    time_t starttime;
    move_cursor(1, 1);
    
    bool first_iteration = true;
    for(char i = getchar(); i != BACKSPACE; i = getchar()){
        if(first_iteration){
            time(&starttime);
            first_iteration = false;
        }

        if(icurr_char >= strlen(curr_word) && i == ' '){
            chars_typed ++;
            s_print(" ");
            icurr_word++;
            if(icurr_word == num){
                break;
            }
            icurr_char = 0;
            curr_word = wlist->word_list[icurr_word];
             
        }
        else if(i != '\n' && i != ' ' && i == curr_word[icurr_char]){
            chars_typed ++;
            icurr_char++;
            snprintf(buf, sizeof(buf), "%c", i);
            s_print(buf);
        }
        else{
            mistakes++;
        }
    }

    time_t endtime;
    time(&endtime);




    

    restore_screen();
    restore_term_settings();

    double secelapsed = difftime(endtime, starttime);

    double words = (chars_typed - mistakes)/5.0;
    double minelapsed = secelapsed/60.0;

    double wpm = words/minelapsed;

    printf("WPM: %.2f\n", wpm);
    printf("Mistakes: %d\n", mistakes);

    freewordlist(wlist);


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