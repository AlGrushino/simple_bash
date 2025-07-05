CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11

all: rebuild

s21_cat:
	$(CC) $(CFLAGS) s21_cat.c -o s21_cat

clean:
	rm -rf s21_cat

rebuild: clean s21_cat

format:
	clang-format -n s21_cat.c

test:
	sh test.sh
