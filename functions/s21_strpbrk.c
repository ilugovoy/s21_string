#include "../s21_string.h"

char *s21_strpbrk(const char *str1, const char *str2) {
  char *result = s21_NULL;
  int flag = 0;
  while (*str1 != '\0') {
    char *second = (char *)str2;
    while (*second != '\0') {
      if (*str1 == *second) {
        result = (char *)str1;
        flag = 1;
        break;
      } else {
        second++;
      }
    }
    if (flag) break;
    str1++;
  }
  return result;
}
