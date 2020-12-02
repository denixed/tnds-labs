#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "handler.h"

double random_double(const double bottom, const double top) {
  double range = top - bottom;
  double div = RAND_MAX / range;

  return bottom + (rand() / div);
}

#define ACTIONS_COUNT 9

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
queue_handler_config_t config;

int reset_config() {
  config.queue[0].in_max = 5;
  config.queue[0].in_min = 1;
  config.queue[0].proccess_max = 4;
  config.queue[0].proccess_min = 0;

  config.queue[1].in_max = 3;
  config.queue[1].in_min = 0;
  config.queue[1].proccess_max = 1;
  config.queue[1].proccess_min = 0;

  config.number_of_requests_1 = 1000;
  config.print_addresses = true;
  config.print_stats = true;
  config.disable_print = false;

  return 0;
}

int handle_queue_array_wrapper() {
  create_handle_queues(QUEUE_ARRAY, &config);
  return 0;
}

int handle_queue_list_wrapper() {
  create_handle_queues(QUEUE_LIST, &config);
  return 0;
}

int analize_wrapper() {
  queue_handler_config_t config_copy = config;
  config_copy.print_addresses = false;
  config_copy.print_stats = false;
  config_copy.disable_print = true;

  for (size_t i = 0; i < QUEUE_COUNT; i++) {
    queue_config_t *q = config_copy.queue + i;
    q->in_min = q->in_max ;//= (q->in_max + q->in_min) * 1;
    q->proccess_min = q->proccess_max ;//= (q->proccess_max + q->proccess_min) * 1;
  }

  // config_copy.number_of_requests_1 *= 2;

  queue_t *queues[QUEUE_COUNT] = {0};

  size_t repeats = 1000;  // Number of repeats for analize

  printf("Анализ эффективности работы очереди, построенной \n");
  printf("- на списке \n- на зацикленном расширяемом массив\n\n");
  printf("\nВ таблице приведено среднее для %zu повторений\n\n", repeats);

  // Array analize
  for (size_t i = 0; i < QUEUE_COUNT; i++)
    queues[i] = create_queue(QUEUE_ARRAY);

  size_t size_array = 0;
  size_t size_used_array = 0;

  clock_t start = clock();
  for (size_t i = 0; i < repeats; i++) {
    for (size_t i = 0; i < QUEUE_COUNT; i++) empty_queue(queues[i]);
    handle_queue(queues, &config_copy);

    size_t temp_size =
        get_queue_memory_size(queues[0]->queue.array->capacity, QUEUE_ARRAY) +
        get_queue_memory_size(queues[1]->queue.array->capacity, QUEUE_ARRAY);
    size_t temp_size_used =
        get_queue_memory_size(length_queue(queues[0]), QUEUE_ARRAY) +
        get_queue_memory_size(length_queue(queues[1]), QUEUE_ARRAY);
    if (temp_size > size_array) size_array = temp_size;
    if (temp_size_used > size_used_array) size_used_array = temp_size_used;
  }
  clock_t time_array = clock() - start;

  for (size_t i = 0; i < QUEUE_COUNT; i++) free_queue(queues[i]);

  // List analize
  for (size_t i = 0; i < QUEUE_COUNT; i++) queues[i] = create_queue(QUEUE_LIST);

  size_t size_list = 0;
  start = clock();
  for (size_t i = 0; i < repeats; i++) {
    for (size_t i = 0; i < QUEUE_COUNT; i++) empty_queue(queues[i]);
    handle_queue(queues, &config_copy);

    size_t temp_size =
        get_queue_memory_size(length_queue(queues[0]), QUEUE_LIST) +
        get_queue_memory_size(length_queue(queues[1]), QUEUE_LIST);

    if (temp_size > size_list) size_list = temp_size;
  }
  clock_t time_list = clock() - start;

  for (size_t i = 0; i < QUEUE_COUNT; i++) free_queue(queues[i]);

  printf("Очередь на массиве:\n");
  printf("- время работы: %zu тиков\n", time_array / repeats);
  printf("- объём занимаемой памяти: %zu Байт\n", size_array);
  printf("- объём используемой памяти: %zu Байт\n\n", size_used_array);

  printf("Очередь на списке:\n");
  printf("- время работы: %zu тиков\n", time_list / repeats);
  printf("- объём занимаемой памяти: %zu Байт\n", size_list);

  return 0;
}

