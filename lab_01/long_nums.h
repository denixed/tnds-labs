#ifndef _LONG_NUMS_H_
#define _LONG_NUMS_H_

#define MANTIS_LENGTH 31
#define MAX_EXP 99999

#include <inttypes.h>
#include <stdbool.h>


typedef struct 
{
    bool sign;
    int8_t digits[MANTIS_LENGTH];
} long_int_t;

typedef struct 
{
    long_int_t mantis;
    int32_t exp;
} long_real_t;


#define POSITIVE_SIGN 1 
#define NEGATIVE_SIGN 0

#define POINTER_ERROR -1
#define INPUT_ERROR -2 
#define EXPONENT_ERROR -3 
#define ZERO_DIVISION_ERR -4

int input_long_real(char *string, long_real_t *number);
int clean_mantis(long_int_t *number, size_t DIGITS_LEN);
int parse_mantis(char *string, long_int_t *number, char **mantis_end, int *dot_pos);
int input_long_int(char *string, long_int_t *number);
int fprint_long_int(long_int_t *number, FILE *file);
int fprint_long_real(long_real_t *number, FILE *file);
int long_int_to_long_real(long_real_t *lreal,const long_int_t *lint);
int mantis_shift_left_side(long_int_t *number, int *shift);
int mantis_shift(long_int_t *number, int *shift);
int normalize_long_real(long_real_t *number);
int div_long_real(const long_real_t *first, const long_real_t *second, long_real_t *result);
int cmp_long_int(const long_int_t *first, const long_int_t *second);



#endif // !_LONG_NUMS_H_