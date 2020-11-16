#include "list_stack.h"

#include <stdlib.h>

list_stack_t* init_list_stack() {
  list_stack_t* stack = calloc(sizeof(list_stack_t), 1);
  return stack;
}

node_t* create_node() {
  node_t* node = calloc(sizeof(node_t), 1);
  node->value = node;

  return node;
}

int push_list_stack(list_stack_t* stack) {
  node_t* node = stack->free_nodes;

  if (node)
    stack->free_nodes = stack->free_nodes->next;
  else
    node = create_node();

  node->next = stack->list;
  stack->list = node;

  return 0;
}

int pop_list_stack(list_stack_t* stack, stack_type_t* element) {
  if (!stack->list) return 1;

  node_t* node = stack->list;

  *element = node->value;

  stack->list = node->next;

  // free(node);
  node->next = stack->free_nodes;
  stack->free_nodes = node;

  return 0;
}

void free_list_stack(list_stack_t* stack) {
  if (!stack) return;
  node_t* cur;

  cur = stack->list;
  while (cur) {
    node_t* l = cur;
    cur = cur->next;
    free(l);
  }

  cur = stack->free_nodes;
  while (cur) {
    node_t* l = cur;
    cur = cur->next;
    free(l);
  }

  free(stack);
}
