#include "../s21_string.h"

char *s21_strrchr(const char *str, int c) {
  char *result = s21_NULL;
  if (c == 0) {
    result = "";
  } else {
    while (*str != '\0') {
      if (*str == (char)c) {
        result = (char *)str;
      }
      str++;
    }
  }
  return result;
}
