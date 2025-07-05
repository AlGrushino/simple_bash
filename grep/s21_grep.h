#ifndef S21_GREP_H
#define S21_GREP_H
#include <regex.h>

typedef struct GrepFlags {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
} GrepFlags;

#define TEMPLATES_MAX 512
#define LINE_MAX 1024
#define FILES_MAX 32

void f_handler(char templates_list[TEMPLATES_MAX][LINE_MAX], char *file_name,
               int *templates_counter);
void filenames_writer(int optind, int argc, int *files_counter, char *argv[],
                      char files_list[FILES_MAX][LINE_MAX]);
void output_line(char *line, char *file_name, int lines_counter);
void c_handler(int match_counter, char *file_name);
void free_reg(int templates_counter, regex_t reg[TEMPLATES_MAX]);
void check_flags(int argc, char **argv, GrepFlags *flags,
                 int *templates_counter,
                 char templates_list[TEMPLATES_MAX][LINE_MAX]);
void files_hanlder(char files_list[FILES_MAX][LINE_MAX], int *templates_counter,
                   int *files_counter, GrepFlags *flags,
                   regex_t regexes[TEMPLATES_MAX]);

void o_handler(regex_t regexes[TEMPLATES_MAX], int templates_counter,
               char buff[LINE_MAX], char *file_name, int lines_counter);
#endif