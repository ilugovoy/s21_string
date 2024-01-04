#include "../s21_string.h"

char *s21_strstr(const char *haystack, const char *needle) {
  char *first = (char *)haystack;
  const char *second = (char *)needle;
  char *result = s21_NULL;
  if (*second == '\0') {
    result = first;
  } else {
    for (s21_size_t i = 0; i < s21_strlen(first); i++) {
      if (*(first + i) == *second) {
        char *ptr = s21_strstr(first + 1 + i, second + 1);
        result = (ptr) ? ptr - 1 : s21_NULL;
        break;
      }
    }
  }
  return result;
}