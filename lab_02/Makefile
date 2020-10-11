C = gcc
LINTER = CodeChecker

TARGET = bin/app.out
LIBRIES = 
CFLAGS = -Wall -std=gnu11 -O0
HDRS_DIR = ./inc

SRCS = src/main.c src/car.c src/cars.c src/utils.c src/sorts.c

.PHONY: clean

app.out: $(SRCS)
	$(CC) -o $(TARGET) $(CFLAGS) $(SRCS) $(LIBRIES) -I $(HDRS_DIR) -O2

clean:
	rm -rf $(TARGET) *.gcov *.gcda *.gcno *.exe