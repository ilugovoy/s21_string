#include "../s21_sprintf.h"

#include "../s21_string.h"

// Основная функция s21_sprintf
int s21_sprintf(char *str, const char *format, ...) {
  if (str == NULL || format == NULL) return -1;  // обработка ошибок
  *str = '\0';  // инициализируем пустую строку

  Parameters settings;  // инициализируем структуру
  parameters_init(&settings);
  va_list args;  // инициализируем список переменных аргументов
  va_start(args, format);

  while (*format != '\0') {
    if (*format == '%') {
      format++;
      process_flags(&format, &settings);        // проверяем флаги
      process_width(&format, &settings, args);  // проверяем ширину
      process_accuracy(&format, &settings, args);  // проверяем .точность
      process_length(&format, &settings);  // проверяем длину
      process_shielding_sign(&str, &format,
                             &settings);  // проверяем экранирующий процент
      // проверяем спецификаторы и записываем строку
      if (*format == 'd')
        process_d(args, str, &settings);
      else if (*format == 'u')
        process_u(args, str, &settings);
      else if (*format == 'f')
        process_f(args, str, &settings);
      else if (*format == 'c')
        process_c(args, str, &settings);
      else if (*format == 's')
        process_s(args, str, &settings);
      else if (*format == 'p')
        process_p(args, str, &settings);
      else if (*format == 'o')
        process_o(args, str, &settings);
      else if (*format == 'g' || *format == 'G') {
        if (*format == 'G') settings.uppercase_g = true;
        process_gG(args, str, &settings);
      } else if (*format == 'e' || *format == 'E') {
        if (*format == 'E') settings.uppercase_e = true;
        process_eE(args, str, &settings);
      } else if (*format == 'x' || *format == 'X') {
        if (*format == 'X') settings.uppercase_x = true;
        process_xX(args, str, &settings);
      }
    } else {
      str[settings.written] =
          *format;  // копируем символ, если это не спецификатор
      settings.written++;
    }
    format++;
    parameters_reset(&settings);  // сбрасываем параметры для следующего цикла
  }
  str[settings.written] = '\0';  // завершаем терминирующим нулем
  va_end(args);
  return settings.written;
}

void parameters_init(Parameters *settings) {
  settings->is_leading_zero = false;
  settings->is_sharp_flag = false;
  settings->is_space_flag = false;
  settings->is_minus_flag = false;
  settings->gG_to_eE_flag = false;
  settings->is_plus_flag = false;
  settings->is_length_L = false;
  settings->is_length_l = false;
  settings->is_length_h = false;
  settings->is_accuracy = false;
  settings->is_negative = false;
  settings->uppercase_x = false;
  settings->uppercase_g = false;
  settings->uppercase_e = false;
  settings->is_pointer = false;
  settings->is_width = false;
  settings->is_gG = false;
  settings->is_eE = false;
  settings->flag = false;
  settings->space_num = 0;
  settings->written = 0;
  settings->accuracy = 6;
  settings->width = 0;
  settings->order = 0;
}

void parameters_reset(Parameters *settings) {
  settings->accuracy = 6;
  settings->is_leading_zero = false;
  settings->is_eE = false;
  settings->uppercase_g = false;
  settings->uppercase_e = false;
  settings->width = 0;
  settings->is_width = false;
}

// __________________________________________________________________________________________________________PROCESS
// FLAGS AND PARAMETERS

void process_flags(const char **format, Parameters *settings) {
  // Реализация обработки флагов форматирования
  if (**format == ' ') {
    settings->is_space_flag = true;
    (*format)++;
  }
  if (**format == '+') {
    settings->is_plus_flag = true;
    (*format)++;
  }
  if (**format == '-') {
    settings->is_minus_flag = true;
    (*format)++;
  }
  if (**format == '#') {
    settings->is_sharp_flag = true;
    (*format)++;
  }
}

void process_width(const char **format, Parameters *settings, va_list args) {
  // проверяем ширину
  if (**format == '*') {
    settings->is_width = true;
    int width_arg = va_arg(
        args,
        int);  // получаем значение точности из параметров переменного списка
    if (width_arg == '0') settings->is_leading_zero = true;
    settings->width =
        width_arg;  // устанавливаем точность из параметров переменного списка
    (*format)++;
  } else if (isdigit(**format)) {
    settings->is_width = true;
    if (**format == '0') {
      settings->is_leading_zero = true;
    }
    settings->width = atoi(*format);  // извлекаем ширину поля из строки формата
    while (isdigit(**format)) {
      (*format)++;  // пропускаем цифры ширины поля
    }
  }
  if (**format == '+') {
    settings->is_plus_flag = true;
    (*format)++;
  }
}

