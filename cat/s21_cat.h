#ifndef S21_CAT_H
#define S21_CAT_H
#include <stdio.h>

typedef struct flags_struct {  // с большой буквы, использовать upper camel case
  int num_non_blank;           // b
  int end_dollar;              // e
  int number;                  // n
  int squeeze_blank;           // s
  int show_tab;                // t
  int v;                       // v
} flags_struct;

void print_lines(char *local_file, int *new_line, int *real_empty,
                 int *lines_counter, flags_struct *flags);
void handle_file(int *new_line, int *real_empty, int *lines_counter,
                 flags_struct *flags, FILE *fptr);
int files_counter(int argc, char **argv, char **files_list);
#endif
