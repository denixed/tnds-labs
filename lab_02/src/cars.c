#include <string.h>
#include <wchar.h>

#include "cars.h"
#include "car.h"
#include "sorts.h"
#include "defines.h"


void fsort(cars_t *a, int first, int last)
{
    qsort(a->cars, a->amount, sizeof(car_t), cmp_marque);
}

void fast_key_sort(keys_t *a, int first, int last)
{
    qsort(a->data, a->amount, sizeof(short_car_info_t), short_cmp);
}

void slow_key_sort(keys_t *a)
{
    selection_sort(a->data, a->amount, sizeof(short_car_info_t), short_cmp);
}

void print_all(const cars_t *a)
{
    if (a->amount == 0)
    {
        printf("\nВ таблице нет ни одного элемента\n\n");
        return;
    }

    printf("|%15s|%15s|%15s|%15s|%15s|%15s|\n", "#", "Brand", "Country", "Price", "Color", "Condition");
    printf("|%15s|%15s|%15s|%15s|%15s|%15s|\n", SEPARATOR, SEPARATOR, SEPARATOR, SEPARATOR, SEPARATOR, SEPARATOR);

    for (size_t i = 0; i < a->amount; i++)
    {
        printf("|%15zu", i + 1);
        print(&a->cars[i]);
    }
}

void ssort(cars_t *a)
{
    selection_sort(a->cars, a->amount, sizeof(car_t), cmp_marque);
}

int price_within_range(const car_t *a, uint64_t from, uint64_t to)
{
    if (from <= a->price && a->price <= to)
        return 1;

    return 0;
}

void print_key_line(short_car_info_t *key)
{
    printf("Индекс:"
           " %zu\n"
           "Марка:"
           " %s\n",
           key->index,
           key->marque);
}

void print_key_table(keys_t *keys)
{
    if (keys->amount == 0)
    {
        printf("\nВ таблице ключей нет ни одного элемента\n\n");
        return;
    }
    printf("|%7s|%7s|%15s|\n", "#", "Real #", "Brand");
    printf("|%15s|%15s|\n", SEPARATOR, SEPARATOR);

    for (size_t i = 0; i < keys->amount; i++)
    {
        printf("|%7zu|%7zu|%15s|\n", i + 1, keys->data[i].index + 1, keys->data[i].marque);
        printf("|%15s|%15s|\n", SEPARATOR, SEPARATOR);
    }
}

void print_by_key_table(cars_t *data, keys_t *keys)
{
    size_t curr_index;

    if (keys->amount == 0)
    {
        printf("\nВ таблице ключей нет ни одного элемента\n\n");
        return;
    }

    printf("|%7s|%7s|%15s|%15s|%15s|%15s|%15s|\n", "#", "Real #","Brand", "Country", "Price", "Color", "Condition");
    printf("|%15s|%15s|%15s|%15s|%15s|%15s|\n", SEPARATOR, SEPARATOR, SEPARATOR, SEPARATOR, SEPARATOR, SEPARATOR);

    for (size_t i = 0; i < keys->amount; i++)
    {
        printf("|%7zu|%7zu", i + 1, keys->data[i].index + 1);
        
        curr_index = keys->data[i].index;
        print(&data->cars[curr_index]);
    }

}

void synchronize(const cars_t *src, keys_t *dst)
{
    dst->amount = src->amount;

    for (size_t i = 0; i < src->amount; i++)
    {
        dst->data[i].index = i;
        strcpy(dst->data[i].marque, src->cars[i].marque);
    }
}

