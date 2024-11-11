#include "lib/wordlib.h"
#include "lib/termlib.h"
#include "lib/worddblib.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define VERSION "0.0.1"

double calculate_wpm(time_t starttime, time_t curr_time, int chars_typed, int mistakes){
    double minutes_elapsed = difftime(curr_time, starttime)/60.0;
    double words = (chars_typed-mistakes)/5.0;

    return minutes_elapsed != 0 ? words/minutes_elapsed : 0;
}
void update_wpm(int location, double wpm){
    int xtemp, ytemp;
    get_cursor_position(&xtemp, &ytemp);
    move_cursor(location, 1);
    reset_color();
    s_clear_line();
    set_text_color(BACKGROUND_BLACK);
    set_text_color(GREEN);
    s_print("WPM: ");
    set_text_color(RED);
    char wpmbuf[32];
    snprintf(wpmbuf, sizeof(wpmbuf), "%.2f", wpm);
    s_print(wpmbuf);
    move_cursor(xtemp, ytemp);
    set_text_color(GREEN);
}

int main(int argc, char *argv[]){
    WordList* wlist = NULL;
    char* dbname = "english_words.db";
    int range = 100;
    int num = 10;
    int words_location = 4;
    int wpm_location = 2;

    if(argc >= 2){
        sscanf(argv[1], "%d", &num);    
    }
    if(argc >= 3){
        sscanf(argv[2], "%d", &range);    

    }

    ///restore_screen();
    switch_alternate_screen();
    disable_canonical();
    clear_screen();

    if(!doesfileexist(dbname)){
        printf("word database doesn't exist\n");
        restore_term_settings();
        return 0;
    }


    initwordlist(&wlist, range, num, dbname);

    char buf[512]; 

    set_text_color(BLUE);

    move_cursor(words_location, 1);

    for(int i = 0; wlist->word_list[i]; i++){
        //snprintf(buf, sizeof(buf), wlist->word_list[i]);
        //strcat(buf, " ");
        s_print(wlist->word_list[i]);
        s_print(" ");
    }


    move_cursor(1, 1);

    set_text_color(GREEN);
    set_text_color(BACKGROUND_BLACK);
    snprintf(buf, sizeof(buf), "english%d ", range);
    s_print(buf);

    set_text_color(BACKGROUND_BLACK);
    set_text_color(GREEN);

    char* curr_word = wlist->word_list[0];
    int icurr_word = 0;
    int icurr_char = 0;
    int mistakes = 0;

    int chars_typed = 0;

    time_t starttime;
    time_t endtime;

    move_cursor(words_location, 1);
    disable_cursor();

    bool first_iteration = true;
    for(char i = getchar(); i != BACKSPACE; i = getchar()){
        if(first_iteration){
            time(&starttime);
            first_iteration = false;
        }
        time(&endtime);

        double current_wpm = calculate_wpm(starttime, endtime, chars_typed, mistakes);

        update_wpm(wpm_location, current_wpm);


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


    restore_screen();
    restore_term_settings();

    printf("WPM: %.2f\n", calculate_wpm(starttime, endtime, chars_typed, mistakes));
    printf("Mistakes: %d\n", mistakes);

    freewordlist(wlist);
    enable_cursor();


    return 0;
    

}
