#include "array_stack.h"

array_stack_t* init_array_stack() {
  array_stack_t* stack = calloc(sizeof(array_stack_t), 1);
  stack->capacity = START_ARRAY_CAPACITY;
  stack->end = stack->buffer = calloc(sizeof(stack_type_t), stack->capacity);
  return stack;
}

int resize_array_stack(array_stack_t* stack, size_t capacity) {
  size_t end_position = stack->end - stack->buffer;
  stack->buffer = realloc(stack->buffer, capacity * sizeof(stack_type_t));
  stack->end = stack->buffer + end_position;
  stack->capacity = capacity;
  return 0;
}

int push_array_stack(array_stack_t* stack, stack_type_t element) {
  if (stack->end - stack->buffer == ARRAY_MAX_CAPCITY)
    return 1;
  if (stack->capacity <= stack->end - stack->buffer)
    resize_array_stack(stack, stack->capacity * 2);

  *(stack->end) = element;
  (stack->end)++;

  return 0;
}

int pop_array_stack(array_stack_t* stack, stack_type_t* element) {
  if (stack->buffer == stack->end) return 1;
  (stack->end)--;
  *element = *(stack->end);
  return 0;
}

void free_array_stack(array_stack_t* stack) {
  if (!stack) return;

  free(stack->buffer);
  free(stack);
}