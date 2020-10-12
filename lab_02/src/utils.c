#include "utils.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "car.h"
#include "cars.h"
#include "defines.h"
#include "sorts.h"

#define CHOICE_FROM 0
#define CHOICE_TO 15
/**
 * @brief Оценить сортировки
 *
 * @param unsorted_data
 * @param unsorted_keys
 */
void evaluate(cars_t *unsorted_data, keys_t *unsorted_keys) {
  setbuf(stdout, NULL);
  printf("Сортировка выполняется... Ожидайте, пожалуйста.\n");

  cars_t data_buf, *data = &data_buf;
  keys_t keys_buf, *keys = &keys_buf;

  clock_t dur_fs_main = 0;
  printf(
      "Тестирование скорости сортировок для %d повторений и %zu элементов:\n",
      EVALUATE_COUNT, unsorted_data->amount);

  printf("\nБыстрая сортировка основной таблицы:   0%c", '%');
  for (size_t i = 0; i < EVALUATE_COUNT; i++) {
    data_buf = *unsorted_data;

    clock_t start = clock();
    if (start < 0) printf("Ошибка \n");
    fsort(data, 0, data->amount - 1);
    clock_t end = clock();
    if (end < 0) printf("Ошибка \n");

    dur_fs_main += end - start;
    printf("\b\b\b\b%3ld%c", (i + 1) * 100 / EVALUATE_COUNT, '%');
  }

  clock_t dur_fs_key = 0;
  printf("\nБыстрая сортировка доп.таблицы:   0%c", '%');
  for (size_t i = 0; i < EVALUATE_COUNT; i++) {
    keys_buf = *unsorted_keys;

    clock_t start = clock();
    if (start < 0) printf("Ошибка \n");
    fast_key_sort(keys, 0, keys->amount - 1);
    clock_t end = clock();
    if (end < 0) printf("Ошибка \n");
    dur_fs_key += end - start;
    printf("\b\b\b\b%3ld%c", (i + 1) * 100 / EVALUATE_COUNT, '%');
  }

  clock_t dur_ss_main = 0;

  printf("\nСортировка выбором основной таблицы:   0%c", '%');
  for (size_t i = 0; i < EVALUATE_COUNT; i++) {
    data_buf = *unsorted_data;

    clock_t start = clock();
    if (start < 0) printf("Ошибка \n");
    ssort(data);
    clock_t end = clock();
    if (end < 0) printf("Ошибка \n");
    dur_ss_main += end - start;
    printf("\b\b\b\b%3ld%c", (i + 1) * 100 / EVALUATE_COUNT, '%');
  }

  clock_t dur_ss_key = 0;

  printf("\nСортировка выбором доп. таблицы:   0%c", '%');
  for (size_t i = 0; i < EVALUATE_COUNT; i++) {
    keys_buf = *unsorted_keys;

    clock_t start = clock();
    if (start < 0) printf("Ошибка \n");
    slow_key_sort(keys);
    clock_t end = clock();
    if (end < 0) printf("Ошибка \n");
    dur_ss_key += end - start;
    printf("\b\b\b\b%3ld%c", (i + 1) * 100 / EVALUATE_COUNT, '%');
  }

  size_t fs_key_size = sizeof(car_t) * data->amount +
                       sizeof(short_car_info_t) * keys->amount +
                       sizeof(int) * keys->amount * 2;
  size_t ss_key_size = sizeof(car_t) * data->amount +
                       sizeof(short_car_info_t) * (keys->amount + 1);
  size_t fs_main_size =
      sizeof(car_t) * data->amount + sizeof(int) * keys->amount * 2;
  size_t ss_main_size = sizeof(car_t) * (data->amount + 1);

  system("clear");

  printf(
      "Тестирование скорости сортировок для %d повторений и %zu элементов:\n",
      EVALUATE_COUNT, data->amount);

  printf("%s - %ld; Объём памяти: %zu\n",
         "Быстрая сортировка (основная таблица) ", dur_fs_main / EVALUATE_COUNT,
         fs_main_size);
  printf("%s - %ld; Объём памяти: %zu\n",
         "Быстрая сортировка (таблица ключей)   ", dur_fs_key / EVALUATE_COUNT,
         fs_key_size);

  printf("%s - %ld; Объём памяти: %zu\n",
         "Сортировка выбором (основная таблица) ", dur_ss_main / EVALUATE_COUNT,
         ss_main_size);
  printf("%s - %ld; Объём памяти: %zu\n",
         "Cортировка выбором (таблица ключей)   ", dur_ss_key / EVALUATE_COUNT,
         ss_key_size);
}
/**
 * @brief Ввести границы поиска
 *
 * @param from
 * @param to
 * @return int
 */
int input_price_range(uint64_t *from, uint64_t *to) {
  char temp_price[15];

  printf("Пожалуйста, выберите границы цены [A, B]\n");

  printf("Введите минимальную цену автомобиля ('A') и нажмите <Enter>: ");

  if (!fgets(temp_price, sizeof(temp_price), stdin)) return FAIL;
  if (temp_price[strlen(temp_price) - 1] != '\n') return FAIL;
  temp_price[strlen(temp_price) - 1] = '\0';
  if (!strlen(temp_price)) return ERR_EMPTY_INPUT;

  if (!is_only_digits(temp_price)) return ERR_READ_CAR_PRICE;

  *from = (uint64_t)atoll(temp_price);

  printf("Введите максимальную цену автомобиля ('B') и нажмите <Enter>: ");

  if (!fgets(temp_price, sizeof(temp_price), stdin)) return FAIL;
  if (temp_price[strlen(temp_price) - 1] != '\n') return FAIL;
  temp_price[strlen(temp_price) - 1] = '\0';
  if (!strlen(temp_price)) return ERR_EMPTY_INPUT;

  if (!is_only_digits(temp_price)) return ERR_READ_CAR_PRICE;

  *to = (uint64_t)atoll(temp_price);

  if (*from > *to) return ERR_READ_PRICE_RANGE;

  return OK;
}

