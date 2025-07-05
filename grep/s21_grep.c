#include "s21_grep.h"

#include <stdio.h>
#include <string.h>

#include "getopt.h"

int main(int argc, char **argv) {
  int templates_counter, files_counter;
  templates_counter = files_counter = 0;
  char templates_list[TEMPLATES_MAX][LINE_MAX];
  char files_list[FILES_MAX][LINE_MAX];
  GrepFlags flags = {0};
  regex_t regexes[TEMPLATES_MAX];

  check_flags(argc, argv, &flags, &templates_counter, templates_list);
  filenames_writer(optind, argc, &files_counter, argv, files_list);

  for (int i = 0; i < templates_counter; i++) {
    regcomp(&regexes[i], templates_list[i], flags.i ? REG_ICASE : 0);
  }
  files_hanlder(files_list, &templates_counter, &files_counter, &flags,
                regexes);
  free_reg(templates_counter, regexes);
}

void output_line(char *line, char *file_name, int lines_counter) {
  char *n = strchr(line, '\n');
  if (file_name != NULL) printf("%s:", file_name);
  if (lines_counter > 0) printf("%d:", lines_counter);
  if (n != NULL) *n = '\0';
  printf("%s\n", line);
}

void f_handler(char templates_list[TEMPLATES_MAX][LINE_MAX], char *file_name,
               int *templates_counter) {
  char buff[LINE_MAX];
  FILE *fptr = fopen(file_name, "r");
  if (fptr != NULL) {
    while (fgets(buff, sizeof(buff), fptr) != 0) {
      char *n = strchr(buff, '\n');
      if (n != NULL) *n = '\0';
      strcpy(templates_list[(*templates_counter)++], buff);
    }
    fclose(fptr);
  }
}

void filenames_writer(int optind, int argc, int *files_counter, char *argv[],
                      char files_list[FILES_MAX][LINE_MAX]) {
  for (int i = optind; i < argc; i++) {
    if (argv[i][0] != '-') {
      strcpy(files_list[(*files_counter)++], argv[i]);
    }
  }
}

void c_handler(int match_counter, char *file_name) {
  if (file_name != NULL) printf("%s:", file_name);
  printf("%d\n", match_counter);
}

void free_reg(int templates_counter, regex_t reg[TEMPLATES_MAX]) {
  for (int i = 0; i < templates_counter; i++) {
    regfree(&reg[i]);
  }
}

void check_flags(int argc, char **argv, GrepFlags *flags,
                 int *templates_counter,
                 char templates_list[TEMPLATES_MAX][LINE_MAX]) {
  int opt;
  while ((opt = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, NULL)) != -1) {
    switch (opt) {
      case 'e':
        flags->e = 1;
        strcpy(templates_list[(*templates_counter)++], optarg);
        break;
      case 'i':
        flags->i = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'c':
        flags->c = 1;
        break;
      case 'l':
        flags->l = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 'h':
        flags->h = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 'f':
        flags->f = 1;
        f_handler(templates_list, optarg, templates_counter);
        break;
      case 'o':
        flags->o = 1;
        break;
      default:
        break;
    }
  }
  if (flags->c) flags->n = 0;
  if (flags->l) flags->c = 0;
  if (!flags->e && !flags->f) {
    strcpy(templates_list[(*templates_counter)++], argv[optind++]);
  }
}

void files_hanlder(char files_list[FILES_MAX][LINE_MAX], int *templates_counter,
                   int *files_counter, GrepFlags *flags,
                   regex_t regexes[TEMPLATES_MAX]) {
  for (int i = 0; i < *files_counter; i++) {
    int match_counter = 0;
    FILE *fptr = fopen(files_list[i], "r");
    if (fptr != NULL) {
      int lines_counter = 0;
      char buff[LINE_MAX];
      while (fgets(buff, LINE_MAX, fptr) != NULL) {
        int find = 0;
        lines_counter++;
        for (int j = 0; j < *templates_counter; j++) {
          int match = regexec(&regexes[j], buff, 0, NULL, 0);
          if (match == 0) find = 1;
        }
        if (flags->v) find = !find;
        match_counter += find;
        int need_to_print = !flags->c && !flags->l;
        if (find && need_to_print && flags->o)
          o_handler(regexes, *templates_counter, buff,
                    *files_counter > 1 && !flags->h ? files_list[i] : NULL,
                    flags->n ? lines_counter : 0);
        else if (find && need_to_print)
          output_line(buff,
                      *files_counter > 1 && !flags->h ? files_list[i] : NULL,
                      flags->n ? lines_counter : 0);
      }
      if (flags->c) {
        c_handler(match_counter,
                  *files_counter > 1 && !flags->h ? files_list[i] : NULL);
      }
      if (flags->l && match_counter > 0) {
        printf("%s\n", files_list[i]);
      }
      fclose(fptr);
    } else if (!flags->s) {
      printf("grep: %s: Нет такого файла или каталога\n", files_list[i]);
    }
  }
}

void o_handler(regex_t regexes[TEMPLATES_MAX], int templates_counter,
               char buff[LINE_MAX], char *file_name, int lines_counter) {
  int status = 0;
  while (status == 0) {
    int so = __INT_MAX__;
    int eo = 0;
    regmatch_t pmatch;
    for (int j = 0; j < templates_counter; j++) {
      int find = regexec(&regexes[j], buff, 1, &pmatch, 0);
      if (find == 0 &&
          (pmatch.rm_so < so ||
           (pmatch.rm_so == so && pmatch.rm_eo - pmatch.rm_so > eo - so))) {
        so = pmatch.rm_so;
        eo = pmatch.rm_eo;
      }
    }
    if (eo > 0) {
      char str[1024] = {0};
      strncpy(str, buff + so, eo - so);
      output_line(str, file_name, lines_counter);
      buff += eo;
    } else
      status = 1;
  }
}