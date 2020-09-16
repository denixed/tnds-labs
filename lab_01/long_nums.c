#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "long_nums.h"
/**
 * @brief Ввод длинного вещественного
 * 
 * @param string 
 * @param number 
 * @return int 
 */
int input_long_real(char *string, long_real_t *number)
{
    char *mantis_end = NULL; 
    int dot_pos = 0;
    number->exp = 0;
    int err = parse_mantis(string, &(number->mantis), &mantis_end, &dot_pos);
    if (err)
        return err;

    if (*mantis_end)
    {
        mantis_end++;
        char temp = 0; 
        int rc = sscanf(mantis_end, "%d%c", &(number->exp), &temp);
        if (rc != 1 || temp)
            return INPUT_ERROR;
    }

    number->exp += dot_pos - 1;

    return normalize_long_real(number);

}

/**
 * @brief Очистка мантисы
 * 
 * @param number 
 * @param DIGITS_LEN 
 * @return int 
 */
int clean_mantis(long_int_t *number, size_t DIGITS_LEN)
{
    if (!number)
        return POINTER_ERROR;
    
    for (size_t i = 0; i < DIGITS_LEN; i++)
        number->digits[i] = 0;
    
    return 0;    
}

/**
 * @brief Обработка мантисы
 * 
 * @param string 
 * @param number 
 * @param mantis_end 
 * @param dot_pos 
 * @return int 
 */
int parse_mantis(char *string, long_int_t *number, char **mantis_end, int *dot_pos)
{
    int dots_count = 0;
    int dot_position = 0;
    int input_len = 0;

    int non_zero_starts = 0;
    number->sign = POSITIVE_SIGN;

    int err = clean_mantis(number, MANTIS_LENGTH);
    if (err)
        return err;

    if (*string == '-' || *string == '+')
    {
        if(*string == '-')
            number->sign = NEGATIVE_SIGN;
        string++;
    }

    
    while (*string && !strchr("eE", *string) && (input_len - dots_count) < MANTIS_LENGTH - 1)
    {
        if (isdigit(*string))
        {
            number->digits[input_len - dots_count] = *string - '0';
            if (!non_zero_starts && number->digits[input_len - dots_count])
                non_zero_starts = input_len;
        }
        else if (*string == '.')
        {
            dots_count++;
            dot_position = input_len;
        }
        else
            return INPUT_ERROR;
        
        input_len++;
        string++; 
    }
    if (!dots_count)
        dot_position = input_len;

    if (dots_count > 1)
        return INPUT_ERROR;
    
    if (*string && !strchr("eE", *string))
        return INPUT_ERROR;    

    
    *mantis_end = string;
    if (dot_position < non_zero_starts)
        non_zero_starts = dot_position;
    *dot_pos = dot_position;

    return 0;
}

/**
 * @brief Ввод длинного целового
 * 
 * @param string 
 * @param number 
 * @return int 
 */
int input_long_int(char *string, long_int_t *number)
{
    size_t input_len = 0;
    size_t digits_len = 0;
    
    int err = 0;
    
    if (!string || !number)
        err = POINTER_ERROR;
    
    if (!err)
        number->sign = POSITIVE_SIGN; 

    if ( (!err && *string == '-') || *string == '+')
    {
        if(*string == '-')
            number->sign = NEGATIVE_SIGN;
        string++;
    }

    if (!err)
        digits_len = strlen(string);
    if (!err && ( digits_len > 30 || !digits_len ))
        err = INPUT_ERROR;
    
    if (!err)
        for (size_t i = 0; i < MANTIS_LENGTH - digits_len; i++)
            number->digits[i] = 0;
        
    while (!err && *string)
    {
        if (isdigit(*string))
            number->digits[MANTIS_LENGTH - digits_len + input_len] = *string - '0';
        else
            err = INPUT_ERROR;
        
        input_len++;
        string++; 
    }

    return err;

}

/**
 * @brief Печать длинного целового
 * 
 * @param number 
 * @param file 
 * @return int 
 */
int fprint_long_int(long_int_t *number, FILE *file)
{
    if (!number || !file)
        return POINTER_ERROR;
    
    if (number->sign == NEGATIVE_SIGN)
        fprintf(file, "-");
    
    size_t i = 0;

    while (i < MANTIS_LENGTH && !number->digits[i])
        i++;

    if (i == MANTIS_LENGTH)
        fprintf(file, "0");
        
    for (; i < MANTIS_LENGTH; i++)
        fprintf(file, "%d", number->digits[i]);
    
    return 0;  
}

/**
 * @brief Печать длинного вещественного
 * 
 * @param number 
 * @param file 
 * @return int 
 */
int fprint_long_real(long_real_t *number, FILE *file)
{
    if (!number || !file)
        return POINTER_ERROR;

    long_int_t zero = { 0 };

    if (!cmp_long_int(&zero, &(number->mantis)))
    {
        fprintf(file, "0");
        return 0;
    }
    int err = normalize_long_real(number);

    if (abs(number->exp) > MAX_EXP)
        return EXPONENT_ERROR;

    if (number->mantis.sign == NEGATIVE_SIGN)
        fprintf(file, "-");
    
    size_t i = 1;

    fprintf(file, "0.");
        
    for (; i < MANTIS_LENGTH; i++)
        fprintf(file, "%d", number->mantis.digits[i]);
    
    fprintf(file, "e%d" , number->exp);

    return err;  
}

/**
 * @brief Преобразование целового длинного к длинному вещественному
 * 
 * @param lreal 
 * @param lint 
 * @return int 
 */
