#include <stdlib.h>

#include "stack.h"

#define START_ARRAY_CAPACITY 2

typedef struct array_stack {
  size_t capacity;
  stack_type_t* end;
  stack_type_t* buffer;
} array_stack_t;

array_stack_t* init_array_stack();

int push_array_stack(array_stack_t* stack, stack_type_t element);

int pop_array_stack(array_stack_t* stack, stack_type_t* element);

void free_array_stack(array_stack_t* stack);
