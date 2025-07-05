#include "s21_cat.h"

#include <stdio.h>

#include "getopt.h"

int main(int argc, char **argv) {
  int opt;
  int new_line = 1;
  int real_empty = 0;
  int lines_counter = 0;
  char *files_list[1024] = {0};
  flags_struct flags = {0};
  const struct option long_options[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {NULL, 0, NULL, 0}};

  while ((opt = getopt_long(argc, argv, "benstET", long_options, NULL)) != -1) {
    switch (opt) {
      case 'b':
        flags.num_non_blank = 1;
        break;
      case 'n':
        flags.number = 1;
        break;
      case 's':
        flags.squeeze_blank = 1;
        break;
      case 'e':
        flags.end_dollar = 1;
        flags.v = 1;
        break;
      case 'E':
        flags.end_dollar = 1;
        break;
      case 't':
        flags.show_tab = 1;
        flags.v = 1;
        break;
      case 'T':
        flags.show_tab = 1;
        break;
      default:
        break;
    }
  }
  int files_amount = files_counter(argc, argv, files_list);

  for (int i = 0; i < files_amount; i++) {
    print_lines(files_list[i], &new_line, &real_empty, &lines_counter, &flags);
  }
  return 0;
}

void handle_file(int *new_line, int *real_empty, int *lines_counter,
                 flags_struct *flags, FILE *fptr) {
  int c;

  while ((c = fgetc(fptr)) != EOF) {
    int print_line = 1;
    int line_is_empty = (*new_line) && (c == '\n');
    if (flags->squeeze_blank && line_is_empty) {
      if (!*real_empty) {
        *real_empty = 1;
      } else {
        print_line = 0;
      }
    } else {
      *real_empty = 0;
    }
    if (*new_line) {
      if (flags->num_non_blank) {
        if (!line_is_empty) {
          printf("%6d\t", ++(*lines_counter));
        }
      } else if (flags->number) {
        printf("%6d\t", ++(*lines_counter));
      }
      *new_line = 0;
    }
    if (c == '\n') {
      *new_line = 1;
      if (flags->end_dollar && print_line) {
        putchar('$');
      }
    }
    if (c == '\t' && flags->show_tab) {
      printf("^I");
      print_line = 0;
    }
    if (flags->v && print_line) {
      if (c < 32 && c != '\n' && c != '\t') {
        printf("^");
        c += 64;
      } else if (c == 127 || c == 255) {
        if (c == 255) printf("M-");
        printf("^?");
        print_line = 0;
      } else if (c >= 128 && c < 160) {
        printf("M-^");
        c -= 128;
      }
    }
    if (print_line) putchar(c);
  }
}

void print_lines(char *local_file, int *new_line, int *real_empty,
                 int *lines_counter, flags_struct *flags) {
  FILE *fptr = fopen(local_file, "r");
  if (fptr == NULL) {
    printf("s21_cat: some mistake has happen with file ----> %s <----\n",
           local_file);
  } else {
    handle_file(new_line, real_empty, lines_counter, flags, fptr);
    fclose(fptr);
  }
}

int files_counter(int argc, char **argv, char **files_list) {
  int files_amount = 0;

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] != '-') {
      files_list[files_amount++] = argv[i];
    }
  }
  return files_amount;
}