void process_accuracy(const char **format, Parameters *settings, va_list args) {
  // проверяем .точность
  if (**format == '.') {
    settings->is_accuracy = true;
    (*format)++;
    if (**format == '*') {
      int precision_arg = va_arg(
          args,
          int);  // получаем значение точности из параметров переменного списка
      settings->accuracy = precision_arg;  // устанавливаем точность из
                                           // параметров переменного списка
      (*format)++;
    } else {
      settings->accuracy =
          atoi(*format);  // извлекаем точность из строки формата
      while (isdigit(**format)) {
        (*format)++;  // пропускаем цифры ширины поля
      }
    }
  }
}

void process_length(const char **format, Parameters *settings) {
  // проверяем длину
  if (**format == 'l') {
    (*format)++;
    settings->is_length_l = true;
  } else if (**format == 'h') {
    (*format)++;
    settings->is_length_h = true;
  } else if (**format == 'L') {
    (*format)++;
    settings->is_length_L = true;
  }
}

void process_shielding_sign(char **str, const char **format,
                            Parameters *settings) {
  // проверяем экранирующий процент
  if (**format == '%') {
    (*str)[settings->written++] = **format;
  }
}

// __________________________________________________________________________________________________________PROCESS
// SPECIFIERS

void process_d(va_list args, char *str, Parameters *settings) {
  long int x = va_arg(args, long int);
  // проверяем флаги и длину
  if (settings->is_length_h) {
    x = (short)x;
  } else if (!settings->is_length_l) {
    x = (int)x;
  }

  if (settings->is_space_flag && x > 0) {
    str[settings->written++] = ' ';
  }
  int len = 0;
  if (settings->is_width && settings->width > 0) {
    if (settings->is_length_h) {
      len = short_int_to_string(str + settings->written, x, settings);
    } else if (settings->is_length_l) {
      len = long_int_to_string(str + settings->written, x, settings);
    } else
      len = int_to_string(str + settings->written, x, settings);
    settings->space_num = settings->width - len;
    if ((settings->is_plus_flag || settings->is_space_flag) && x >= 0)
      settings->space_num--;
    add_spaces_to_d(str, settings);  // записываем пробелы
  }
  if (settings->is_plus_flag && x >= 0) str[settings->written++] = '+';

  if (settings->is_negative) {
    str[settings->written++] = '-';
    settings->is_negative = false;
  }
  // записываем число
  if (settings->is_length_h) {
    len = short_int_to_string(str + settings->written, x, settings);
  } else if (settings->is_length_l) {
    len = long_int_to_string(str + settings->written, x, settings);
  } else {
    len = int_to_string(str + settings->written, x, settings);
  }
  settings->written += len;  // увеличиваем счетчик записанных символов на длину
                             // записанного числа
  check_minus_flag(str, settings);
}

void process_u(va_list args, char *str, Parameters *settings) {
  unsigned long int x = va_arg(args, unsigned long int);
  // проверяем флаги и длину
  if (settings->is_length_h) {
    x = (unsigned short)x;
  } else if (!settings->is_length_l) {
    x = (unsigned int)x;
  }

  int len = 0;
  if (settings->is_width && settings->width > 0) {
    if (settings->is_length_h) {
      len = unsigned_short_int_to_string(str + settings->written, x, settings);
    } else if (settings->is_length_l) {
      len = unsigned_long_int_to_string(str + settings->written, x, settings);
    } else {
      len = unsigned_int_to_string(str + settings->written, x, settings);
    }

    settings->space_num = settings->width - len;
    if (settings->is_plus_flag) settings->space_num--;
    for (int i = 0; i < settings->space_num; i++) {
      if (settings->is_minus_flag) break;
      if (settings->is_leading_zero)
        str[settings->written++] = '0';
      else
        str[settings->written++] = ' ';  // записываем пробелы
    }
  }
  // записываем число
  if (settings->is_length_h) {
    len = unsigned_short_int_to_string(str + settings->written, x, settings);
  } else if (settings->is_length_l) {
    len = unsigned_long_int_to_string(str + settings->written, x, settings);
  } else {
    len = unsigned_int_to_string(str + settings->written, x, settings);
  }
  settings->written += len;
  check_minus_flag(str, settings);
}

