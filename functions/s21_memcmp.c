#include "../s21_string.h"

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  int result = 0;
  for (size_t i = 0; i < n; i++) {
    if (((const unsigned char *)str1)[i] != ((const unsigned char *)str2)[i]) {
      result =
          ((const unsigned char *)str1)[i] - ((const unsigned char *)str2)[i];
      break;
    }
  }
  return result;
}
