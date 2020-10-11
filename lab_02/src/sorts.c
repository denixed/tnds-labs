#include "sorts.h"
#include <string.h>

/**
 * @brief Swap any elements
 * 
 * @param first element
 * @param second element
 * @param size size of one element
 */
static void swap(void *first, void *second, void *dop_memory, size_t size)
{
    memcpy(dop_memory, first, size);
    memcpy(first, second, size);
    memcpy(second, dop_memory, size);  
}

/**
 * @brief Get the position of lowerest element
 * 
 * @param arr_begin begin pointer to array
 * @param arr_end end pointer to array
 * @param size size of one array element
 * @param cmp comparision funtion
 * @return void* pointer to found element
 */
static void *get_min_pos(void *arr_begin, void *arr_end, size_t size, int (*cmp)(const void *,const  void *))
{
    void *min_ptr = arr_begin;
    for (char *i_ptr = (char *)arr_begin + size; (void *)i_ptr < arr_end; i_ptr += size)
        if (cmp(i_ptr, min_ptr) < 0)
            min_ptr = i_ptr;   
    
    return min_ptr;    
}


void selection_sort(void *array, size_t nmemb, size_t size, int (*cmp)(const void *,const  void *))
{
    char dop_memory[size];

    char *arr_end = (char *)array + nmemb * size;

    for (char *i_ptr = array; i_ptr < arr_end; i_ptr += size)
    {
        void *min_pos = get_min_pos(i_ptr, arr_end, size, cmp);
        swap(i_ptr, min_pos,dop_memory, size);
    }
}

void shuffle(void *array, size_t n, size_t size) {
    char tmp[size];
    char *arr = array;
    size_t stride = size * sizeof(char);

    if (n > 1) {
        size_t i;
        for (i = 0; i < n - 1; ++i) {
            size_t rnd = (size_t) rand();
            size_t j = i + rnd / (RAND_MAX / (n - i) + 1);

            memcpy(tmp, arr + j * stride, size);
            memcpy(arr + j * stride, arr + i * stride, size);
            memcpy(arr + i * stride, tmp, size);
        }
    }
}