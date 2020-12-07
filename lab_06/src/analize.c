/**
 * @file analize.c
 * @author Vladimir Larin
 * @brief Analize module
 *
 * @copyright Copyright (c) 2020
 *
 */
#include "inc/analize.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "inc/avl_tree.h"
#include "inc/btree.h"
#include "inc/hash_funcs.h"
#include "inc/hash_table.h"
#include "inc/utils.h"

#define MAX_ARRAY_SIZE 1000000

static int counts = 1000;
extern char *filename_data_global;

int fill_array(char **array) {
  int size = 0;
  FILE *file = fopen(filename_data_global, "r");
  if (!file) return -1;
  while (size < MAX_ARRAY_SIZE && !feof(file)) {
    char *str = NULL;
    size_t n = 0;
    size_t len = getline(&str, &n, file);
    str[len - 1] = 0;
    if (len <= 1) {
      free(str);
      continue;
    }
    array[size] = str;
    size++;
  }
  fclose(file);

  return size;
}

void free_array(char **array, int size) {
  for (int i = 0; i < size; i++) free(array[i]);
}

int analyze_wrapper() {
  printf(
      "Анализ был проведен для добавления элемента в дерево. В таблице\n"
      "приведено среднее значения для %d повторений \n\n",
      counts);

  char *array[MAX_ARRAY_SIZE] = {0};
  int size = fill_array(array);

  analyze_report_t reports[3] = {0};

  size_t hash_table_size = next_prime(size * 0.85);

  for (size_t i = 0; i < counts; i++) {
    btree_t *btree = init_btree(strcmp_connector);
    avl_tree_t *avl_tree = init_avl_tree(strcmp_connector);
    hash_table_t *table =
        init_hash_table(hash_table_size, hash_sum, strcmp_connector);

    clock_t start = clock();
    for (size_t i = 0; i < size; i++)
      add_btree_cmps(btree, strdup(array[i]), &(reports[0].cmps));
    reports[0].time += clock() - start;

    start = clock();
    for (size_t i = 0; i < size; i++)
      add_avl_tree_cmps(avl_tree, strdup(array[i]), &(reports[1].cmps));
    reports[1].time += clock() - start;

    start = clock();
    for (size_t i = 0; i < size; i++)
      add_element_hash_table_cmps(table, strdup(array[i]), &(reports[2].cmps));
    reports[2].time += clock() - start;

    btree_free(btree);
    free_avl_tree(avl_tree);
    free_hash_table(table);
  }
  free_array(array, size);

  printf("### ДДП\n");
  printf("Время добавление одного элемента: %lf тиков\n",
         1. * reports[0].time / (counts * size));
  printf("Время построения дерева: %lf тиков\n", 1. * reports[0].time / counts);
  printf("Кол-во сравнений при добавлении одного элемента: %lf\n",
         1. * reports[0].cmps / (counts * size));
  printf("Занимаемая память: %zu Байт\n",
         sizeof(btree_node_t) * size + sizeof(btree_t));

  printf("\n");
  printf("### АВЛ дерево\n");
  printf("Время добавление одного элемента: %lf тиков\n",
         1. * reports[1].time / (counts * size));
  printf("Время построения дерева: %lf тиков\n", 1. * reports[1].time / counts);
  printf("Кол-во сравнений при добавлении одного элемента: %lf\n",
         1. * reports[1].cmps / (counts * size));
  printf("Занимаемая память: %zu Байт\n",
         sizeof(avl_tree_node_t) * size + sizeof(avl_tree_t));

  printf("\n");
  printf("### Хэш таблица\n");
  printf("Время добавление одного элемента: %lf тиков\n",
         1. * reports[2].time / (counts * size));
  printf("Время построения дерева: %lf тиков\n", 1. * reports[2].time / counts);
  printf("Кол-во сравнений при добавлении одного элемента: %lf\n",
         1. * reports[2].cmps / (counts * size));
  printf("Занимаемая память: %zu Байт\n",
         sizeof(hash_table_node_t *) * hash_table_size + sizeof(hash_table_t) +
             sizeof(hash_table_node_t) * size);

  return 0;
}

int analyze_has_wrapper() {
  printf(
      "Анализ был проведен для поиска элемента в дереве. В таблице\n"
      "приведено среднее значения для %d повторений \n\n",
      counts);

  char *array[MAX_ARRAY_SIZE] = {0};
  int size = fill_array(array);

  analyze_report_t reports[4] = {0};

  size_t hash_table_size = next_prime(size * 0.85);

  btree_t *btree = init_btree(strcmp_connector);
  avl_tree_t *avl_tree = init_avl_tree(strcmp_connector);
  hash_table_t *table =
      init_hash_table(hash_table_size, hash_sum, strcmp_connector);

  for (size_t i = 0; i < size; i++) {
    add_avl_tree(avl_tree, strdup(array[i]));
    add_btree(btree, strdup(array[i]));
    add_element_hash_table(table, strdup(array[i]));
  }

  for (size_t i = 0; i < counts; i++) {
    clock_t start = clock();
    for (size_t i = 0; i < size; i++)
      has_element_btree_cmps(btree, array[i], &(reports[0].cmps));
    reports[0].time += clock() - start;

    start = clock();
    for (size_t i = 0; i < size; i++)
      has_avl_tree_cmps(avl_tree, array[i], &(reports[1].cmps));
    reports[1].time += clock() - start;

    start = clock();
    for (size_t i = 0; i < size; i++)
      has_element_hash_table_cmps(table, array[i], &(reports[2].cmps));
    reports[2].time += clock() - start;
  }
  btree_free(btree);
  free_avl_tree(avl_tree);
  free_hash_table(table);

  free_array(array, size);

  printf("### ДДП\n");
  printf("Время поиска одного элемента: %lf тиков\n",
         1. * reports[0].time / (counts * size));
  printf("Кол-во сравнений при поиска одного элемента: %lf\n",
         1. * reports[0].cmps / (counts * size));
  printf("Занимаемая память: %zu Байт\n",
         sizeof(btree_node_t) * size + sizeof(btree_t));

  printf("\n");
  printf("### АВЛ дерево\n");
  printf("Время поиска одного элемента: %lf тиков\n",
         1. * reports[1].time / (counts * size));
  printf("Кол-во сравнений при поиска одного элемента: %lf\n",
         1. * reports[1].cmps / (counts * size));
  printf("Занимаемая память: %zu Байт\n",
         sizeof(avl_tree_node_t) * size + sizeof(avl_tree_t));

  printf("\n");
  printf("### Хэш таблица\n");
  printf("Время поиска одного элемента: %lf тиков\n",
         1. * reports[2].time / (counts * size));
  printf("Кол-во сравнений при поиска одного элемента: %lf\n",
         1. * reports[2].cmps / (counts * size));
  printf("Занимаемая память: %zu Байт\n",
         sizeof(hash_table_node_t *) * hash_table_size + sizeof(hash_table_t) +
             sizeof(hash_table_node_t) * size);

  return 0;
}
