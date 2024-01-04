#include "../s21_string.h"

s21_size_t s21_strlen(const char *string) {
  s21_size_t len = 0;
  for (int i = 0; string[i] != '\0'; i++) {
    len++;
  }
  return len;
}
