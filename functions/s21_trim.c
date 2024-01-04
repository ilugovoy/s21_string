#include "../s21_string.h"

void delete_on_start(const char *src, const char *trim_chars, char **result) {
  int i = 0, flag = 1, z = 0;
  for (size_t j = 0; j < s21_strlen(src); j++) {
    if (flag) {
      for (size_t x = 0; x < s21_strlen(trim_chars); x++) {
        if (src[j] == trim_chars[x]) {
          z = 1;
          break;
        }
      }
    }
    if (!z) {
      (*result)[i] = src[j];
      i++;
      flag = 0;
    }
    z = 0;
  }
  (*result)[i] = '\0';
}

void lenin(char **result) {
  for (size_t x = 0; x < s21_strlen((*result)) / 2; x++) {
    char c;
    c = (*result)[x];
    (*result)[x] = (*result)[s21_strlen((*result)) - x - 1];
    (*result)[s21_strlen((*result)) - x - 1] = c;
  }
}

void *s21_trim(const char *src, const char *trim_chars) {
  char *result1 = NULL;
  char *result2 = NULL;
  if (src != NULL) {
    result1 = malloc(s21_strlen(src) + 1);
    delete_on_start(src, trim_chars, &result1);
    lenin(&result1);
    result2 = malloc(s21_strlen(result1) + 1);
    delete_on_start(result1, trim_chars, &result2);
    lenin(&result2);

    // добавил освобождение памяти в случае ошибки
    // для исправления functions/s21_trim.c:43:5: error: Common realloc mistake:
    // 'result2' nulled but not freed upon failure [memleakOnRealloc]
    void *temp = realloc(result2, s21_strlen(result2) + 1);
    if (temp == NULL) {
      free(result2);
      return NULL;
    } else {
      result2 = temp;
    }
    free(result1);
  }
  return result2;
}
