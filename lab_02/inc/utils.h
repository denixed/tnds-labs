#ifndef UTILS_H
#define UTILS_H

#include <inttypes.h>
#include "cars.h"


void print_start_menu();
int input_price_range(uint64_t *from, uint64_t *to);
int search(const cars_t *data);
int choice_menu(cars_t *data, keys_t *key_data, int *choice);
void evaluate(cars_t *unsorted_data, keys_t *unsorted_keys);


#endif // UTILS_H