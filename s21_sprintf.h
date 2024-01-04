#ifndef S21_SPRINTF
#define S21_SPRINTF

#include "s21_string.h"

#define S21_MAX_INF ((1.0f) / (0.0f))
#define S21_MIN_INF ((-1.0f) / (0.0f))
#define MAX_DIGITS_IN_FRACTION (64)

typedef struct {
  bool is_leading_zero;
  bool is_sharp_flag;
  bool is_space_flag;
  bool is_minus_flag;
  bool gG_to_eE_flag;
  bool is_plus_flag;
  bool is_negative;
  bool is_length_L;
  bool is_length_l;
  bool is_length_h;
  bool is_accuracy;
  bool uppercase_x;
  bool uppercase_g;
  bool uppercase_e;
  bool is_pointer;
  bool is_width;
  bool is_gG;
  bool is_eE;
  bool flag;
  int space_num;
  int written;
  int accuracy;
  int width;
  int order;
} Parameters;

// Process flags and parameters
void process_flags(const char **format, Parameters *settings);
void process_length(const char **format, Parameters *settings);
void process_width(const char **format, Parameters *settings, va_list args);
void process_accuracy(const char **format, Parameters *settings, va_list args);
void process_shielding_sign(char **str, const char **format,
                            Parameters *settings);

// Process specifiers
void process_d(va_list args, char *str, Parameters *settings);
void process_u(va_list args, char *str, Parameters *settings);
void process_f(va_list args, char *str, Parameters *settings);
void process_c(va_list args, char *str, Parameters *settings);
void process_s(va_list args, char *str, Parameters *settings);
void process_o(va_list args, char *str, Parameters *settings);
void process_p(va_list args, char *str, Parameters *settings);
void process_xX(va_list args, char *str, Parameters *settings);
void process_gG(va_list args, char *str, Parameters *settings);
void process_eE(va_list args, char *str, Parameters *settings);

// Auxiliary functions
void reverse_str(char *str, int i);
void parameters_init(Parameters *settings);
int count_divisions_by_10(long double value);
char *s21_strcpy(char *destination, const char *source);
int create_str_from_integers(char *str, int i, Parameters *settings);

// Function for converting an unsigned integer to a string
int unsigned_int_to_string(char *str, unsigned int value, Parameters *settings);
int unsigned_long_int_to_string(char *str, unsigned long int value,
                                Parameters *settings);
int unsigned_short_int_to_string(char *str, unsigned short int value,
                                 Parameters *settings);

// Functions for converting int numbers to string
int int_to_string(char *str, int value, Parameters *settings);
int long_int_to_string(char *str, long int value, Parameters *settings);
int short_int_to_string(char *str, short int value, Parameters *settings);

// Functions for converting float numbers to string
int float_to_string(char *str, long double value, Parameters *settings);
int eE_float_to_string(char *str, long double value, Parameters *settings);
void frac_part_to_string(char *str, int *i, int *digit_count,
                         long double *frac_part, Parameters *settings);

// Functions for converting hexadecimal %g to a string
int gG_float_to_string(char *str, long double value, Parameters *settings);

// Functions for converting hexadecimal and octal numbers to a string
int hex_to_string(char *str, unsigned long int value, Parameters *settings);
int oct_to_string(char *str, unsigned long int value, Parameters *settings);

// Helpful functions
void parameters_reset(Parameters *settings);
int check_infinite(char *str, long double value);
void add_spaces_to_d(char *str, Parameters *settings);
void check_minus_flag(char *str, Parameters *settings);
void add_decimal_point(char *str, int *i, const Parameters *settings);
void check_negative_numbers(char *str, int *i, long double *value);
void order_determine(long double *value_temp, Parameters *settings);
void process_gG_to_eE(const char *str, int *i, Parameters *settings);
void adding_zeros(char *str, int *i, int digit_count,
                  const Parameters *settings);
void check_spaces_and_zeros_and_plus(char *str, int *i, long double value,
                                     Parameters *settings);
void start_eE_from_gG(char *str, int *i, long double value,
                      long double value_temp, Parameters *settings);

#endif