#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "car.h"
#include "cars.h"
#include "defines.h"
#include "utils.h"

int main(void) {
  srand(time(NULL));
  setlocale(LC_ALL, "ru_RU.UTF-8");

  int choice = -1;
  cars_t data = {0};
  keys_t key_data = {0};
  int rc = OK;

  while (choice) {
    print_start_menu();
    rc = choice_menu(&data, &key_data, &choice);

    if (rc >= 1 && rc <= 3) printf("\nНекорректный ввод строки\n");

    if (rc >= 4 && rc <= 5) printf("\nНекорректный ввод числа\n");

    if (rc == ERR_READ_PRICE_RANGE) printf("\nНекорректный ввод диапазона\n");

    if (rc == INCORRECT_CHOICE_RANGE || rc == ERR_READ_CAR_CONDITION ||
        rc == ERR_INPUT_CHOICE)
      printf("\nТакого пункта нет\n");

    if (rc == ERR_OVERFLOW_CARS) printf("\nПроизошло переполнение таблицы\n");

    if (rc == ERR_EMPTY_INPUT) printf("\nПоле не может быть пустым\n");

    printf("\nНажмите <Enter>, чтобы продолжить.");
    getc(stdin);
    system("clear");
  }

  return OK;
}