void process_f(va_list args, char *str, Parameters *settings) {
  long double x = 0;
  if (settings->is_length_L)
    x = va_arg(args, long double);
  else
    x = va_arg(args, double);
  // проверяем флаги и длину
  if (settings->is_space_flag && x > 0) str[settings->written++] = ' ';
  if (settings->is_plus_flag && x >= 0) str[settings->written++] = '+';
  // записываем пробелы
  if (settings->is_width && settings->width > 0) {
    int llen = float_to_string(str + settings->written, x, settings);
    settings->space_num = settings->width - llen;
    for (int i = 0; i < settings->space_num; i++) {
      if (settings->is_minus_flag) break;
      if (settings->is_leading_zero)
        str[settings->written++] = '0';
      else
        str[settings->written++] = ' ';
    }
  }
  int len = float_to_string(str + settings->written, x,
                            settings);  // записываем число
  settings->written += len;
  if (settings->is_plus_flag && x >= 0) {
    settings->space_num--;  // для обработки +-width/accuracy
  }
  check_minus_flag(str, settings);
}

void process_c(va_list args, char *str, Parameters *settings) {
  char x = (char)va_arg(args, int);
  // записываем пробелы
  if (settings->is_width) {
    for (int i = 0; i < settings->width - 1;
         i++) {  // символ всегда один, поэтому -1
      if (settings->is_minus_flag) break;
      str[settings->written++] = ' ';
    }
  }
  str[settings->written] = x;  // записываем символ в буфер
  settings->written++;  // увеличиваем счетчик записанных символов на 1

  if (settings->is_minus_flag) {
    for (int i = 0; i < settings->width - 1; i++) {
      str[settings->written++] = ' ';
    }
  }
}

void process_s(va_list args, char *str, Parameters *settings) {
  const char *x = va_arg(args, char *);
  // записываем пробелы
  if (settings->is_width) {
    size_t llen = s21_strlen(x);
    if (settings->is_accuracy && settings->accuracy < (int)llen)
      llen = settings->accuracy;
    settings->space_num = settings->width - (int)llen;

    for (int i = 0; i < settings->space_num; i++) {
      if (settings->is_minus_flag) break;
      str[settings->written++] = ' ';
    }
  }
  size_t len = s21_strlen(x);
  if (settings->is_accuracy && (size_t)settings->accuracy < len) {
    len = (size_t)settings->accuracy;
  }

  s21_strncpy(str + settings->written, x, len);  // копируем строку в буфер
  settings->written += len;

  check_minus_flag(str, settings);
}

void process_p(va_list args, char *str, Parameters *settings) {
  settings->is_pointer = true;
  void *ptr = va_arg(args, void *);  // Извлекаем указатель из списка аргументов
  unsigned long int address = (unsigned long int)ptr;

  int len = hex_to_string(str + settings->written, address, settings);
  if (settings->is_minus_flag) {
    len = hex_to_string(str + settings->written, address, settings);
  }
  settings->written += len;
  settings->is_pointer = false;
}

void process_xX(va_list args, char *str, Parameters *settings) {
  unsigned long int x = va_arg(args, unsigned long int);
  int len = hex_to_string(str + settings->written, x, settings);
  if (settings->is_minus_flag) {
    len = hex_to_string(str + settings->written, x, settings);
  }
  settings->written += len;
}

void process_o(va_list args, char *str, Parameters *settings) {
  unsigned long int o = va_arg(args, unsigned long int);
  int len = oct_to_string(str + settings->written, o, settings);
  if (settings->is_minus_flag) {
    len = oct_to_string(str + settings->written, o, settings);
  }
  settings->written += len;
}

void process_gG(va_list args, char *str, Parameters *settings) {
  settings->is_gG = true;
  long double x = 0;
  if (settings->is_length_L)
    x = va_arg(args, long double);
  else
    x = va_arg(args, double);
  // добавляем поддержку минуса и пробела
  if (x < 0 && !settings->is_space_flag) {
    str[settings->written++] = '-';
    x = -x;
  } else if (settings->is_space_flag && x > 0) {
    str[settings->written++] = ' ';
  }
  // записываем число
  gG_float_to_string(str + settings->written, x, settings);
}

