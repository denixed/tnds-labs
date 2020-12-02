C = gcc
LINTER = CodeChecker

TARGET = bin/app.out
LIBRIES = 
CFLAGS = -Wall -std=gnu11 -O0
HDRS_DIR = ./inc

SRCS = src/array_stack.c src/list_stack.c src/utils.c src/main.c

.PHONY: clean

app.out: $(SRCS)
	$(CC) -o $(TARGET) $(CFLAGS) $(SRCS) $(LIBRIES) -I $(HDRS_DIR) -g -ggdb $(CFLAGS)

clean:
	rm -rf $(TARGET) *.gcov *.gcda *.gcno *.exe