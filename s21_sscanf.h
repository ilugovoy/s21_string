#ifndef S21_SSCANF
#define S21_SSCANF

#include "s21_string.h"

typedef struct {
  bool is_length_L;
  bool is_length_l;
  bool is_length_h;
  bool is_unsigned;
  bool is_width;
  bool is_hex;
  bool is_oct;
  int width;
} Parameters_sscanf;

void process_width_sscanf(const char **format, Parameters_sscanf *settings,
                          va_list args);
void process_length_sscanf(const char **format, Parameters_sscanf *settings);
void parameters_init_sscanf(Parameters_sscanf *settings);
void skip_spaces(const char **str);

bool is_int_sym(char sym);
bool is_oct_sym(char sym);
bool is_hex_sym(char sym);

void process_s_sscanf(const char **str, const Parameters_sscanf *settings,
                      va_list *args);
void process_d_sscanf(const char **str, const Parameters_sscanf *settings,
                      va_list *args);
void process_f_sscanf(const char **str, const Parameters_sscanf *settings,
                      va_list *args);
void process_i_sscanf(const char **str, Parameters_sscanf *settings,
                      va_list *args);
void process_n_sscanf(const char **str_start, const char **str, va_list *args);
void process_p_sscanf(const char **str, va_list *args);
void process_c_sscanf(const char **str, va_list *args);

#endif