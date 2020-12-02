C = gcc
LINTER = CodeChecker

TARGET = bin/app.out
LIBRIES = 
CFLAGS = -Wall -std=gnu11 -O0
HDRS_DIR = ./inc

SRCS = src/main.c src/utils.c src/matrix.c src/io_matrix.c src/random_matrix.c

.PHONY: clean

app.out: $(SRCS)
	$(CC) -o $(TARGET) $(CFLAGS) $(SRCS) $(LIBRIES) -I $(HDRS_DIR) -g -ggdb $(CFLAGS)

clean:
	rm -rf $(TARGET) *.gcov *.gcda *.gcno *.exe