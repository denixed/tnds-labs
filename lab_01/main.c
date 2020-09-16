#include <stdio.h>
#include "long_nums.h"

/**
 * @brief 
 * 
 * @return int error code
 */
int main()
{
    int err = 0;
    char number_buffer[255] = { 0 };
    printf("               123456789012345678901234567890\n");
    printf("Введите целое: ");
    scanf("%s", number_buffer);

    long_int_t number = { 0 };
    long_real_t number_1 = { 0 };
    long_real_t number_2 = { 0 };

    err = input_long_int(number_buffer, &number);
    
    if (!err)
        long_int_to_long_real(&number_1, &number);
    if (!err)
    {
        printf("                        123456789012345678901234567890\n");
        printf("Введите действительное: ");
        scanf("%s", number_buffer);
        err = input_long_real(number_buffer, &number_2);
    }
    
    long_real_t result = { 0 };
    
    if (!err)
    {
        err = div_long_real(&number_1, &number_2, &result);
    }

    if (!err)
    {
        printf("Результат деления: ");
        err = fprint_long_real(&result, stdout);
        printf("\n");
    }    

    if (err)
        printf("Произошла ошибка");

    return err;
}