/**
 * @brief Поиск по условию
 *
 * @param data
 * @return int
 */
int search(const cars_t *data) {
  uint64_t from, to;
  cars_t searched;
  car_t target = {.condition = PRE_OWNED,
                  .cond_attr.for_pre_owned.num_of_owners = 1,
                  .cond_attr.for_pre_owned.num_of_repairs = 0};
  int rc;
  printf(
      "Поиcк поддержанных автомобилей указанного бренда с одним предыдущим \n"
      "собственником, отсутствием ремонта в указанном диапазоне цен.\n");
  if ((rc = input_marque(&target)) != OK) return rc;

  if ((rc = input_price_range(&from, &to)) != OK) return rc;

  searcher(data, &target, &from, &to, &searched);

  print_all(&searched);

  return OK;
}
/**
 * @brief Напечатать стартовое меню
 *
 */
void print_start_menu() {
  system("clear");

  printf(
      "Доступные команды:\n"
      "1)  Вывести автомобили\n"
      "2)  Вывести автомобили (с помощью таблицы ключей)\n"
      "3)  Вывести таблицу ключей\n\n"
      "4)  Добавить автомобиль в таблицу\n"
      "5)  Удалить автомобиль из таблицы (по бренду)\n\n"
      "6)  Быстрая сортировка (основная таблица)\n"
      "7)  Быстрая сортировка (таблица ключей)\n"
      "8)  Cортировка выбором (основная таблица)\n"
      "9)  Cортировка выбором (таблица ключей)\n\n"
      "10) Синхронизировать таблицу ключей с основной таблицей\n\n"
      "11) Поиск по условию\n\n"
      "12) Считать таблицу из файла\n"
      "13) Сохранить таблицу в файл\n\n"
      "14) Оценка сортировок\n\n"
      "15) Перемешать таблицы\n\n"
      "0)  Выход\n\n");
}

/**
 * @brief Ввести вариант ответов
 *
 * @param choice
 * @return int
 */
int input_choice(int *choice) {
  char temp_choice[4] = {0};

  if (!fgets(temp_choice, sizeof(temp_choice), stdin)) return FAIL;
  if (temp_choice[strlen(temp_choice) - 1] != '\n') return FAIL;
  temp_choice[strlen(temp_choice) - 1] = '\0';

  if (!strlen(temp_choice))
    *choice = 0;
  else {
    if (!is_only_digits(temp_choice)) return FAIL;
    if (strlen(temp_choice) > 1 && temp_choice[0] == '0') return FAIL;

    *choice = atoi(temp_choice);
  }

  return OK;
}
/**
 * @brief Запустить меню
 *
 * @param data
 * @param key_data
 * @param choice
 * @return int
 */
int choice_menu(cars_t *data, keys_t *key_data, int *choice) {
  FILE *f;

  printf("Введите номер команды (%d - %d): ", CHOICE_FROM, CHOICE_TO);

  if (input_choice(choice) != OK) return ERR_INPUT_CHOICE;

  if (*choice < CHOICE_FROM || *choice > CHOICE_TO)
    return INCORRECT_CHOICE_RANGE;

  int rc = OK;
  system("clear");

  switch (*choice) {
    case (1):
      print_all(data);
      break;

    case (2):
      print_by_key_table(data, key_data);
      break;

    case (3):
      print_key_table(key_data);
      break;

    case (4):
      rc = add(data);
      break;

    case (5):
      rc = delete_by_marque(data);
      synchronize(data, key_data);
      break;

    case (6):
      fsort(data, 0, data->amount - 1);
      synchronize(data, key_data);

      break;

    case (7):
      fast_key_sort(key_data, 0, key_data->amount - 1);
      break;

    case (8):
      ssort(data);
      synchronize(data, key_data);
      break;

    case (9):
      slow_key_sort(key_data);
      break;

    case (10):
      printf("Таблица ключей синхронизирована.\n");
      synchronize(data, key_data);
      break;

    case (11):
      rc = search(data);
      break;

    case (12):
      f = fopen(FILENAME, "r");
      if (f == NULL) {
        f = fopen(FILENAME, "w");
        fclose(f);
        f = fopen(FILENAME, "r");
      }

      read_from_file(f, data);
      synchronize(data, key_data);

      fclose(f);
      printf("Данные из файла успешно считаны\n");
      break;

    case (13):
      f = fopen(FILENAME, "w");
      write_to_file(f, data);
      fclose(f);
      printf("Таблица сохранена в файл\n");
      break;

    case (14):
      evaluate(data, key_data);
      break;

    case (15):
      shuffle(data->cars, data->amount, sizeof(car_t));
      synchronize(data, key_data);
      print_all(data);
      break;

    case (0):
      printf("До скорой встречи!\n");
      break;
  }

  return rc;
}