int show_config_wrapper() {
  printf("Параметры: \n");
  for (size_t i = 0; i < QUEUE_COUNT; i++) {
    printf("Заявки %zu-типа: \n", i + 1);
    printf("- интервал прихода: [%lf;%lf]\n", config.queue[i].in_min,
           config.queue[i].in_max);
    printf("- интервал обработки: [%lf;%lf]\n", config.queue[i].proccess_min,
           config.queue[i].proccess_max);
  }

  static char *options[] = {"Выключен", "Включен"};
  printf("Кол-во заявок 1-го типо для обработки: %zu\n",
         config.number_of_requests_1);
  printf("Печать отладочной информации: %s\n", options[config.print_stats]);
  printf("Печать адресов: %s\n", options[config.print_addresses]);

  return 0;
}

int edit_config_wrapper() {
  queue_handler_config_t tconfig = config;
  printf("Параметры: \n");

  for (size_t i = 0; i < QUEUE_COUNT; i++) {
    printf("Заявки %zu-типа: \n", i + 1);
    printf("- интервал прихода [%lf;%lf]: ", config.queue[i].in_min,
           config.queue[i].in_max);
    if (scanf("%lf%lf", &(tconfig.queue[i].in_min),
              &(tconfig.queue[i].in_max)) != 2)
      return -1;
    if (tconfig.queue[i].in_min > tconfig.queue[i].in_max) return -1;
    printf("- интервал обработки [%lf;%lf]: ", config.queue[i].proccess_min,
           config.queue[i].proccess_max);

    if (scanf("%lf%lf", &(tconfig.queue[i].proccess_min),
              &(tconfig.queue[i].proccess_max)) != 2)
      return -1;
    if (tconfig.queue[i].proccess_min > tconfig.queue[i].proccess_max)
      return -1;
  }

  static char *options[] = {"Выкл", "Вкл"};
  printf("Кол-во заявок 1-го типо для обработки [%zu]: ",
         config.number_of_requests_1);
  if (scanf("%zu", &(tconfig.number_of_requests_1)) != 1) return -1;

  printf("Используйте следующие значения: \n");
  for (size_t i = 0; i < 2; i++) {
    printf("- %zu == %s \n", i, options[i]);
  }
  int temp = 0;
  printf("Печать отладочной информации [%s]: ", options[config.print_stats]);
  if (scanf("%d", &temp) != 1) return -1;
  tconfig.print_stats = temp;
  printf("Печать адресов [%s]: ", options[config.print_addresses]);
  if (scanf("%d", &temp) != 1) return -1;
  tconfig.print_addresses = temp;

  config = tconfig;
  return 0;
}

void print_menu() {
  static struct {
    char *name;
    int (*action)(void);
  } actions[ACTIONS_COUNT] = {
      {.name = "Моделирование", .action = NULL},
      {.name = "на массиве", .action = handle_queue_array_wrapper},
      {.name = "на листе", .action = handle_queue_list_wrapper},
      {.name = "Параметры моделирования", .action = NULL},
      {.name = "Просмотреть параметры", .action = show_config_wrapper},
      {.name = "Редактировать параметры", .action = edit_config_wrapper},
      {.name = "Сбросить параметры", .action = reset_config},
      {.name = "Анализ", .action = NULL},
      {.name = "Анализ эффективности", .action = analize_wrapper},

  };
  reset_config();
  while (1) {
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
  }
}
