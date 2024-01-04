#include "../s21_string.h"

// char *s21_strtok(char *str, const char *delim) {
//   static char *lastToken = s21_NULL;
//   char *startToken;
//   char *endToken;
//   if (str != s21_NULL) {
//     lastToken = str;
//   }
//   if (lastToken == s21_NULL || *lastToken == '\0') {
//     return s21_NULL;
//   }
//   startToken = lastToken;
//   while (*startToken != '\0' && s21_strchr(delim, *startToken) != s21_NULL) {
//     ++startToken;
//   }
//   if (*startToken == '\0') {
//     lastToken = s21_NULL;
//     return s21_NULL;
//   }
//   endToken = startToken + 1;
//   while (*endToken != '\0' && s21_strchr(delim, *endToken) == s21_NULL) {
//     ++endToken;
//   }
//   *endToken = '\0';
//   lastToken = endToken + 1;
//   return startToken;
// }

// тесты strtok проходили на wsl без проблем, но на маке упали с такой ошибкой
// Assertion 'my_strtok == ((void *)0)' failed: my_strtok == 0x7ffeee405494,
// ((void *)0) == 0 добавил такую реализацию: если что-то завалится на wsl,
// сделаю двойную реализацию через #if defined(__linux__)
char *s21_strtok(char *str, const char *delim) {
  static char *lastToken = s21_NULL;
  char *startToken;
  char *endToken;

  if (str != s21_NULL) {
    lastToken = str;
  }
  if (lastToken == s21_NULL || *lastToken == '\0') {
    return s21_NULL;
  }
  startToken = lastToken;
  while (*startToken != '\0' && s21_strchr(delim, *startToken) != s21_NULL) {
    ++startToken;
  }
  if (*startToken == '\0') {
    lastToken = s21_NULL;
    return s21_NULL;
  }
  endToken = startToken + 1;
  while (*endToken != '\0' && s21_strchr(delim, *endToken) == s21_NULL) {
    ++endToken;
  }
  char *token =
      (char *)malloc(endToken - startToken + 1);  // Выделяем память для токена
  if (token == s21_NULL) {
    // Освобождаем ранее выделенную память, если malloc вернул NULL
    // Это предотвращает утечку памяти в случае ошибки выделения памяти
    free(token);
    return s21_NULL;
  }
  s21_strncpy(token, startToken,
              endToken - startToken);  // Копируем токен в новую строку
  token[endToken - startToken] =
      '\0';  // Устанавливаем завершающий нулевой символ
  lastToken = endToken;

  return token;
}