void process_eE(va_list args, char *str, Parameters *settings) {
  settings->is_eE = true;
  long double x = 0;
  if (settings->is_length_L)
    x = va_arg(args, long double);
  else
    x = va_arg(args, double);

  if (settings->is_plus_flag && x >= 0) {
    settings->width--;
  } else if (settings->is_space_flag && x > 0) {
    str[settings->written++] = ' ';
    settings->width--;
  }

  // записываем пробелы
  if (settings->is_width && settings->width > 0) {
    int llen = eE_float_to_string(str + settings->written, x, settings);
    settings->space_num = settings->width - llen;
    for (int i = 0; i < settings->space_num; i++) {
      if (settings->is_minus_flag) break;
      if (settings->is_leading_zero)
        str[settings->written++] = '0';
      else
        str[settings->written++] = ' ';
    }
  }

  if (settings->is_plus_flag && x >= 0) {
    str[settings->written++] = '+';
  }

  int len = eE_float_to_string(str + settings->written, x,
                               settings);  // записываем число
  settings->written += len;
  check_minus_flag(str, settings);
}

// __________________________________________________________________________________________________________HELPFUL_FUNCTIONS

void add_spaces_to_d(char *str, Parameters *settings) {
  if (settings->is_negative) {
    settings->space_num--;
    str[settings->written++] = '-';
    settings->is_negative = false;
  }
  for (int i = 0; i < settings->space_num; i++) {
    if (settings->is_minus_flag) break;
    if (settings->is_leading_zero)
      str[settings->written++] = '0';
    else
      str[settings->written++] = ' ';
  }
}

void check_minus_flag(char *str, Parameters *settings) {
  if (settings->is_minus_flag) {
    for (int i = 0; i < settings->space_num; i++) {
      str[settings->written++] = ' ';
    }
  }
}

void reverse_str(char *str, int i) {
  for (int j = 0; j < i / 2; j++) {
    char temp = str[j];
    str[j] = str[i - j - 1];
    str[i - j - 1] = temp;
  }
}

int create_str_from_integers(char *str, int i, Parameters *settings) {
  // Добавление знака минус, если число отрицательное
  if (settings->is_negative && !settings->is_accuracy &&
      !settings->is_leading_zero) {
    if (!settings->is_gG && !settings->is_eE) str[i++] = '-';
    settings->is_negative = false;
  }
  if (settings->is_accuracy && !settings->flag) {
    // если ширина указана как ноль, то точность не отрабатывает
    // точность отрабатывает если ширина больше нуля
    if (settings->is_width && settings->width > 0) {
      int print_null_count = settings->accuracy - i;
      for (int k = 0; k < print_null_count; k++) {
        str[i++] = '0';
      }
    }
    // но если ширина не указана, точность должна отработать
    // хотя она инициализируется нулём
    else if (!settings->is_width) {
      int print_null_count = settings->accuracy - i;
      for (int k = 0; k < print_null_count; k++) {
        str[i++] = '0';
      }
    }
    settings->flag = false;
    settings->is_negative = false;
  }
  str[i] = '\0';  // завершаем терминирующим нулем
  reverse_str(str, i);  // реверс строки, чтобы получить правильный порядок цифр
  return i;             // возвращаем длину строки
}

// Функция для преобразования беззнакового целого числа в строку
int unsigned_int_to_string(char *str, unsigned int value,
                           Parameters *settings) {
  int i = 0;
  do {
    str[i++] = '0' + (value % 10);
    value /= 10;
  } while (value > 0);
  i = create_str_from_integers(str, i, settings);
  return i;
}

int unsigned_long_int_to_string(char *str, unsigned long int value,
                                Parameters *settings) {
  int i = 0;
  do {
    str[i++] = '0' + (value % 10);
    value /= 10;
  } while (value > 0);
  i = create_str_from_integers(str, i, settings);
  return i;
}

int unsigned_short_int_to_string(char *str, unsigned short int value,
                                 Parameters *settings) {
  int i = 0;
  do {
    str[i++] = '0' + (value % 10);
    value /= 10;
  } while (value > 0);
  i = create_str_from_integers(str, i, settings);
  return i;
}

