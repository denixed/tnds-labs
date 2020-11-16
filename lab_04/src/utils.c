#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "array_stack.h"
#include "list_stack.h"

#define ACTIONS_COUNT 10

static array_stack_t *array_stack = NULL;
static list_stack_t *list_stack = NULL;

int print_array_stack()
{
  printf("Стек содержит %zu элементов\n",
         array_stack->end - array_stack->buffer);

  printf("Длина стека %zu элементов\n\n", array_stack->capacity);

  for (stack_type_t *pi = array_stack->end - 1; pi >= array_stack->buffer; pi--)
    printf("Адрес: %10p Элемент: " STACK_TYPE_SPEC "\n", pi, *pi);

  return 0;
}

int push_array_stack_wrapper()
{
  stack_type_t element;
  printf("Введите адресс: ");

  if (scanf(STACK_TYPE_SPEC, &element) != 1)
    return 1;
  wait_and_clear();
  return push_array_stack(array_stack, element);
}

int pop_array_stack_wrapper()
{
  stack_type_t element;

  if (pop_array_stack(array_stack, &element))
    return 1;

  printf("Получили элемент стека: " STACK_TYPE_SPEC "\n", element);

  return 0;
}

int print_list_stack()
{
  size_t size = 0;
  size_t free_size = 0;

  node_t *cur = list_stack->list;
  while (cur)
  {
    size++;
    cur = cur->next;
  }

  cur = list_stack->free_nodes;
  while (cur)
  {
    free_size++;
    cur = cur->next;
  }

  printf("Стек содержит %zu элементов\n", size);

  printf("Список свободных областей содержит %zu элементов\n\n", free_size);

  if (size)
    printf("Элементы стека:\n");
  for (node_t *cur = list_stack->list; cur; cur = cur->next)
    printf("Адрес: %10p Элемент: " STACK_TYPE_SPEC "\n", cur, cur->value);

  if (free_size)
    printf("Элементы cписка свободных областей:\n");
  for (node_t *cur = list_stack->free_nodes; cur; cur = cur->next)
    printf("Адрес: %10p \n", cur);

  return 0;
}

int push_list_stack_wrapper()
{
  push_list_stack(list_stack);
  return 0;
}

int pop_list_stack_wrapper()
{
  stack_type_t element;

  if (pop_list_stack(list_stack, &element))
    return 1;

  printf("Получили элемент стека: " STACK_TYPE_SPEC "\n", element);

  return 0;
}

size_t size_array_stack(size_t capacity)
{
  return capacity * sizeof(stack_type_t) + sizeof(array_stack_t);
}

size_t size_list_stack(size_t count)
{
  return count * sizeof(node_t) + sizeof(list_stack_t);
}

