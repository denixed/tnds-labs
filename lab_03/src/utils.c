#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

#include "io_matrix.h"
#include "matrix.h"
#include "random_matrix.h"

#define TEMP_FILE "./temp.out"
#define ACTIONS_COUNT 13

static matrix_t *full_matrix = NULL;
static sparse_matrix_t *sparse_matrix = NULL;

static matrix_t *full_vector = NULL;
static sparse_matrix_t *sparse_vector = NULL;

int input_matrix() {
  free_matrix(full_matrix);
  full_matrix = create_matrix_from_coord(stdin);
  if (!full_matrix) {
    return 1;
  }
  free_sparse_matrix(sparse_matrix);
  sparse_matrix = matrix_to_sparse(full_matrix);
  return 0;
}

int print_matrix_wrapper() {
  if (!full_matrix) {
    printf("Матрица еще не заполнена. ");
    return 0;
  }
  print_matrix(stdout, full_matrix);

  return 0;
}

int print_matrix_vim() {
  if (!full_matrix) {
    printf("Матрица еще не заполнена. ");
    return 0;
  }
  FILE *file = fopen(TEMP_FILE, "w");
  print_matrix(file, full_matrix);
  fclose(file);

  system("vim  -c \"set wrap!\"  -n " TEMP_FILE);

  file = fopen(TEMP_FILE, "r");

  matrix_t *temp = create_matrix_full(file);

  fclose(file);

  if (!temp) {
    printf("Файл испорчен.\n");
    return 1;
  }

  free_matrix(full_matrix);
  free_sparse_matrix(sparse_matrix);

  full_matrix = temp;
  sparse_matrix = matrix_to_sparse(full_matrix);

  return 0;
}

int random_matrix_wrapper() {
  size_t rows = 0, cols = 0, occupancy = 0;
  printf("Введите размеры матрицы и процент заполнености:");

  if (scanf("%zu%zu%zu", &rows, &cols, &occupancy) != 3 || !rows || !cols ||
      occupancy > 100) {
    wait_and_clear();
    return 1;
  }

  free_matrix(full_matrix);
  full_matrix = create_random_matrix_by_occupancy(rows, cols, occupancy);
  free_sparse_matrix(sparse_matrix);
  sparse_matrix = matrix_to_sparse(full_matrix);

  wait_and_clear();
  return 0;
}

int random_vector_wrapper() {
  size_t cols = 0, occupancy = 0;
  printf("Введите размер вектора и процент заполнености:");

  if (scanf("%zu%zu", &cols, &occupancy) != 2 || !cols || occupancy > 100) {
    wait_and_clear();
    return 1;
  }

  free_matrix(full_vector);
  full_vector = create_random_matrix_by_occupancy(1, cols, occupancy);
  free_sparse_matrix(sparse_vector);
  sparse_vector = matrix_to_sparse(full_vector);

  wait_and_clear();
  return 0;
}

int print_vector_wrapper() {
  if (!full_vector) {
    printf("Вектор еще не заполнен. ");
    return 0;
  }
  print_matrix(stdout, full_vector);
  return 0;
}

int print_vector_vim() {
  if (!full_vector) {
    printf("Вектор еще не заполнен. ");
    return 0;
  }
  FILE *file = fopen(TEMP_FILE, "w");
  print_matrix(file, full_vector);
  fclose(file);

  system("vim  -c \"set wrap!\"  -n " TEMP_FILE);

  file = fopen(TEMP_FILE, "r");

  matrix_t *temp = create_matrix_full(file);

  fclose(file);

  if (!temp) {
    printf("Файл испорчен.\n");
    return 1;
  }
  if (temp->rows != 1) {
    printf("Введенная сущность не является вектором.\n");
    return 1;
  }

  free_matrix(full_vector);
  free_sparse_matrix(sparse_vector);

  full_vector = temp;
  sparse_vector = matrix_to_sparse(full_vector);

  return 0;
}
matrix_t *create_vector_from_coord(FILE *stream) {
  int err = 0;

  size_t cols = 0;
  size_t count = 0;
  if (stream == stdin)
    printf("Введите кол-во столбцов и ненулевых элементов: ");

  if (!err) err = fscanf(stream, "%zu%zu", &cols, &count) != 2;

  if (!err && cols < count) err = 1;

  matrix_t *matrix = NULL;
  if (!err) matrix = init_matrix(1, cols);

  if (!err && !matrix) err = 1;

  for (size_t k = 0; k < count && !err; k++) {
    size_t j = 0;
    double element = 0;
    if (stream == stdin)
      printf("%3zu) Введите столбец элемента и сам элемент: ", k + 1);
    err = fscanf(stream, "%zu%" MATRIX_TYPE_SPECIF, &j, &element) != 2;
    if (!err && (j == 0)) err = 1;
    if (!err && (j > cols)) err = 1;
    if (!err) matrix->data[0][j - 1] = element;
  }

  if (err && matrix) {
    free_matrix(matrix);
    matrix = NULL;
  }

  return matrix;
}

int input_vector() {
  free_matrix(full_vector);
  full_vector = create_vector_from_coord(stdin);
  if (!full_vector) {
    return 1;
  }
  free_sparse_matrix(sparse_vector);
  sparse_vector = matrix_to_sparse(full_vector);
  return 0;
}

