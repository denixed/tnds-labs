C = gcc
LINTER = CodeChecker

TARGET = bin/app.out
LIBRIES = 
CFLAGS = -Wall -std=gnu11 -O0

SRCS = \
src/main.c \
src/io.c \
src/utils.c \
src/matrix.c \
src/graph.c

.PHONY: clean

app.out: $(SRCS)
	$(CC) -o $(TARGET) $(CFLAGS) $(SRCS) -g -ggdb $(CFLAGS) $(LIBRIES) -I.

clean:
	rm -rf $(TARGET) *.gcov *.gcda *.gcno *.exe