int compare_methods()
{
  size_t counts = 100;

  printf(
      "## Анализ скорости работы и объёма занимаемой памяти для различных \n"
      "представлений стека. \n\n");

  printf("Время указано в тактах процессора \n");
  printf("Объём указан в Байтах \n\n");

  printf(
      "|Длинна|  Время добавления в |  Время удаления из  |    Объём "
      "памяти     |\n");
  printf(
      "| стека| массив   |  список  | массив   |  список  | массив   |  список "
      " |\n");
  printf(
      "|------|----------|----------|----------|----------|----------|---------"
      "-|\n");

  for (size_t i = 4; i < 10000000; i *= 4)
  {
    clock_t start, time_push_array = 0, time_push_list = 0, time_pop_array = 0,
                   time_pop_list = 0;

    for (size_t k = 0; k < counts; k++)
    {
      array_stack_t *a = init_array_stack();
      list_stack_t *l = init_list_stack();

      start = clock();
      for (size_t j = 0; j < i; j++)
        push_array_stack(a, (stack_type_t)j);
      time_push_array += clock() - start;

      start = clock();
      for (size_t j = 0; j < i; j++)
        push_list_stack(l);
      time_push_list += clock() - start;

      stack_type_t element;
      start = clock();
      for (size_t j = 0; j < i; j++)
        pop_array_stack(a, &element);
      
      free_array_stack(a);
      time_pop_array += clock() - start;

      start = clock();
      for (size_t j = 0; j < i; j++)
        pop_list_stack(l, &element);
      free_list_stack(l);

      time_pop_list += clock() - start;

    }

    printf("|%6zu|%10ld|%10ld|%10ld|%10ld|%10ld|%10ld|\n", i,
           time_push_array / counts, time_push_list / counts,
           time_pop_array / counts, time_pop_list / counts, size_array_stack(i),
           size_list_stack(i));
  }

  for (size_t i = 4; i < 10000000; i *= 4)
  {
    i++;

    clock_t start, time_push_array = 0, time_push_list = 0, time_pop_array = 0,
                   time_pop_list = 0;

    for (size_t k = 0; k < counts; k++)
    {
      
      array_stack_t *a = init_array_stack();
      list_stack_t *l = init_list_stack();

      start = clock();
      
      for (size_t j = 0; j < i; j++)
        push_array_stack(a, (stack_type_t)j);
      time_push_array += clock() - start;

      start = clock();
      for (size_t j = 0; j < i; j++)
        push_list_stack(l);
      time_push_list += clock() - start;

      stack_type_t element;
      start = clock();
      for (size_t j = 0; j < i; j++)
        pop_array_stack(a, &element);
      
      free_array_stack(a);

      time_pop_array += clock() - start;

      start = clock();
      for (size_t j = 0; j < i; j++)
        pop_list_stack(l, &element);

      free_list_stack(l);

      time_pop_list += clock() - start;


    }

    printf("|%6zu|%10ld|%10ld|%10ld|%10ld|%10ld|%10ld|\n", i,
           time_push_array / counts, time_push_list / counts,
           time_pop_array / counts, time_pop_list / counts, size_array_stack((i - 1) * 2),
           size_list_stack(i));
    i--;
  }

  return 0;
}

int clear_input()
{
  int ch = 0;

  do
  {
    ch = getc(stdin);
  } while ((char)ch != '\n');

  return 0;
}
int wait_and_clear()
{
  printf("Нажмите любую клавишу, чтобы продолжить.\n");
  clear_input();
  system("clear");
  return 0;
}

void exit_handler()
{
  printf("До скорой встречи!\n");
  exit(0);
}

void print_menu()
{
  static struct
  {
    char *name;
    int (*action)(void);
  } actions[ACTIONS_COUNT] = {
      {.name = "Стек - массив", .action = NULL},
      {.name = "Распечатать весь стек", .action = print_array_stack},
      {.name = "Добавить элемент в стек", .action = push_array_stack_wrapper},
      {.name = "Вытащить элемент из стека", .action = pop_array_stack_wrapper},

      {.name = "Стек - список", .action = NULL},
      {.name = "Распечатать весь стек", .action = print_list_stack},
      {.name = "Добавить элемент в стек", .action = push_list_stack_wrapper},
      {.name = "Вытащить элемент из стека", .action = pop_list_stack_wrapper},

      {.name = "Сравнения", .action = NULL},
      {.name = "Провести анализ", .action = compare_methods},

  };

  array_stack = init_array_stack();
  list_stack = init_list_stack();

  while (1)
  {
    printf("Меню \n\n");

    size_t diff = 0;
    for (size_t i = 0; i < ACTIONS_COUNT; i++)
    {
      if (actions[i].action)
      {
        printf("%3zu) %s\n", i + 1 - diff, actions[i].name);
      }
      else
      {
        diff++;
        printf("===== %s\n", actions[i].name);
      }
    }

    printf("\n  0) Выход \n");

    printf("Введите номер команды: ");
    int action = 0;
    if (scanf("%d", &action) == 0 || action > ACTIONS_COUNT - diff ||
        action < 0)
    {
      wait_and_clear();
      printf("Неправильный номер команды.");
      return print_menu();
    }

    wait_and_clear();
    if (!action)
      return exit_handler();

    size_t i = 0;
    for (size_t j = 0; j < action; i++)
      if (actions[i].action)
        j++;

    if (actions[i - 1].action())
    {
      wait_and_clear();
      printf("Произошла ошибка\n");
    }

    wait_and_clear();
  }

  print_menu();
}