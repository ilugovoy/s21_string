#include "../s21_string.h"

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  s21_size_t i = 0;
  int len = s21_strlen(dest);
  while (i < n && src[i] != '\0') {
    dest[i + len] = src[i];
    i++;
  }
  dest[i + len] = '\0';
  return dest;
}
