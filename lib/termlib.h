/* termlib.h */
#ifndef TERMLIB_H
#define TERMLIB_H

enum editorKey {
  BACKSPACE = 127,
  ARROW_LEFT = 1000,
  ARROW_RIGHT,
  ARROW_UP,
  ARROW_DOWN,
  DEL_KEY,
  HOME_KEY,
  END_KEY,
  PAGE_UP,
  PAGE_DOWN
};

enum TextColor {
    RED = 31,
    GREEN = 32,
    YELLOW = 33,
    BLUE = 34,
    MAGENTA = 35,
    CYAN = 36,
    WHITE = 37
};

enum BackgroundColor {
    BACKGROUND_BLACK = 40,
    BACKGROUND_RED = 41,
    BACKGROUND_GREEN = 42,
    BACKGROUND_YELLOW = 43,
    BACKGROUND_BLUE = 44,
    BACKGROUND_MAGENTA = 45, 
    BACKGROUND_CYAN = 46,
    BACKGROUND_WHITE = 47,
};


void switch_alternate_screen();
void restore_screen();
void clear_screen();
int disable_canonical();
int restore_term_settings();
char get_char();
void die(const char *s);
int read_key();
void get_cursor_position(int *row, int *col);
void move_cursor(int x, int y);
void clear_line();
void set_text_color(int color);
void reset_color();
int get_term_width();
void s_print(char *buf);
void s_clear_line();
void enable_cursor();
void disable_cursor();


#endif