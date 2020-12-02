#ifndef LIST_STACK_H_
#define LIST_STACK_H_

#include "stack.h"

typedef struct node {
  struct node* next;
  stack_type_t value;
} node_t;

typedef struct list_stack {
  node_t* list;
  node_t* free_nodes;
} list_stack_t;

list_stack_t* init_list_stack();

node_t* create_node();

int push_list_stack(list_stack_t* stack);

int pop_list_stack(list_stack_t* stack, stack_type_t* element);

void free_list_stack(list_stack_t* stack);

#endif  // LIST_STACK_H_