// Функция для преобразования целого числа в строку
int int_to_string(char *str, int value, Parameters *settings) {
  int i = 0;
  // Обработка отрицательных чисел
  if (value < 0) {
    settings->is_negative = true;
    value = -value;
  }
  // Преобразование числа в строку
  do {
    str[i++] = '0' + (value % 10);
    value /= 10;
  } while (value > 0);
  i = create_str_from_integers(str, i, settings);
  return i;  // возвращаем длину строки
}

int long_int_to_string(char *str, long int value, Parameters *settings) {
  int i = 0;
  // Обработка отрицательных чисел
  if (value < 0) {
    settings->is_negative = true;
    value = -value;
  }
  // Преобразование числа в строку
  do {
    str[i++] = '0' + (value % 10);
    value /= 10;
  } while (value > 0);
  i = create_str_from_integers(str, i, settings);
  return i;  // возвращаем длину строки
}

int short_int_to_string(char *str, short int value, Parameters *settings) {
  int i = 0;
  // Обработка отрицательных чисел
  if (value < 0) {
    settings->is_negative = true;
    value = -value;
  }
  // Преобразование числа в строку
  do {
    str[i++] = '0' + (value % 10);
    value /= 10;
  } while (value > 0);
  i = create_str_from_integers(str, i, settings);
  return i;  // возвращаем длину строки
}

char *s21_strcpy(char *destination, const char *source) {
  char *start = destination;
  while (*source != '\0') {
    *destination = *source;
    destination++;
    source++;
  }
  *destination = '\0';
  return start;
}

void frac_part_to_string(char *str, int *i, int *digit_count,
                         long double *frac_part, Parameters *settings) {
  // записываем дробную часть в строку
  while (ceil(*frac_part) - *frac_part > 0.000001 &&
         *frac_part - floor(*frac_part) > 0.000001) {
    *frac_part = *frac_part - floor(*frac_part);
    *frac_part *= 10;
    if (*digit_count >= settings->accuracy) {
      if (*frac_part >= 5) {
        int t = 1;
        while (str[*i - t] == '9' || str[*i - t] == '.') {
          if (str[*i - t] == '.') {
            t++;
            continue;
          }
          str[*i - t] = '0';
          t++;
        }
        if (str[*i - t] == ' ' || *i - t < 0) {
          char *tmp =
              (char *)s21_insert(str, "1", (*i - t < 0) ? 0 : *i - t + 1);
          s21_strcpy(str, tmp);
          free(tmp);
          if (!(settings->is_gG)) {
            char *p = s21_strchr(str, '.');
            char c = *(p - 1);
            *p = c;
            *(p - 1) = '.';
            settings->order++;
          }
        } else
          str[*i - t] = str[*i - t] + 1;
      }
      break;
    }
    if (ceil(*frac_part) - *frac_part > 0.000001)
      int_to_string(str + *i, floor(*frac_part), settings);
    else
      int_to_string(str + *i, ceil(*frac_part), settings);
    (*digit_count)++;
    (*i)++;
  }
}

int check_infinite(char *str, long double value) {
  int infinite = 0;
  if (value == S21_MAX_INF) {
    s21_strcpy(str, "inf");
    infinite = 3;
  } else if (value == S21_MIN_INF) {
    s21_strcpy(str, "-inf");
    infinite = 4;
  }
  return infinite;
}

int float_to_string(char *str, long double value, Parameters *settings) {
  int i = 0;
  i = check_infinite(str, value);
  if (i == 0) {
    // Обработка отрицательных чисел
    if (value < 0) {
      value = -value;
      str[i++] = '-';  // добавляем знак "-" в начало строки
    }
    // Преобразование целой части числа в строку
    long int int_part = (long int)value;
    settings->flag = true;
    int int_part_len = int_to_string(str + i, int_part, settings);
    i += int_part_len;
    // Добавление десятичной точки
    // если ширина 0, а точность не задана, то отработать должно так, будто
    // ничего не указано
    if (settings->is_width && settings->width == 0 && !settings->is_accuracy)
      str[i++] = '.';
    else if (settings->accuracy != 0)
      str[i++] = '.';  // если точность не равно 0 ставим точку
    // Преобразование дробной части числа в строку
    int digit_count = 0;
    long double frac_part = value;
    frac_part_to_string(str, &i, &digit_count, &frac_part, settings);
    if ((settings->accuracy > digit_count || settings->is_sharp_flag) &&
        !settings->is_gG) {
      int print_null_count = settings->accuracy - digit_count;
      // добавляем нули в строку
      for (int c = 0; c < print_null_count; c++) {
        str[i++] = '0';
      }
    }
  }
  settings->is_gG = false;
  str[i] = '\0';  // завершаем терминирующим нулем
  return i;       // возвращаем длину строки
}

