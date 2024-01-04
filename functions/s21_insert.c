#include "../s21_string.h"

void *s21_insert(const char *src, const char *str, size_t start_index) {
  char *result = "\0";
  if (start_index > s21_strlen(src)) {
    result = s21_NULL;
  } else if (!s21_strlen(src) && !s21_strlen(str)) {
    result = "\0";
  } else {
    result = malloc(s21_strlen(src) + s21_strlen(str) + 1);
    if (s21_strlen(src)) {
      size_t i = 0;
      size_t z = 0;
      for (i = 0; i < s21_strlen(src); i++) {
        if (i == start_index) {
          for (z = 0; z < s21_strlen(str); z++) {
            result[i + z] = str[z];
          }
        }
        result[i + z] = src[i];
      }
      result[i + z] = '\0';
    } else {
      s21_strncpy(result, str, s21_strlen(str) + 1);
    }
  }
  return result;
}