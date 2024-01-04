#include "../s21_string.h"

void *s21_memchr(const void *ptr, int c, s21_size_t count) {
  s21_size_t i = 0;
  char *str = (char *)ptr;
  char *result = s21_NULL;

  while (*str != '\0' && i < count) {
    if (*str == (char)c) {
      result = (char *)str;
      break;
    }
    str++;
    i++;
  }
  return result;
}
