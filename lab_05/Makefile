C = gcc
LINTER = CodeChecker

TARGET = bin/app.out
LIBRIES = -lncurses
CFLAGS = -Wall -std=gnu11 -O0
HDRS_DIR = ./inc

SRCS = src/arr_queue.c src/list_queue.c src/queue.c src/utils.c src/handler.c src/main.c

.PHONY: clean

app.out: $(SRCS)
	$(CC) -o $(TARGET) $(CFLAGS) $(SRCS) -I $(HDRS_DIR) -g -ggdb $(CFLAGS) $(LIBRIES)

clean:
	rm -rf $(TARGET) *.gcov *.gcda *.gcno *.exe
