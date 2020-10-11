#ifndef CARS_H
#define CARS_H

#include <stdio.h>
#include <inttypes.h>
#include "car.h"

#define MAX_NUMOF_CARS 10000

typedef struct 
{
    car_t cars[MAX_NUMOF_CARS];
    size_t amount;
} cars_t;

typedef struct 
{
    short_car_info_t data[MAX_NUMOF_CARS];
    size_t amount;
} keys_t;



void slow_key_sort(keys_t *a);
void fast_key_sort(keys_t *a, int first, int last);

void synchronize(const cars_t *src, keys_t *dst);

void print_by_key_table(cars_t *data, keys_t *keys);

/*Печать таблицы*/
void print_all(const cars_t *a);
void print_key_table(keys_t *keys);


void fsort(cars_t *a, int first, int last);
void ssort(cars_t *a);

void write_to_file(FILE *f, cars_t *a);
void write_line(FILE *f, car_t *car);

/*Добавить автомобиль*/
int add(cars_t *a);

/*Удалить автомобиль*/
void delete(cars_t *a, size_t index);

int delete_by_marque(cars_t *a);
int price_within_range(const car_t *a, uint64_t from, uint64_t to);

void searcher(const cars_t *data, car_t *target, uint64_t *p_from, uint64_t *p_to, cars_t *searched);
int read_line(FILE *f, car_t *car);
void read_from_file(FILE *f, cars_t *a);



#endif // CARS_H