void processing_gG_to_eE(const char *str, int *i, const Parameters *settings) {
  if (settings->gG_to_eE_flag) {
    int j = *i, c = 0;
    while (j != 0) {
      if (str[j] == '0') c++;
      j -= 1;
    }
    *i -= c;
    if (settings->accuracy == 1) {
      while (str[*i - 1] != '.') {
        *i -= 1;
      }
      *i -= 1;
    }
  }
}

void check_case_of_E(char *str, int *i, const Parameters *settings) {
  if (settings->uppercase_e || settings->uppercase_g)
    str[(*i)++] = 'E';
  else
    str[(*i)++] = 'e';  // добавляем символ экспоненты
  if (settings->order < 0)
    str[(*i)++] = '-';
  else
    str[(*i)++] = '+';
  if (settings->order <= 9 && settings->order >= -9) str[(*i)++] = '0';
}

void add_decimal_point(char *str, int *i, const Parameters *settings) {
  // если ширина 0, а точность не задана, то отработать должно так, будто ничего
  // не указано
  if (settings->is_width && settings->width == 0 && !settings->is_accuracy)
    str[(*i)++] = '.';
  else if (settings->accuracy != 0)
    str[(*i)++] = '.';  // если точность не равно 0 ставим точку
}

void adding_zeros(char *str, int *i, int digit_count,
                  const Parameters *settings) {
  if (settings->accuracy > digit_count) {
    int print_null_count = settings->accuracy - digit_count;
    for (int c = 0; c < print_null_count; c++) {
      str[(*i)++] = '0';  // добавляем нули в строку
    }
  }
}

void order_determine(long double *value_temp, Parameters *settings) {
  if (*value_temp > 0 || *value_temp < 0) {
    while (*value_temp < 1) {
      *value_temp *= 10;
      settings->order--;
    }
    while (*value_temp >= 10) {
      *value_temp /= 10;
      settings->order++;
    }
  }
}

void check_negative_numbers(char *str, int *i, long double *value) {
  if (*value < 0 || !(*value > 0 || *value < 0)) {
    *value = -*value;
    str[(*i)++] = '-';  // добавляем знак "-" в начало строки
  }
}

int eE_float_to_string(char *str, long double value, Parameters *settings) {
  int i = 0;
  i = check_infinite(str, value);
  if (i == 0) {
    check_negative_numbers(str, &i, &value);  // Обработка отрицательных чисел
    long int int_part = (long int)value;
    long double value_temp = value;
    order_determine(&value_temp, settings);  // Определение порядка числа

    if (settings->order > -4 || settings->order <= settings->accuracy)
      int_part = (long int)value_temp;
    settings->flag = true;
    int int_part_len = int_to_string(str + i, int_part, settings);
    i += int_part_len;
    add_decimal_point(str, &i, settings);  // Добавление десятичной точки

    int digit_count = 0;
    long double frac_part = value;
    if (settings->is_eE &&
        (settings->order > -4 || settings->order <= settings->accuracy))
      frac_part = value_temp;
    frac_part_to_string(
        str, &i, &digit_count, &frac_part,
        settings);  // Преобразование дробной части числа в строку
    adding_zeros(str, &i, digit_count, settings);  // добавляем нули если надо
  }

  processing_gG_to_eE(str, &i,
                      settings);  // обрабатываю флаги в случае запуска из gG
  check_case_of_E(str, &i, settings);
  int len = int_to_string(str + i, settings->order,
                          settings);  // выводим показатель степени
  i += len;

  if (!settings->gG_to_eE_flag) settings->order = 0;
  str[i] = '\0';
  return i;
}

void check_spaces_and_zeros_and_plus(char *str, int *i, long double value,
                                     Parameters *settings) {
  if (settings->is_plus_flag && value >= 0)
    settings->space_num--;  // для обработки +-width/accuracy
  if (settings->is_space_flag) settings->space_num--;
  for (int j = 0; j < settings->space_num; j++) {
    if (settings->is_minus_flag) break;
    if (settings->is_leading_zero)
      str[(*i)++] = '0';
    else
      str[(*i)++] = ' ';
  }
  if (settings->is_plus_flag && value >= 0) {
    str[(*i)++] = '+';
  }
}

