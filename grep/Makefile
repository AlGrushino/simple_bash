CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11

all: rebuild

s21_grep:
	$(CC) $(CFLAGS) s21_grep.c -o s21_grep

clean:
	rm -rf s21_grep

rebuild: clean s21_grep

format:
	clang-format -n s21_grep.c

test:
	sh test.sh

test_memory:
	sh test_memory.sh