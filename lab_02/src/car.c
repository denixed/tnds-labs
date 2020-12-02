#include "car.h"

#include <ctype.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "defines.h"
/**
 * @brief Сравнить элементы доп таблицы по бренду
 *
 * @param a
 * @param b
 * @return int
 */
int short_cmp(const void *a, const void *b) {
  return strcasecmp(((short_car_info_t *)a)->marque,
                    ((short_car_info_t *)b)->marque);
}
/**
 * @brief Сравнить элементы основной таблицы по бренду
 *
 * @param a
 * @param b
 * @return int
 */
int cmp_marque(const void *a, const void *b) {
  return strcasecmp(((car_t *)a)->marque, ((car_t *)b)->marque);
}
/**
 * @brief Сравнить элементы основной таблицы по стране производства
 *
 * @param a
 * @param b
 * @return int
 */
int cmp_country(const car_t *a, const car_t *b) {
  return strcasecmp(a->country, b->country);
}
/**
 * @brief Сравнить по цене
 *
 * @param a
 * @param b
 * @return int
 */
int cmp_price(const car_t *a, const car_t *b) {
  if (a->price < b->price)
    return -1;
  else if (a->price > b->price)
    return 1;

  return 0;
}
/**
 * @brief Сравнить по цвету
 *
 * @param a
 * @param b
 * @return int
 */
int cmp_color(const car_t *a, const car_t *b) {
  return strcasecmp(a->color, b->color);
}
/**
 * @brief Сравнить по состоянию
 *
 * @param a
 * @param b
 * @return int
 */
int cmp_condition(const car_t *a, const car_t *b) {
  return !(a->condition == b->condition);
}
/**
 * @brief Сравнить по кол-ву собственникова
 *
 * @param a
 * @param b
 * @return int
 */
int cmp_numof_owners(const car_t *a, const car_t *b) {
  return a->cond_attr.for_pre_owned.num_of_owners -
         b->cond_attr.for_pre_owned.num_of_owners;
}

/**
 * @brief Срвнить по кол-ву ремонтов
 *
 * @param a
 * @param b
 * @return int
 */
int cmp_numof_repairs(const car_t *a, const car_t *b) {
  return a->cond_attr.for_pre_owned.num_of_repairs -
         b->cond_attr.for_pre_owned.num_of_repairs;
}

/**
 * @brief Напечатать вариативную часть в таблицу
 *
 * @param a
 */
void print_cond_attr(const car_t *a) {
  if (a->condition == NEW)

    printf(
        "%15s|\n"
        "|%15s|%10d year|%15s|%15s|%15s|%15s|\n",
        "New", "Guarantee:", a->cond_attr.for_new.guarantee, "", "", "", "");
  else
    printf(
        "%15s|\n"
        "|%15s|%10d year|%15s|%12d km|%15s|%7d/%7d|\n",
        "Old", "Product year:", a->cond_attr.for_pre_owned.product_year,
        "Milage:", a->cond_attr.for_pre_owned.milage,
        "Repairs/Owners:", a->cond_attr.for_pre_owned.num_of_repairs,
        a->cond_attr.for_pre_owned.num_of_owners);
}
/**
 * @brief Напечатать строку таблицы
 *
 * @param a
 */
void print(const car_t *a) {
  printf("|%15s|%15s|%10" PRIu64 " rub.|%15s|", a->marque, a->country, a->price,
         a->color);

  print_cond_attr(a);

  printf("|%15s|%15s|%15s|%15s|%15s|%15s|\n", SEPARATOR, SEPARATOR, SEPARATOR,
         SEPARATOR, SEPARATOR, SEPARATOR);
}
/**
 * @brief Ввод марки
 *
 * @param car
 * @return int
 */
int input_marque(car_t *car) {
  printf("Введите бренд: ");
  if (!fgets(car->marque, sizeof(car->marque), stdin))
    return ERR_READ_CAR_MARQUE;
  if (car->marque[strlen(car->marque) - 1] != '\n') return ERR_READ_CAR_MARQUE;
  car->marque[strlen(car->marque) - 1] = '\0';
  if (!strlen(car->marque)) return ERR_EMPTY_INPUT;

  return OK;
}
/**
 * @brief Ввод страны
 *
 * @param car
 * @return int
 */
int input_country(car_t *car) {
  printf("Введите название страны-производителя: ");
  if (!fgets(car->country, sizeof(car->country), stdin))
    return ERR_READ_CAR_COUNTRY;
  if (car->country[strlen(car->country) - 1] != '\n')
    return ERR_READ_CAR_COUNTRY;
  car->country[strlen(car->country) - 1] = '\0';
  if (!strlen(car->country)) return ERR_EMPTY_INPUT;

  return OK;
}
/**
 * @brief Проверка на цифры переданной строки
 *
 * @param str
 * @return int
 */
int is_only_digits(char *str) {
  size_t i = 0;

  while (str[i]) {
    if (!isdigit((int)str[i])) return 0;
    i++;
  }
  return 1;
}
/**
 * @brief Ввод цены
 *
 * @param car
 * @return int
 */
