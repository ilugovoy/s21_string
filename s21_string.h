#ifndef S21_STRING
#define S21_STRING

#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>

typedef unsigned long s21_size_t;  // псевдоним для "размерного" типа данных
#define s21_NULL ((void *)0)  // константа "указатель в никуда"

// Обязательное задание №1: функции библиотеки s21_srting.a

//__shanikat
void *s21_memchr(const void *ptr, int c,
                 s21_size_t count);  // поиск первого вхождения символа в первых
                                     // n байтах строки str
char *s21_strncat(char *dest, const char *src,
                  s21_size_t n);  // добавляет src в конец строки dest, длиной n
char *s21_strchr(const char *str,
                 int c);  // поиск первого вхождения в строке str

//__holdosto
void *s21_memcpy(void *dest, const void *src,
                 s21_size_t n);  // копирует n символов из src в dest
void *s21_memset(
    void *str, int c,
    s21_size_t n);  // копирует символ в первые n символов строки str
char *s21_strncpy(char *dest, const char *src,
                  s21_size_t n);  // копирует до n символов из строки src в dest

//__fungusgr
char *s21_strerror(
    int errnum);  // поиск в массиве номера ошибки errnum и возврат указателя на
                  // строку с сообщением об ошибке
s21_size_t s21_strlen(const char *str);  // вычисляет длину str, без учёта /0
s21_size_t s21_strcspn(
    const char *str1,
    const char *str2);  // вычисляет длину начального сегмента str1, который
                        // состоит из символов, не входящих в str2

//__foldeslu
int s21_strncmp(const char *str1, const char *str2,
                s21_size_t n);  // сравнивает строки с ограничением количества
                                // сравниваемых символов
int s21_memcmp(const void *str1, const void *str2,
               s21_size_t n);  // сравнивает первые n байтов str1 и str2
char *s21_strtok(char *str, const char *delim);  // разбивает строку str на ряд
                                                 // токенов, разделенных delim.

//__maggytel
char *s21_strstr(const char *haystack,
                 const char *needle);  // находит первое вхождение строки needle
                                       // в строке haystack
char *s21_strpbrk(
    const char *str1,
    const char *str2);  // находит первый символ в строке str1, который
                        // соответствует любому символу в str2
char *s21_strrchr(const char *str,
                  int c);  // поиск последнего вхождения символа в строке str

// Обязательное задание №2 + дополнительное задание №1: s21_sprintf
int s21_sprintf(char *str, const char *format, ...);

// Дополнительное задание №2: s21_sscanf
int s21_sscanf(const char *str, const char *format, ...);

// Дополнительное задание №3: like C# функции
void *s21_to_upper(const char *str);  // Возвращает копию строки (str),
                                      // преобразованной в верхний регистр
void *s21_to_lower(const char *str);  // Возвращает копию строки (str),
                                      // преобразованной в нижний регистр
void *s21_insert(const char *src, const char *str,
                 size_t start_index);  // Возвращает строку, в которой (str)
                                       // вставлена в (start_index) в (src)
void *s21_trim(
    const char *src,
    const char *trim_chars);  // Возвращает строку, в которой удаляются все
                              // начальные и конечные вхождения набора заданных
                              // символов (trim_chars)

#endif