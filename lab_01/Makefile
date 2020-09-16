CC = gcc
LINTER = CodeChecker

TARGET = bin/main.out
LIBRIES = 


SRCS = $(shell find  -name '*.c')

.PHONY: all clean test linter

all: $(SRCS)
	$(CC) -Wall -Wextra -Werror -std=c99 -pedantic -Wvla -o $(TARGET) $(CFLAGS) $(SRCS) $(LIBRIES)

clean:
	rm -rf $(TARGET) *.gcov *.gcda *.gcno

test:
	$(CC) -Wall -Wextra -Werror -O0 -Wvla -std=c99 -pedantic -fprofile-arcs -ftest-coverage -o $(TARGET) $(CFLAGS) $(SRCS) $(LIBRIES)
	bash ./func_tests/test.sh
	$(foreach s,$(SRCS), gcov $s;)

linter:
	$(foreach s,$(SRCS), $(LINTER) $s;)