int add(cars_t *a)
{
    /*
        Ответственность за переполнение на вызывающей стороне
    */

    if (a->amount == MAX_NUMOF_CARS)
        return ERR_OVERFLOW_CARS;
    int rc;

    size_t new = a->amount;

    if ((rc = input_marque(&a->cars[new])) != OK)
        return rc;

    if ((rc = input_country(&a->cars[new])) != OK)
        return rc;

    if ((rc = input_price(&a->cars[new])) != OK)
        return rc;

    if ((rc = input_color(&a->cars[new])) != OK)
        return rc;

    if ((rc = input_condition(&a->cars[new])) != OK)
        return rc;

    if ((rc = input_cond_attr(&a->cars[new])) != OK)
        return rc;

    a->amount++;

    return OK;
}

void delete (cars_t *a, size_t index)
{
    for (size_t i = index; i < a->amount - 1; i++)
        a->cars[i] = a->cars[i + 1];

    a->amount--;
}

int delete_by_marque(cars_t *a)
{
    size_t counter = 0;
    car_t target = {0};

    if (input_marque(&target) != OK)
        return ERR_READ_CAR_MARQUE;

    size_t i = 0;

    while (i < a->amount)
    {
        if (!cmp_marque(&target, &a->cars[i]))
        {
            delete (a, i);
            counter++;
        }
        else
            i++;
    }

    printf("\n[Было удалено автомобилей: %zu]\n", counter);

    return OK;
}

void searcher(const cars_t *data, car_t *target, uint64_t *p_from, uint64_t *p_to, cars_t *searched)
{
    /*Вывести цены не новых машин указанной марки с одним предыдущим 
    собственником, отсутствием ремонта в указанном диапазоне цен.*/

    searched->amount = 0;

    for (size_t i = 0; i < data->amount; i++)
    {
        if (!cmp_marque(&data->cars[i], target) && !cmp_condition(&data->cars[i], target))
        {
            if (!cmp_numof_owners(&data->cars[i], target) && !cmp_numof_repairs(&data->cars[i], target))
                if (price_within_range(&data->cars[i], *p_from, *p_to))
                    searched->cars[searched->amount++] = data->cars[i];
        }
    }
}

int read_line(FILE *f, car_t *car)
{
    int rc = 0;
    if ((rc = fscanf(f, "%[^;\n];%[^;\n];%" SCNu64 ";%[^;\n];%d;", car->marque,
                     car->country,
                     &car->price,
                     car->color,
                     (int *)&car->condition)) != 5)
        return FAIL;

    if (car->condition == NEW)
    {
        if (fscanf(f, "%" SCNu8 "\n", &car->cond_attr.for_new.guarantee) != 1)
            return FAIL;
    }
    else
    {
        if (fscanf(f, "%" SCNu16 ";%" SCNu32 ";%" SCNu16 ";%" SCNu16 "\n",
                   &car->cond_attr.for_pre_owned.product_year,
                   &car->cond_attr.for_pre_owned.milage,
                   &car->cond_attr.for_pre_owned.num_of_repairs,
                   &car->cond_attr.for_pre_owned.num_of_owners) != 4)
            return FAIL;
    }

    return OK;
}

void read_from_file(FILE *f, cars_t *a)
{
    a->amount = 0;

    while (read_line(f, &a->cars[a->amount]) == OK)
        a->amount++;
}

void write_line(FILE *f, car_t *car)
{
    fprintf(f, "%s;%s;%" PRIu64 ";%s;%d;", car->marque,
            car->country,
            car->price,
            car->color,
            car->condition);

    if (car->condition == NEW)
    {
        fprintf(f, "%" PRIu8 "\n", car->cond_attr.for_new.guarantee);
    }
    else
    {
        fprintf(f, "%" PRIu16 ";%" PRIu32 ";%" PRIu16 ";%" PRIu16 "\n",
                car->cond_attr.for_pre_owned.product_year,
                car->cond_attr.for_pre_owned.milage,
                car->cond_attr.for_pre_owned.num_of_repairs,
                car->cond_attr.for_pre_owned.num_of_owners);
    }
}

void write_to_file(FILE *f, cars_t *a)
{
    for (size_t i = 0; i < a->amount; i++)
    {
        write_line(f, &a->cars[i]);
    }
}