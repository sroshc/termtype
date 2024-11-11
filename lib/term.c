#include "termlib.h"
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/ioctl.h>


void switch_alternate_screen() {
	printf("\e[?1049h");
	fflush(stdout);

}

void restore_screen() {
  printf("\e[?1049l");
	fflush(stdout);

}

void clear_screen(){
	printf("\033[H\033[J");
	fflush(stdout);
}

struct termios orig_conf;
struct termios game_conf;
int is_old_conf_stored = 0;

int disable_canonical(){
  if(is_old_conf_stored){
    return -1; // Meaning canonical has already been disblaed
  }

  tcgetattr(STDIN_FILENO, &orig_conf);
  game_conf = orig_conf;

  game_conf.c_lflag &= ~(ICANON | ECHO);

  tcsetattr(STDIN_FILENO, TCSANOW, &game_conf);


  game_conf.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON); 
  game_conf.c_oflag &= ~(OPOST);
  game_conf.c_cflag |= (CS8);
  game_conf.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  game_conf.c_cc[VMIN] = 0;
  game_conf.c_cc[VTIME] = 1;

  is_old_conf_stored = 1;

  return 0;
  

}

int restore_term_settings(){
  if(!is_old_conf_stored){
    return -1; // Meaning the original was never stored
  }

  tcsetattr(STDIN_FILENO, TCSANOW, &orig_conf);

  return 0;

}

char get_char() {
    struct termios oldt, newt; //Stores the terminal settings
    char ch;

    tcgetattr(STDIN_FILENO, &oldt); //Retrives the current term settings for sdin and stores them in oldt
    newt = oldt;

    newt.c_lflag &= ~(ICANON | ECHO); //Disables canonical mode (immedietly gives char), and disables echo (characters aren't displayed)
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); //Sets the new terminal settings

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); //Resets the old terminal settings

    return ch;
}

void die(const char *s) {
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);

  perror(s);
  exit(1);
}


int read_key() {  //use this one this is much better, copied from kilo lol
  int nread;
  char c;
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN) die("read");
  }

  if (c == '\x1b') {
    char seq[3];

    if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
    if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';

    if (seq[0] == '[') {
      if (seq[1] >= '0' && seq[1] <= '9') {
        if (read(STDIN_FILENO, &seq[2], 1) != 1) return '\x1b';
        if (seq[2] == '~') {
          switch (seq[1]) {
            case '1': return HOME_KEY;
            case '3': return DEL_KEY;
            case '4': return END_KEY;
            case '5': return PAGE_UP;
            case '6': return PAGE_DOWN;
            case '7': return HOME_KEY;
            case '8': return END_KEY;
          }
        }
      } else {
        switch (seq[1]) {
          case 'A': return ARROW_UP;
          case 'B': return ARROW_DOWN;
          case 'C': return ARROW_RIGHT;
          case 'D': return ARROW_LEFT;
          case 'H': return HOME_KEY;
          case 'F': return END_KEY;
        }
      }
    } else if (seq[0] == 'O') {
      switch (seq[1]) {
        case 'H': return HOME_KEY;
        case 'F': return END_KEY;
      }
    }

    return '\x1b';
  } else {
    return c;
  }
}

void get_cursor_position(int *row, int *col) {
    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    printf("\033[6n");
    fflush(stdout);

    char buf[32];
    int i = 0;
    while (1) {
        if (read(STDIN_FILENO, buf + i, 1) != 1) break;
        if (buf[i] == 'R') break; 
        i++;
    }
    buf[i] = '\0';

    if (buf[0] == '\033' && buf[1] == '[') {
        sscanf(buf + 2, "%d;%d", row, col);
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

int get_term_width(){
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  return w.ws_col;

}

void move_cursor(int x, int y){
	printf("\033[%d;%dH", x, y);
	fflush(stdout);
}

void clear_line(){
    printf("\033[2K\r");
}

void set_text_color(int color){ //sets it to specified color 
    char buf[30];
    snprintf(buf, sizeof(buf), "\033[%dm", color);

    write(STDOUT_FILENO, buf, strlen(buf));

}

void reset_color(){ //Resets color to terminal default
    printf("\033[0m");
}


void s_print(char *buf){
    write(STDOUT_FILENO, buf, strlen(buf));
    return;
}

void disable_cursor(){
  s_print("\33[?25l");
}

void enable_cursor(){
  s_print("\33[?25h");
}

void s_clear_line(){
  int x, y;

  get_cursor_position(&x, &y);

  for(int i = 0; i < get_term_width(); i ++){
    s_print(" ");
  }

  move_cursor(x, y);
  return;
}