int long_int_to_long_real(long_real_t *lreal,const long_int_t *lint)
{
    if (!lreal || !lint)
        return POINTER_ERROR;
    
    
    lreal->mantis.sign = lint->sign;
    
    for (size_t i = 0; i < MANTIS_LENGTH; i++)
        lreal->mantis.digits[i] = lint->digits[i];
    
    int shift = 0; 
    int err = mantis_shift_left_side(&(lreal->mantis), &shift);

    lreal->exp = MANTIS_LENGTH - shift - 1; 

    return err;
}

/**
 * @brief Сдвиг мантисы к левому краю
 * 
 * @param number 
 * @param shift 
 * @return int 
 */
int mantis_shift_left_side(long_int_t *number, int *shift)
{
    *shift = 0;

    while (!number->digits[*shift] && *shift < MANTIS_LENGTH - 1)
        (*shift)++;
    (*shift)--;
    
    if (*shift >= MANTIS_LENGTH - 1)
        return 0;

    return mantis_shift(number, shift);
}

/**
 * @brief Сдвиг мантисы на определенное количество разрядов
 * 
 * @param number 
 * @param shift 
 * @return int 
 */
int mantis_shift(long_int_t *number, int *shift)
{
    if (!number || !shift)
        return POINTER_ERROR;
    if (!(*shift))
        return 0;
    
    if (*shift > 0)
    {
        for (int i = 0; i < MANTIS_LENGTH - *shift; i++)
            number->digits[i] = number->digits[*shift + i];
        
        for (int i = 0; i < *shift ; i++)
            number->digits[MANTIS_LENGTH - 1 - i] = 0;
    }else
    {
        *shift = -*shift;
        
        if (number->digits[MANTIS_LENGTH - *shift] >= 5)
        {
            number->digits[MANTIS_LENGTH - *shift - 1]++; 
            for (size_t i = MANTIS_LENGTH - *shift - 1; i > 0 && number->digits[i] > 9; i--)
            {
                   number->digits[i - 1]++;
                   number->digits[i] -= 10;
            }
        }


        for (int i = 0; i < MANTIS_LENGTH - *shift; i++)
            number->digits[MANTIS_LENGTH - i - 1] = number->digits[MANTIS_LENGTH - *shift - i - 1 ];
        
        for (int i = 0; i < *shift ; i++)
            number->digits[i] = 0;

        if (number->digits[*shift] > 9)
        {
            number->digits[*shift] -= 10;
            number->digits[*shift - 1]++;
        }

        *shift = -*shift;
    }
    
    return 0;
    
}
/**
 * @brief Нормализация числа
 * 
 * @param number 
 * @return int 
 */
int normalize_long_real(long_real_t *number)
{
    int shift = 0;

    mantis_shift_left_side(&(number->mantis), &shift);
    number->exp -= shift;

    return 0;
}

/**
 * @brief Добавление к одному длинному целому другое
 * 
 * @param first 
 * @param second 
 * @return int 
 */
int add_long_int(long_int_t *first, const long_int_t *second)
{
    for (int i = MANTIS_LENGTH - 1; i > 0; i--)
    {
        first->digits[i] += second->digits[i];
        if (first->digits[i] > 9)
        {
            first->digits[i] -= 10;
            first->digits[i - 1]++;
        }
    }   
    return 0;
}

/**
 * @brief Вычитание из одного длинного целового другое 
 * 
 * @param first 
 * @param second 
 * @return int 
 */
int sub_long_int(long_int_t *first, const long_int_t *second)
{
    for (int i = MANTIS_LENGTH - 1; i >= 0; i--)
    {
        if (first->digits[i] < second->digits[i])
        {
            first->digits[i] += 10;
            first->digits[i - 1]--;
        }
        first->digits[i] -= second->digits[i]; 
    }   
    return 0;
}

/**
 * @brief Сравнение длинные целых
 * 
 * @param first 
 * @param second 
 * @return int 0 -если равны, >0 - если первое больше, иначе <0  
 */
int cmp_long_int(const long_int_t *first, const long_int_t *second)
{
    for (size_t i = 0; i < MANTIS_LENGTH; i++)
    {
        int diff = first->digits[i] - second->digits[i];
        if (diff)
            return diff;
    }
    return 0;
}

/**
 * @brief Деление длинных вещественных чисел
 * 
 * @param first 
 * @param second 
 * @param result 
 * @return int 
 */
int div_long_real(const long_real_t *first, const long_real_t *second, long_real_t *result)
{
    long_int_t zero = { 0 };

    if (!cmp_long_int(&zero, &(second->mantis)))
        return ZERO_DIVISION_ERR;
    
    if (!cmp_long_int(&zero, &(first->mantis)))
    {
        result->mantis.sign = POSITIVE_SIGN;
        result->exp = 0;
        clean_mantis(&(result->mantis), MANTIS_LENGTH);
        return 0;
    }

    long_int_t work_num = first->mantis;

    result->mantis.sign = POSITIVE_SIGN;

    if (first->mantis.sign != second->mantis.sign)
        result->mantis.sign = NEGATIVE_SIGN;
    
    result->exp = first->exp - second->exp;

    if (cmp_long_int(&(second->mantis), &(work_num)) > 0)
    {
        result->exp--;
        int shift = 1;
        mantis_shift(&work_num, &shift);
    }    
    

    for (size_t i = 0; i < MANTIS_LENGTH; i++)
    {
        long_int_t temp;
        clean_mantis(&temp, MANTIS_LENGTH);
        result->mantis.digits[i] = 0;
        while (cmp_long_int(&temp, &(work_num)) <= 0 ) 
        {
            add_long_int(&temp, &(second->mantis));
            result->mantis.digits[i]++;
        }     
        result->mantis.digits[i]--;
        sub_long_int(&temp, &(second->mantis));
        
        sub_long_int(&work_num, &temp);

        int shift = 1;
        mantis_shift(&work_num, &shift);
    }

    return normalize_long_real(result);    
}