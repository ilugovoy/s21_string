#include "../s21_string.h"

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  s21_size_t len = 0;
  bool match = false;
  const char *p_to_el_1 = str1;
  const char *p_to_el_2 = str2;

  for (; *p_to_el_1 != '\0'; p_to_el_1++) {
    for (; *p_to_el_2 != '\0'; p_to_el_2++) {
      if (*p_to_el_1 == *p_to_el_2) match = true;
    }
    if (match) break;
    p_to_el_2 = str2;
    len++;
  }
  return len;
}