int mul_wrapper() {
  if (!full_matrix) {
    printf("Матрица еще не заполнена. ");
    return 0;
  }

  if (!full_vector) {
    printf("Вектор еще не заполнен. ");
    return 0;
  }

  if (full_vector->cols != full_matrix->rows) {
    printf("Матрицу размера %zuX%zu нельзя умнодить на вектор размера %zuX%zu.",
           full_matrix->rows, full_matrix->cols, full_vector->rows,
           full_vector->cols);
    return 0;
  }

  clock_t time_full = 0, time_sparse = 0;
  matrix_t *result_vector = mul(full_vector, full_matrix, &time_full);
  sparse_matrix_t *result_sparse =
      mul_sparse(sparse_vector, sparse_matrix, &time_sparse);

  matrix_t *temp = sparse_to_matrix(result_sparse);

  printf("### Результат умножения полных матриц.\n");
  print_matrix(stdout, result_vector);
  printf("\n ### Результат умножения разреженных матриц.\n");
  print_matrix(stdout, temp);

  printf("Умножение полных матриц было произведено за %ld тактов\n", time_full);
  printf("Умножение разреженных матриц было произведено за %ld тактов\n",
         time_sparse);

  free_matrix(temp);
  free_matrix(result_vector);
  free_sparse_matrix(result_sparse);

  return 0;
}
size_t matrix_size(size_t rows, size_t cols) {
  return sizeof(matrix_t) + sizeof(matrix_type_t) * cols * rows;
}

size_t sparse_matrix_size(size_t cols, size_t count) {
  return (sizeof(matrix_type_t) + sizeof(size_t)) * count +
         sizeof(size_t) * (cols + 1);
}

int mul_analize() {
  size_t repeats = 10;
  printf("#### Анализ умножения вектора-строки на матрицу ####\n");
  printf("Для каждого измерения взято среднее значение\n");
  printf("по времени для %4zu разных случайных пар матрица-", repeats);
  printf("вектор.\n\nВремя работы указано ");
  printf("в тактах.\nОбъём матриц указан в байтах.\nЗаполненность ");
  printf("указана в процентах\n\n");

  printf("| Размер  |Запол-|       Время умнож.  |    Объём матриц     |\n");
  printf("| матриц  |  сть |    полных| разрежен.|    полных| разрежен.|\n");
  printf("|---------|------|----------|----------|----------|----------|\n");

  for (size_t j = 16; j < 3025; j *= 2) {
    for (size_t i = 0; i <= 100; i += 10) {
      clock_t time_1 = 0, time_2 = 0;
      for (size_t k = 0; k < repeats; k++) {
        matrix_t *matrix = create_random_matrix_by_occupancy(j, j, i);
        matrix_t *vector = create_random_matrix_by_occupancy(1, j, i);
        sparse_matrix_t *sparse = matrix_to_sparse(matrix);
        sparse_matrix_t *svector = matrix_to_sparse(vector);

        clock_t cur_1 = 0;
        clock_t cur_2 = 0;

        free_matrix(mul(vector, matrix, &cur_1));
        free_sparse_matrix(mul_sparse(svector, sparse, &cur_2));
        time_1 += cur_1;
        time_2 += cur_2;

        free_matrix(matrix);
        free_matrix(vector);
        free_sparse_matrix(sparse);
        free_sparse_matrix(svector);
      }

      printf("|%4zuX%-4zu|  %3zu |%10ld|%10ld|%10zu|%10zu|\n", j, j, i,
             time_1 / repeats, time_2 / repeats,
             matrix_size(j, j) + matrix_size(1, j),
             sparse_matrix_size(j, j * j * i / 100) +
                 sparse_matrix_size(j, j * i / 100));
    }
  }
  return 0;
}

int clear_input() {
  int ch = 0;

  do {
    ch = getc(stdin);
  } while ((char)ch != '\n');

  return 0;
}
int wait_and_clear() {
  printf("Нажмите любую клавишу, чтобы продолжить.\n");
  clear_input();
  system("clear");
  return 0;
}

void exit_handler() {
  printf("До скорой встречи!\n");
  exit(0);
}

void print_menu() {
  static struct {
    char *name;
    int (*action)(void);
  } actions[ACTIONS_COUNT] = {
      {.name = "Работа с матрицей", .action = NULL},
      {.name = "Ввести матрицу", .action = input_matrix},
      {.name = "Сгенерировать случайную матрицу",
       .action = random_matrix_wrapper},
      {.name = "Редактировать матрицу в VIM", .action = print_matrix_vim},
      {.name = "Напечатать матрицу", .action = print_matrix_wrapper},
      {.name = "Работа с вектором", .action = NULL},
      {.name = "Ввести вектор", .action = input_vector},
      {.name = "Сгенерировать случайный вектор",
       .action = random_vector_wrapper},
      {.name = "Редактировать вектор в VIM", .action = print_vector_vim},
      {.name = "Напечатать вектор", .action = print_vector_wrapper},
      {.name = "Операция умножения", .action = NULL},
      {.name = "Умножить", .action = mul_wrapper},
      {.name = "Провести сравнение разных преставлений матриц",
       .action = mul_analize},
  };

  printf("Меню \n\n");

  size_t diff = 0;
  for (size_t i = 0; i < ACTIONS_COUNT; i++) {
    if (actions[i].action) {
      printf("%3zu) %s\n", i + 1 - diff, actions[i].name);
    } else {
      diff++;
      printf("===== %s\n", actions[i].name);
    }
  }

  printf("\n  0) Выход \n");

  printf("Введите номер команды: ");
  int action = 0;
  if (scanf("%d", &action) == 0 || action > ACTIONS_COUNT - diff ||
      action < 0) {
    wait_and_clear();
    printf("Неправильный номер команды.");
    return print_menu();
  }

  wait_and_clear();
  if (!action) return exit_handler();

  size_t i = 0;
  for (size_t j = 0; j < action; i++)
    if (actions[i].action) j++;

  if (actions[i - 1].action()) {
    wait_and_clear();
    printf("Произошла ошибка\n");
  }

  wait_and_clear();
  print_menu();
}