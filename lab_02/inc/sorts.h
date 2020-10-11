#ifndef SORTS_H
#define SORTS_H

#include <stdlib.h>

void selection_sort(void *array, size_t nmemb, size_t size, int (*cmp)(const void *,const void *));
void shuffle(void *array, size_t n, size_t size);
#endif // SORTS_H