int input_price(car_t *car) {
  char temp_price[15];

  printf("Введите цену автомобиля (руб.) и нажмите <Enter>: ");
  if (!fgets(temp_price, sizeof(temp_price), stdin)) return ERR_READ_CAR_PRICE;
  if (check_int(temp_price) != OK) return ERR_READ_CAR_PRICE;
  if (!strlen(temp_price)) return ERR_EMPTY_INPUT;

  car->price = (uint64_t)atoll(temp_price);

  return OK;
}

/**
 * @brief Ввод цвета
 *
 * @param car
 * @return int
 */
int input_color(car_t *car) {
  printf("Введите цвет автомобиля и нажмите <Enter>: ");
  if (!fgets(car->color, sizeof(car->color), stdin)) return ERR_READ_CAR_COLOR;
  if (car->color[strlen(car->color) - 1] != '\n') return ERR_READ_CAR_COLOR;
  car->color[strlen(car->color) - 1] = '\0';
  if (!strlen(car->color)) return ERR_EMPTY_INPUT;

  return OK;
}
/**
 * @brief Ввод состояния автомобиля
 *
 * @param car
 * @return int
 */
int input_condition(car_t *car) {
  char _tmp_[1 + 2] = {0};

  printf(
      "Выберите состояние автомобиля:\n"
      "0. Новый\n"
      "1. Подержанный\n"
      "Введите <0> или <1>"
      " и нажмите <Enter>): ");
  if (!fgets(_tmp_, sizeof(_tmp_), stdin)) return ERR_READ_CAR_CONDITION;
  if (check_int(_tmp_) != OK) return ERR_READ_CAR_CONDITION;
  if (!strlen(_tmp_)) return ERR_EMPTY_INPUT;

  car->condition = atoi(_tmp_);

  if (car->condition != NEW && car->condition != PRE_OWNED)
    return ERR_READ_CAR_CONDITION;

  return OK;
}
/**
 * @brief Проверка целового в переданной строке
 *
 * @param str
 * @return int
 */
int check_int(char *str) {
  if (str[strlen(str) - 1] != '\n') return FAIL;

  str[strlen(str) - 1] = '\0';

  if (!is_only_digits(str)) return FAIL;

  if (strlen(str) > 1 && str[0] == '0') return FAIL;

  return OK;
}
/**
 * @brief Ввод вариативной
 *
 * @param car
 * @return int
 */
int input_cond_attr(car_t *car) {
  /*
      Вызывать при известном car->condition !
  */

  char _int_8_[2 + 2] = {0};
  char _int_16_[4 + 2] = {0};
  char _int_32_[8 + 2] = {0};

  if (car->condition == NEW) {
    printf("Введите количество лет гарантии (0-99) и нажмите <Enter>: ");
    if (!fgets(_int_8_, sizeof(_int_8_), stdin)) return ERR_READ_CAR_COND_ATTR;
    if (check_int(_int_8_) != OK) return ERR_READ_CAR_COND_ATTR;
    if (!strlen(_int_8_)) return ERR_EMPTY_INPUT;
    car->cond_attr.for_new.guarantee = (uint8_t)atoi(_int_8_);
  } else {
    printf("Введите год производства и нажмите <Enter>: ");
    if (!fgets(_int_16_, sizeof(_int_16_), stdin))
      return ERR_READ_CAR_COND_ATTR;
    if (check_int(_int_16_) != OK) return ERR_READ_CAR_COND_ATTR;
    if (!strlen(_int_16_)) return ERR_EMPTY_INPUT;
    car->cond_attr.for_pre_owned.product_year = (uint16_t)atol(_int_16_);

    printf("Введите пробег (км.) и нажмите <Enter>: ");
    if (!fgets(_int_32_, sizeof(_int_32_), stdin))
      return ERR_READ_CAR_COND_ATTR;
    if (check_int(_int_32_) != OK) return ERR_READ_CAR_COND_ATTR;
    if (!strlen(_int_32_)) return ERR_EMPTY_INPUT;
    car->cond_attr.for_pre_owned.milage = (uint32_t)atol(_int_32_);

    printf(
        "Введите количество ремонтов (0-9999) автомобиля и нажмите <Enter>: ");
    if (!fgets(_int_16_, sizeof(_int_16_), stdin))
      return ERR_READ_CAR_COND_ATTR;
    if (check_int(_int_16_) != OK) return ERR_READ_CAR_COND_ATTR;
    if (!strlen(_int_16_)) return ERR_EMPTY_INPUT;
    car->cond_attr.for_pre_owned.num_of_repairs = (uint16_t)atol(_int_16_);

    printf(
        "Введите количество предыдущих владельцев автомобиля (0-9999) и "
        "нажмите <Enter>: ");
    if (!fgets(_int_16_, sizeof(_int_16_), stdin))
      return ERR_READ_CAR_COND_ATTR;
    if (check_int(_int_16_) != OK) return ERR_READ_CAR_COND_ATTR;
    if (!strlen(_int_16_)) return ERR_EMPTY_INPUT;
    car->cond_attr.for_pre_owned.num_of_owners = (uint16_t)atol(_int_16_);
  }

  return OK;
}