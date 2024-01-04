#include "../s21_sscanf.h"

#include "../s21_string.h"

int s21_sscanf(const char *str, const char *format, ...) {
  if (str == NULL || format == NULL) {
    return -1;
  }

  Parameters_sscanf settings;
  parameters_init_sscanf(&settings);

  va_list args;
  va_start(args, format);

  const char *str_start = str;

  while (*format != '\0') {
    if (*format == '%') {
      format++;
      process_length_sscanf(&format, &settings);
      process_width_sscanf(&format, &settings, args);
      if (*format == '%') {
        str++;
      }
      if (*format == 's') process_s_sscanf(&str, &settings, &args);
      if (*format == 'c') process_c_sscanf(&str, &args);
      if (*format == 'd') process_d_sscanf(&str, &settings, &args);
      if (*format == 'i') process_i_sscanf(&str, &settings, &args);
      if (*format == 'u') {
        settings.is_unsigned = true;
        process_d_sscanf(&str, &settings, &args);
      }
      if (*format == 'x' || *format == 'X') {
        settings.is_hex = true;
        process_d_sscanf(&str, &settings, &args);
      }
      if (*format == 'o') {
        settings.is_oct = true;
        process_d_sscanf(&str, &settings, &args);
      }
      if (*format == 'e' || *format == 'E' || *format == 'f' ||
          *format == 'g' || *format == 'G')
        process_f_sscanf(&str, &settings, &args);
      if (*format == 'n') process_n_sscanf(&str_start, &str, &args);
      if (*format == 'p') process_p_sscanf(&str, &args);
      skip_spaces(&str);
    }

    format++;
    parameters_init_sscanf(&settings);
  }

  va_end(args);
  return 0;
}

void parameters_init_sscanf(Parameters_sscanf *settings) {
  settings->is_length_L = false;
  settings->is_length_l = false;
  settings->is_length_h = false;
  settings->is_unsigned = false;
  settings->is_hex = false;
  settings->is_oct = false;
  settings->is_width = false;

  settings->width = 0;
}

void process_length_sscanf(const char **format, Parameters_sscanf *settings) {
  if (**format == 'L') {
    (*format)++;
    settings->is_length_L = true;
  } else if (**format == 'l') {
    (*format)++;
    settings->is_length_l = true;
  } else if (**format == 'h') {
    (*format)++;
    settings->is_length_h = true;
  }
}

void process_width_sscanf(const char **format, Parameters_sscanf *settings,
                          va_list args) {
  if (**format == '*') {
    settings->is_width = true;
    int width_arg = va_arg(args, int);
    settings->width = width_arg;
    (*format)++;
  } else if (isdigit(**format)) {
    settings->is_width = true;
    settings->width = atoi(*format);
    while (isdigit(**format)) {
      (*format)++;
    }
  }
}

void skip_spaces(const char **str) {
  while (true) {
    if (**str == '\0') {
      break;
    }
    if (**str != ' ' && **str != '\t') {
      break;
    }
    (*str)++;
  }
}

bool is_int_sym(char sym) {
  if (!isdigit(sym) && sym != '-') {
    return false;
  }
  return true;
}

bool is_oct_sym(char sym) {
  if ((48 <= sym && sym <= 55) || sym == '-') {
    return true;
  }
  return false;
}

bool is_hex_sym(char sym) {
  if (isdigit(sym) || sym == '-' || (65 <= sym && sym <= 70) ||
      (97 <= sym && sym <= 102)) {
    return true;
  }
  return false;
}

void process_c_sscanf(const char **str, va_list *args) {
  char *c = va_arg(*args, char *);
  *c = **str;
  (*str)++;
}

void process_s_sscanf(const char **str, const Parameters_sscanf *settings,
                      va_list *args) {
  char *s = va_arg(*args, char *);

  const char *p_to_start = *str;
  const char *p_to_end = NULL;
  int width = 0;

  while (!(p_to_end)) {
    if (settings->is_width && settings->width == width) {
      p_to_end = *str;
      break;
    } else if (**str == ' ' || **str == '\t' || **str == '\n') {
      p_to_end = *str;
      break;
    }

    (*str)++;
    width++;
  }

  s21_strncpy(s, p_to_start, p_to_end - p_to_start);
  s[p_to_end - p_to_start] = '\0';
}

void process_d_sscanf(const char **str, const Parameters_sscanf *settings,
                      va_list *args) {
  const char *p_to_start = *str;
  const char *p_to_end = NULL;
  int width = 0;

  while (!(p_to_end) && **str != '\n') {
    if (settings->is_width && settings->width == width) {
      p_to_end = *str;
      break;
    }
    if (settings->is_hex) {
      if (!is_hex_sym(**str)) {
        p_to_end = *str;
        break;
      }
    } else if (settings->is_oct) {
      if (!is_oct_sym(**str)) {
        p_to_end = *str;
        break;
      }
    } else {
      if (!is_int_sym(**str)) {
        p_to_end = *str;
        break;
      }
    }
    (*str)++;
    width++;
  }
  if (**str == '\n') p_to_end = *str;
  char s[p_to_end - p_to_start + 1];
  s21_strncpy(s, p_to_start, p_to_end - p_to_start);
  s[p_to_end - p_to_start] = '\0';

  if (settings->is_unsigned) {
    unsigned int *d = va_arg(*args, unsigned int *);
    if (settings->is_hex)
      *d = (unsigned int)strtol(s, NULL, 16);
    else if (settings->is_oct)
      *d = (unsigned int)strtol(s, NULL, 8);
    else
      *d = atoi(s);
  } else {
    int *d = va_arg(*args, int *);
    if (settings->is_hex)
      *d = strtol(s, NULL, 16);
    else if (settings->is_oct)
      *d = strtol(s, NULL, 8);
    else
      *d = atoi(s);
  }
}

void process_i_sscanf(const char **str, Parameters_sscanf *settings,
                      va_list *args) {
  const char *str_cpy = *str;
  if (*str_cpy == '0') {
    if (*(++str_cpy) == 'x') {
      settings->is_hex = true;
      (*str) += 2;
    } else {
      settings->is_oct = true;
      (*str)++;
    }
  }
  process_d_sscanf(str, settings, args);
}

void process_f_sscanf(const char **str, const Parameters_sscanf *settings,
                      va_list *args) {
  const char *p_to_start = *str;
  const char *p_to_end = NULL;
  int width = 0;

  while (!(p_to_end) && **str != '\n') {
    if (settings->is_width && settings->width == width) {
      p_to_end = *str;
      break;
    }

    if (!isdigit(**str) && **str != '-' && **str != '+' && **str != '.' &&
        **str != 'e' && **str != 'E') {
      p_to_end = *str;
      break;
    }

    (*str)++;
    width++;
  }

  if (**str == '\n') p_to_end = *str;

  char s[p_to_end - p_to_start + 1];
  s21_strncpy(s, p_to_start, p_to_end - p_to_start);
  s[p_to_end - p_to_start] = '\0';

  float *f = va_arg(*args, float *);
  *f = atof(s);
}

void process_n_sscanf(const char **str_start, const char **str, va_list *args) {
  int *d = va_arg(*args, int *);
  *d = s21_strlen(*str_start) - s21_strlen(*str);
}

void process_p_sscanf(const char **str, va_list *args) {
  char s[12];
  for (int i = 0; i < 11; i++) {
    if (**str == '\0') {
      break;
    }
    s[i] = **str;
    (*str)++;
  }
  s[11] = '\0';

  void **a = va_arg(*args, void **);
  *a = (void *)strtoull(s, NULL, 16);
}