void start_eE_from_gG(char *str, int *i, long double value,
                      long double value_temp, Parameters *settings) {
  settings->gG_to_eE_flag = true;
  int llen = eE_float_to_string(str, value_temp, settings);
  settings->space_num = settings->width - llen;
  check_spaces_and_zeros_and_plus(str, i, value, settings);
  if (settings->is_length_L) *i += llen;
  if (!settings->is_length_L) {
    int len = eE_float_to_string(str + *i, value_temp, settings);
    *i += len;
  }
  settings->written += *i;
}

int gG_float_to_string(char *str, long double value, Parameters *settings) {
  int i = 0;
  i = check_infinite(str, value);
  if (i == 0) {
    // обработка отрицательных чисел
    check_negative_numbers(str, &i, &value);  // Обработка отрицательных чисел
    long double value_temp = value;
    order_determine(&value_temp, settings);  // Определение порядка числа
    if (settings->order >= -4 && settings->order < settings->accuracy) {
      settings->accuracy = settings->accuracy - (settings->order + 1);
      int llen = float_to_string(str, value, settings);
      settings->space_num = settings->width - llen;
      check_spaces_and_zeros_and_plus(str, &i, value, settings);
      int len = float_to_string(str + i, value, settings);
      i += len;
      while (str[i - 1] == '0') {
        i--;  // убираем лишние нули
      }
      if (settings->is_minus_flag) {
        for (int j = 0; j < settings->space_num; j++) {
          str[i++] = ' ';
        }
      }
      settings->written += i;
    } else
      start_eE_from_gG(str, &i, value, value_temp, settings);
  }
  settings->gG_to_eE_flag = false;
  settings->is_gG = false;
  settings->order = 0;
  str[i] = '\0';  // завершаем терминирующим нулем
  return i;       // возвращаем длину строки
}

// Функция для преобразования шестнадцатеричного числа в строку
int hex_to_string(char *str, unsigned long int value, Parameters *settings) {
  const char digits_lower[] = "0123456789abcdef";  // нижний регистр hex
  const char digits_upper[] = "0123456789ABCDEF";  // верхний регистр hex
  const char *digits = (settings->uppercase_x) ? digits_upper : digits_lower;

  int i = 0;
  if (settings->is_minus_flag) {
    for (int k = 0; k < settings->space_num; k++) {
      str[i++] = ' ';
    }
  }
  // преобразование числа в строку
  do {
    str[i++] = digits[value % 16];
    value /= 16;
  } while (value > 0);

  // добавление "0x" в начало строки, если # или %p
  if (settings->is_sharp_flag || settings->is_pointer) {
    str[i++] = (settings->uppercase_x) ? 'X' : 'x';
    str[i++] = '0';
  }
  int llen = i;
  if (settings->is_width && settings->width > 0) {
    settings->space_num = settings->width - llen;
    // записываем пробелы
    if (settings->is_plus_flag) settings->space_num--;
    for (int j = 0; j < settings->space_num; j++) {
      if (settings->is_minus_flag) break;
      str[i++] = ' ';
    }
  }
  str[i] = '\0';
  reverse_str(str, i);
  return i;
}

int oct_to_string(char *str, unsigned long int value, Parameters *settings) {
  const char digits[] = "01234567";  // Цифры в восьмеричной системе
  int i = 0;
  if (settings->is_minus_flag) {
    for (int k = 0; k < settings->space_num; k++) {
      str[i++] = ' ';
    }
  }
  // Преобразование числа в строку в восьмеричной системе
  do {
    str[i++] = digits[value % 8];
    value /= 8;
  } while (value > 0);
  // Добавление "0" в начало строки, если # или %p
  if (settings->is_sharp_flag || settings->is_pointer) {
    str[i++] = '0';
  }
  int llen = i;
  if (settings->is_width && settings->width > 0) {
    settings->space_num = settings->width - llen;
    for (int j = 0; j < settings->space_num; j++) {
      if (settings->is_minus_flag) break;
      str[i++] = ' ';
    }
  }
  str[i] = '\0';
  reverse_str(str, i);
  return i;
}
