#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s21_sprintf.h"
#include "s21_sscanf.h"
#include "s21_string.h"

// TESTS CASES
#define STRING_LENGTH_50 "_THIS is a SOURCE string. Length = (49 + 0) = 50_\0"
#define NULL_STRING "_THIS is a'\0' SOURCE. Length'\0' = (51 + 0) = 51_\0"
#define STRING_LENGTH_6 "-----"
#define LENGTH (50)

#define ERROR_TEST ck_assert_str_eq(s21_strerror(i), strerror(i))
#define ERROR_TEST_COUNT (140)

#include <sys/utsname.h>
// Функция для проверки архитектуры процессора
int isProcessorArchitectureM1orM2() {
  struct utsname systemInfo;
  uname(&systemInfo);
  if (strcmp(systemInfo.machine, "arm64") == 0) {
    return 1;  // Возвращаем 1, если архитектура соответствует M1 или M2
  }
  return 0;
}

//_________________________________________________NEXT COME THE MANDATORY TASK
// PART 1 TESTS: STRING FUNCTIONS

START_TEST(s21_strlen_test) {
  ck_assert(s21_strlen(STRING_LENGTH_50) == strlen(STRING_LENGTH_50));
  ck_assert_int_eq(s21_strlen("1023456789"), strlen("1023456789"));
  ck_assert_int_eq(s21_strlen("123\0456"), strlen("123\0456"));
  ck_assert_int_eq(s21_strlen("hhh\0"), strlen("hhh\0"));
  ck_assert_int_eq(s21_strlen("789"), strlen("789"));
  ck_assert_int_eq(s21_strlen("1\0"), strlen("1\0"));
  ck_assert_int_eq(s21_strlen("   "), strlen("   "));
}
END_TEST

START_TEST(s21_strerror_test) {
  for (int i = 0; i < ERROR_TEST_COUNT; i++) ERROR_TEST;
}
END_TEST

START_TEST(s21_strcspn_test) {
  ck_assert(s21_strcspn(NULL_STRING, STRING_LENGTH_50) ==
            strcspn(NULL_STRING, STRING_LENGTH_50));
  ck_assert(s21_strcspn(STRING_LENGTH_50, NULL_STRING) ==
            strcspn(STRING_LENGTH_50, NULL_STRING));
  ck_assert_uint_eq(s21_strcspn("1023456789", "789"),
                    strcspn("1023456789", "789"));
  ck_assert_uint_eq(s21_strcspn("1\0", "1023456789"),
                    strcspn("1\0", "1023456789"));
  ck_assert_uint_eq(s21_strcspn("   ", "1023456789"),
                    strcspn("   ", "1023456789"));
  ck_assert(s21_strcspn("", STRING_LENGTH_50) == strcspn("", STRING_LENGTH_50));
  ck_assert(s21_strcspn(STRING_LENGTH_50, "") == strcspn(STRING_LENGTH_50, ""));
  ck_assert_uint_eq(s21_strcspn("1\0", "   "), strcspn("1\0", "   "));
  ck_assert_uint_eq(s21_strcspn("1\0", "0/"), strcspn("1\0", "0/"));
  ck_assert_uint_eq(s21_strcspn("789", "0/"), strcspn("789", "0/"));
}
END_TEST

START_TEST(s21_memcmp_test) {
  ck_assert_uint_eq(s21_memcmp(STRING_LENGTH_50, NULL_STRING, 10) < 0,
                    memcmp(STRING_LENGTH_50, NULL_STRING, 10) < 0);
  ck_assert_uint_eq(s21_memcmp(STRING_LENGTH_50, "\0", '1') > 0,
                    memcmp(STRING_LENGTH_50, "\0", '1') > 0);
  ck_assert_uint_eq(s21_memcmp(NULL_STRING, "\0", '1') > 0,
                    memcmp(NULL_STRING, "\0", '1') > 0);
  ck_assert_uint_eq(s21_memcmp("123\0456", STRING_LENGTH_50, 10) > 0,
                    memcmp("123\0456", STRING_LENGTH_50, 10) > 0);
}
END_TEST

START_TEST(s21_strncmp_test) {
  const char case1[] = "hella 1";
  const char case2[] = "hello 3";
  ck_assert_int_eq(s21_strncmp(case1, case2, 5) < 0,
                   strncmp(case1, case2, 5) < 0);
  ck_assert_int_eq(s21_strncmp(case1, case2, 3) == 0,
                   strncmp(case1, case2, 3) == 0);
}
END_TEST

START_TEST(s21_strtok_test) {
  char str_strtok1[] = "one/two/three(four)five";
  const char delim1[] = "/()";
  char str_strtok2[] = "one/two/three(four)five";
  const char delim2[] = "/()";
  char *my_strtok = s21_strtok(str_strtok1, delim1);
  char *origin_strtok = strtok(str_strtok2, delim2);
  while (my_strtok != s21_NULL) {
    ck_assert_str_eq(my_strtok, origin_strtok);
    my_strtok = s21_strtok(s21_NULL, delim1);
    origin_strtok = strtok(s21_NULL, delim2);
  }

  char str_strtok3[] = "/()";
  const char delim3[] = "/()";
  char str_strtok4[] = "/()";
  const char delim4[] = "/()";
  const char *my_strtok1 = s21_strtok(str_strtok3, delim3);
  const char *origin_strtok1 = strtok(str_strtok4, delim4);
  ck_assert(my_strtok1 == NULL);
  ck_assert(origin_strtok1 == NULL);
}
END_TEST

START_TEST(s21_strstr_test) {
  const char *test_cases[][2] = {{"<<<5>>>", "5"},
                                 {"QwertyQwertyQwerty", "Qwerty"},
                                 {"abcd", ""},
                                 {"Qwerty", "abcd"},
                                 {"aaaaa123aaa1234aaa", "1234"}};

  for (int i = 0; i < 5; i++) {
    ck_assert_ptr_eq(s21_strstr(test_cases[i][0], test_cases[i][1]),
                     strstr(test_cases[i][0], test_cases[i][1]));
  }
}
END_TEST

START_TEST(s21_strrchr_test) {
  const char *test_cases[] = {"0163456769", ";;;;;;H%%//#HH", "     /"};
  const int test_cases_second[] = {'H', '$', ';', ' '};
  s21_size_t test_numbers = 3;

  for (s21_size_t i = 0; i < test_numbers; i++) {
    for (s21_size_t j = 0; j < test_numbers; j++)
      ck_assert_uint_eq(
          (unsigned long)s21_strrchr(test_cases[i], test_cases_second[j]),
          (unsigned long)strrchr(test_cases[i], test_cases_second[j]));
  }
  ck_assert_str_eq(s21_strrchr(test_cases[2], '\0'),
                   strrchr(test_cases[2], '\0'));
}
END_TEST

START_TEST(s21_strpbrk_test) {
  const char str_for_strpbrk[] = "Megalomania";
  const char str_oneof[] = "yal";
  ck_assert_str_eq(s21_strpbrk(str_for_strpbrk, str_oneof), "alomania");
  ck_assert_str_eq(s21_strpbrk(str_for_strpbrk, str_oneof),
                   strpbrk(str_for_strpbrk, str_oneof));

  const char *str = "Hello, world";
  const char *empty = "";
  ck_assert(s21_strpbrk(str, empty) == s21_NULL);
  ck_assert_uint_eq((unsigned long)s21_strpbrk(str, empty),
                    (unsigned long)strpbrk(str, empty));
}

START_TEST(s21_memcpy_test) {
  char case_1[10] = "12345";
  const char case_2[] = "  ";
  char case_3[20] = "///  ";
  const char case_4[] = "   ///";
  char case_5[30] = "";
  const char case_6[] = "4444";
  const char case_7[] = "123\0123";
  ck_assert_str_eq(s21_memcpy(case_1, case_2, 2), memcpy(case_1, case_2, 2));
  ck_assert_str_eq(s21_memcpy(case_3, case_4, 6), memcpy(case_3, case_4, 6));
  ck_assert_str_eq(s21_memcpy(case_5, case_6, 4), memcpy(case_5, case_6, 4));
  ck_assert_str_eq(s21_memcpy(case_5, case_7, 6), memcpy(case_5, case_7, 6));
}
END_TEST

START_TEST(s21_memset_test) {
  unsigned char case1[10] = "1234357890";
  unsigned char case2[10] = "hello";
  ck_assert_uint_eq((unsigned long)s21_memset(case1, '1', 10),
                    (unsigned long)memset(case1, '1', 10));
  ck_assert_uint_eq((unsigned long)s21_memset(case1, '3', 10),
                    (unsigned long)memset(case1, '3', 10));
  ck_assert_uint_eq((unsigned long)s21_memset(case1, '/', 8),
                    (unsigned long)memset(case1, '/', 8));
  ck_assert_uint_eq((unsigned long)s21_memset(case1, '9', 9),
                    (unsigned long)memset(case1, '9', 9));
  ck_assert_uint_eq((unsigned long)s21_memset(case1, '0', 4),
                    (unsigned long)memset(case1, '0', 4));
  ck_assert_uint_eq((unsigned long)s21_memset(case2, '1', 3),
                    (unsigned long)memset(case2, '1', 3));
}
END_TEST

START_TEST(s21_strncpy_test) {
  char dest1[15] = "";
  char dest2[5] = "";
  char dest3[] = "crushcrush";
  ck_assert_str_eq("test ", s21_strncpy(dest1, "test of string", 5));
  ck_assert_str_eq("test", s21_strncpy(dest2, "test\0", 4));
  ck_assert_str_eq("test crush", s21_strncpy(dest3, "test ", 5));
}
END_TEST

START_TEST(s21_memchr_test) {
  ck_assert_uint_eq((unsigned long)s21_memchr("1023456789", 10, 10),
                    (unsigned long)memchr("1023456789", 10, 10));
  ck_assert_uint_eq((unsigned long)s21_memchr("1023456789", 'h', 10),
                    (unsigned long)memchr("1023456789", 'h', 10));
  ck_assert_uint_eq((unsigned long)s21_memchr("h", 'h', '1'),
                    (unsigned long)memchr("h", 'h', '1'));
  ck_assert_uint_eq((unsigned long)s21_memchr("1023456789", '/', 10),
                    (unsigned long)memchr("1023456789", '/', 10));
  ck_assert_uint_eq((unsigned long)s21_memchr("1023456789", '\0', 10),
                    (unsigned long)memchr("1023456789", '\0', 10));
}
END_TEST

START_TEST(s21_strncat_test) {
  char case_2[4] = "<<<";
  char case_4[2] = "5";
  char case_44[2] = "5";
  const char case_5[] = "";
  ck_assert_str_eq(s21_strncat(case_2, case_4, 1), "<<<5");
  ck_assert_str_eq(s21_strncat(case_44, case_5, 0), strncat(case_4, case_5, 0));
}
END_TEST

START_TEST(s21_strchr_test) {
  char str1[] = "Abcdefg";
  int symbol1 = 'd';
  char *ach1 = s21_strchr(str1, symbol1);
  ck_assert_str_eq(ach1, "defg");

  char str2[] = "Abcdefg";
  int symbol2 = 'd';
  char *ach2 = strchr(str2, symbol2);
  ck_assert_str_eq(ach1, ach2);
  ck_assert_str_eq(str1, str2);

  char str3[] = "Abcdefg";
  int symbol3 = '\0';
  char *ach3 = s21_strchr(str3, symbol3);

  char str4[] = "Abcdefg";
  int symbol4 = '\0';
  char *ach4 = strchr(str4, symbol4);
  ck_assert_str_eq(ach3, ach4);
  ck_assert_str_eq(str3, str4);
}
END_TEST

//_________________________________________________NEXT COME THE MANDATORY TASK
// PART 2 TESTS: sprintf

START_TEST(s21_sprintf_f_test) {
  char str[512];
  char str_orig[512];

  double b = 1;
  int res_s21 = s21_sprintf(str, "%.f", b);
  int res_orig = sprintf(str_orig, "%.f", b);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(str, str_orig);

  double b_2 = 0.00000002345;
  res_s21 = s21_sprintf(str, "%+-10.3f", b_2);
  res_orig = sprintf(str_orig, "%+-10.3f", b_2);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(str, str_orig);

  double b_3 = -23.087654;
  res_s21 = s21_sprintf(str, "%20.10f", b_3);
  res_orig = sprintf(str_orig, "%20.10f", b_3);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(str, str_orig);

  double b_4 = 0;
  res_s21 = s21_sprintf(str, "%0+.15f", b_4);
  res_orig = sprintf(str_orig, "%0+.15f", b_4);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(str, str_orig);

  double b_5 = 0;
  res_s21 = s21_sprintf(str, "%.0f", b_5);
  res_orig = sprintf(str_orig, "%.0f", b_5);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(str, str_orig);

  double b_6 = 1.0 / 0.0;
  res_s21 = s21_sprintf(str, "%10.0f", b_6);
  res_orig = sprintf(str_orig, "%10.0f", b_6);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(str, str_orig);

  double b_8 = 1;
  res_s21 = s21_sprintf(str, "%f", b_8);
  res_orig = sprintf(str_orig, "%f", b_8);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(str, str_orig);

  double b_9 = 0.00345;
  res_s21 = s21_sprintf(str, "%f", b_9);
  res_orig = sprintf(str_orig, "%f", b_9);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(str, str_orig);

  double b_10 = 0.00345;
  res_s21 = s21_sprintf(str, "%f", b_10);
  res_orig = sprintf(str_orig, "%f", b_10);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(str, str_orig);

  double b_11 = 90.996;
  // double b_11 = 9.996;
  res_s21 = s21_sprintf(str, "%.2f", b_11);
  res_orig = sprintf(str_orig, "%.2f", b_11);
  // printf("res_orig = %d\nres_s21  = %d\n", res_orig, res_s21);
  // printf("res_orig = |%s|\nres_s21  = |%s|\n\n", str_orig, str);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(str, str_orig);

  double b_12 = 901.9969;
  int res_12 = s21_sprintf(str, "% f", b_12);
  int res_12_orig = sprintf(str_orig, "% f", b_12);
  ck_assert_int_eq(res_12, res_12_orig);
  ck_assert_str_eq(str, str_orig);
}
END_TEST

START_TEST(s21_sprintf_c_test) {
  char str[80];
  char str1[80];
  char x = 'c';
  int res = s21_sprintf(str, "%-10c", x);
  int res1 = sprintf(str1, "%-10c", x);
  ck_assert_uint_eq((unsigned long)*str, (unsigned long)*str1);
  ck_assert_int_eq(res, res1);

  res = s21_sprintf(str, "%10c", x);
  res1 = sprintf(str1, "%10c", x);
  ck_assert_uint_eq((unsigned long)*str, (unsigned long)*str1);
  ck_assert_int_eq(res, res1);
}
END_TEST

START_TEST(s21_sprintf_s_test) {
  char str[80];
  char str1[80];
  char *x = "blablabla";
  int res = s21_sprintf(str, "%-11.10s", x);
  int res1 = sprintf(str1, "%-11.10s", x);
  ck_assert_uint_eq((unsigned long)*str, (unsigned long)*str1);
  ck_assert_int_eq(res, res1);

  res = s21_sprintf(str, "%11.8s", x);
  res1 = sprintf(str1, "%11.8s", x);
  ck_assert_uint_eq((unsigned long)*str, (unsigned long)*str1);
  ck_assert_int_eq(res, res1);
}
END_TEST

START_TEST(s21_sprintf_u_test) {
  char str[80];
  char str_orig[80];
  unsigned int a = 2345;

  char *format_strings[] = {"%10.5u", "%3.5u",   "%0u",   "%-u",    "%u",
                            "%5u",    "%05u",    "%-5u",  "%.5u",   "%.5u",
                            "%-.5u",  "%-10.5u", "%3.5u", "%-3.5u", "%6.2u",
                            "%3.2u",  "%01u",    "%hu",   "%lu"};

  size_t array_size = sizeof(format_strings) / sizeof(format_strings[0]);
  for (size_t i = 0; i < array_size; ++i) {
    int res = s21_sprintf(str, format_strings[i], a);
    int res_orig = sprintf(str_orig, format_strings[i], a);
    ck_assert_int_eq(res, res_orig);
    ck_assert_str_eq(str, str_orig);
  }
}
END_TEST

START_TEST(s21_sprintf_d_test) {
  char str[512];
  char str_orig[512];
  int a = 2345;

  char *format_strings[] = {"%10.5d", "%3.5d",   "%0d",   "%-d",     "%d",
                            "%5d",    "%09d",    "%-5d",  "%.5d",    "%.5d",
                            "%-.5d",  "%-10.5d", "%3.5d", "%-3.5d",  "%6.2d",
                            "%3.2d",  "%01d",    "%hd",   "%ld",     "% 10.5d",
                            "%+d",    "% d",     "%05d",  "%+-10.3d"};

  size_t array_size = sizeof(format_strings) / sizeof(format_strings[0]);
  for (size_t i = 0; i < array_size; ++i) {
    int res, res_orig;
    res = s21_sprintf(str, format_strings[i], a);
    res_orig = sprintf(str_orig, format_strings[i], a);
    ck_assert_int_eq(res, res_orig);
    ck_assert_str_eq(str, str_orig);
  }

  char s21_buffer[1024];
  char buffer[1024];
  int res_orig = 0;
  int res_s21 = 0;

  long int b = -2345;
  res_orig = sprintf(buffer, "%ld", b);
  res_s21 = s21_sprintf(s21_buffer, "%ld", b);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(buffer, s21_buffer);

  short int c = -2142;
  res_orig = sprintf(buffer, "%hd", c);
  res_s21 = s21_sprintf(s21_buffer, "%hd", c);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(buffer, s21_buffer);
}
END_TEST

//_________________________________________________NEXT COME THE ADDITIONAL TASK
// PART 1 TESTS: MORE TO SPRINTF

START_TEST(s21_sprintf_g_test) {
  char s21_buffer[1024];
  char buffer[1024];
  int res_orig = 0;
  int res_s21 = 0;

  double g_num = 123.123;
  res_orig = sprintf(buffer, "Number 1 in general format: %g", g_num);
  res_s21 = s21_sprintf(s21_buffer, "Number 1 in general format: %g", g_num);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(buffer, s21_buffer);

  g_num = 123.123;
  res_orig = sprintf(buffer, "Number 1 in general format: %+-20.10g", g_num);
  res_s21 =
      s21_sprintf(s21_buffer, "Number 1 in general format: %+-20.10g", g_num);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(buffer, s21_buffer);

  // если дробная часть больше точности, то выводится с префиксом e-/+0X где X -
  // количество нулей
  g_num = 9.9966;
  res_orig = sprintf(buffer, "% -5.15g", g_num);
  res_s21 = s21_sprintf(s21_buffer, "% -5.15g", g_num);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(buffer, s21_buffer);

  if (!isProcessorArchitectureM1orM2()) {
    int dlina = 10;
    int tochnost = 4;
    res_orig = sprintf(buffer, "%#*.*g", dlina, tochnost, g_num);
    res_s21 = s21_sprintf(s21_buffer, "%#*.*g", dlina, tochnost, g_num);
    ck_assert_int_eq(res_s21, res_orig);
    ck_assert_str_eq(buffer, s21_buffer);
  }

  g_num = 98.996;
  res_orig = sprintf(buffer, "%0.5g", g_num);
  res_s21 = s21_sprintf(s21_buffer, "%0.5g", g_num);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(buffer, s21_buffer);

  g_num = 0.5996;
  res_orig = sprintf(buffer, "%.2g", g_num);
  res_s21 = s21_sprintf(s21_buffer, "%.2g", g_num);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(buffer, s21_buffer);

  if (!isProcessorArchitectureM1orM2()) {
    double b_9 = 0.0000345;
    res_orig = sprintf(buffer, "%G", b_9);
    res_s21 = s21_sprintf(s21_buffer, "%G", b_9);
    ck_assert_int_eq(res_s21, res_orig);
    ck_assert_str_eq(buffer, s21_buffer);
  }

  g_num = 0.00000123;
  res_orig = sprintf(buffer, "Number 2 in general format: %G", g_num);
  res_s21 = s21_sprintf(s21_buffer, "Number 2 in general format: %G", g_num);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(buffer, s21_buffer);

  g_num = 0.0012345678;
  res_orig = sprintf(buffer, "Number 2 in general format: %g", g_num);
  res_s21 = s21_sprintf(s21_buffer, "Number 2 in general format: %g", g_num);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(buffer, s21_buffer);

  g_num = 12345.12345;
  res_orig = sprintf(buffer, "Number 2 in general format: %.1g", g_num);
  res_s21 = s21_sprintf(s21_buffer, "Number 2 in general format: %.1g", g_num);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(buffer, s21_buffer);

  g_num = -123.12;
  res_orig = sprintf(buffer, "Number 2 in general format: %g", g_num);
  res_s21 = s21_sprintf(s21_buffer, "Number 2 in general format: %g", g_num);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(buffer, s21_buffer);

  long double LG_num = 0.000012345;
  res_orig = sprintf(buffer, "Number 2 in general format: %LG", LG_num);
  res_s21 = s21_sprintf(s21_buffer, "Number 2 in general format: %LG", LG_num);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(buffer, s21_buffer);

  LG_num = 123456.789;
  res_orig = sprintf(buffer, "Number 2 in general format: %LG", LG_num);
  res_s21 = s21_sprintf(s21_buffer, "Number 2 in general format: %LG", LG_num);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(buffer, s21_buffer);
}
END_TEST

START_TEST(s21_sprintf_e_test) {
  // тесты %e & %E

  char s21_buffer[512];
  char buffer[512];
  int res_s21 = 0;
  int res_orig = 0;

  const char *format_str1 = "%-20.7e yo";
  double mantissa1 = -4044050;
  res_s21 = s21_sprintf(s21_buffer, format_str1, mantissa1);
  res_orig = sprintf(buffer, format_str1, mantissa1);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(s21_buffer, buffer);

  const char *format_str2 = "%-.5E yo";
  double mantissa2 = -0.00;
  res_s21 = s21_sprintf(s21_buffer, format_str2, mantissa2);
  res_orig = sprintf(buffer, format_str2, mantissa2);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(s21_buffer, buffer);

  if (!isProcessorArchitectureM1orM2()) {
    const char *format_str3 = "Hello %+020.15E %LE yo";
    double mantissa3 = 1.004554367;
    long double mantissa4 = -432432455454;
    res_s21 = s21_sprintf(s21_buffer, format_str3, mantissa3, mantissa4);
    res_orig = sprintf(buffer, format_str3, mantissa3, mantissa4);
    ck_assert_int_eq(res_s21, res_orig);
    ck_assert_str_eq(s21_buffer, buffer);
  }

  double e_num = 0.01;
  if (!isProcessorArchitectureM1orM2()) {
    res_orig = sprintf(buffer, "Number 1 in general format: %+-20.2e", e_num);
    res_s21 =
        s21_sprintf(s21_buffer, "Number 1 in general format: %+-20.2e", e_num);
    ck_assert_int_eq(res_s21, res_orig);
    ck_assert_str_eq(s21_buffer, buffer);
  }

  res_orig = sprintf(buffer, "Number 1 in general format: %#+.2E", e_num);
  res_s21 =
      s21_sprintf(s21_buffer, "Number 1 in general format: %#+.2E", e_num);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(s21_buffer, buffer);

  e_num = 0.00000123;
  res_orig = sprintf(buffer, "Number 2 in general format: %e", e_num);
  res_s21 = s21_sprintf(s21_buffer, "Number 2 in general format: %e", e_num);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(s21_buffer, buffer);

  double g_num = 9.996;
  res_orig = sprintf(buffer, "%.2e", g_num);
  res_s21 = s21_sprintf(s21_buffer, "%.2e", g_num);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(s21_buffer, buffer);

  g_num = 98.996;
  res_orig = sprintf(buffer, "%.2e", g_num);
  res_s21 = s21_sprintf(s21_buffer, "%.2e", g_num);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(s21_buffer, buffer);

  g_num = 0.5996;
  res_orig = sprintf(buffer, "%.2e", g_num);
  res_s21 = s21_sprintf(s21_buffer, "%.2e", g_num);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(s21_buffer, buffer);

  res_orig = sprintf(buffer, "% e", g_num);
  res_s21 = s21_sprintf(s21_buffer, "% e", g_num);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(s21_buffer, buffer);

  res_orig = sprintf(buffer, "% 0e", g_num);
  res_s21 = s21_sprintf(s21_buffer, "% 0e", g_num);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(s21_buffer, buffer);

  //________________________________________________________________
}
END_TEST

START_TEST(s21_sprintf_p_test) {
  char s21_buffer[1024];
  char buffer[1024];
  int res_orig = 0;
  int res_s21 = 0;

  int test_num = -42;
  res_orig = sprintf(buffer, "Address: %-20p", (void *)&test_num);
  res_s21 = s21_sprintf(s21_buffer, "Address: %-20p", (void *)&test_num);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(buffer, s21_buffer);

  float test_float_num = 42.0;
  res_orig = sprintf(buffer, "Address: %p", (void *)&test_float_num);
  res_s21 = s21_sprintf(s21_buffer, "Address: %p", (void *)&test_float_num);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(buffer, s21_buffer);

  char *string = "Hello, world!";
  res_orig = sprintf(buffer, "Address: %p", string);
  res_s21 = s21_sprintf(s21_buffer, "Address: %p", string);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(buffer, s21_buffer);
}
END_TEST

START_TEST(s21_sprintf_o_test) {
  char s21_buffer[1024];
  char buffer[1024];
  int res_orig = 0;
  int res_s21 = 0;

  int oct_num = 100;
  res_orig = sprintf(buffer, "Number in octal: %-20o", oct_num);
  res_s21 = s21_sprintf(s21_buffer, "Number in octal: %-20o", oct_num);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(buffer, s21_buffer);

  res_orig = sprintf(buffer, "Number in octal: %#20o", oct_num);
  res_s21 = s21_sprintf(s21_buffer, "Number in octal: %#20o", oct_num);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(buffer, s21_buffer);
}
END_TEST

START_TEST(s21_sprintf_x_hex_sharp_star_test) {
  int arr[] = {0, -1, 1, 123, 1234, -1383, 9387};
  char str[80];
  char str1[80];
  int res;
  int res1;

  for (int i = 0; i < 3; i++) {
    res = s21_sprintf(str, "%x", arr[i]);
    res1 = sprintf(str1, "%x", arr[i]);
    ck_assert_uint_eq((unsigned long)*str, (unsigned long)*str1);
    ck_assert_int_eq(res, res1);
  }

  for (int i = 3; i < 7; i++) {
    res = s21_sprintf(str, "%X", arr[i]);
    res1 = sprintf(str1, "%X", arr[i]);
    ck_assert_uint_eq((unsigned long)*str, (unsigned long)*str1);
    ck_assert_int_eq(res, res1);

    res = s21_sprintf(str, "%#10.7lX", (unsigned long)arr[i]);
    res1 = sprintf(str1, "%#10.7lX", (unsigned long)arr[i]);
    ck_assert_uint_eq((unsigned long)*str, (unsigned long)*str1);
    ck_assert_int_eq(res, res1);

    res = s21_sprintf(str, "%-#10.3hX", (unsigned short)arr[i]);
    res1 = sprintf(str1, "%-#10.3hX", (unsigned short)arr[i]);
    ck_assert_uint_eq((unsigned long)*str, (unsigned long)*str1);
    ck_assert_int_eq(res, res1);

    res = s21_sprintf(str, "%-#*.3hX", 10, (unsigned short)arr[i]);
    res1 = sprintf(str1, "%-#*.3hX", 10, (unsigned short)arr[i]);
    ck_assert_uint_eq((unsigned long)*str, (unsigned long)*str1);
    ck_assert_int_eq(res, res1);
  }
}
END_TEST

START_TEST(s21_sprintf_trash_test) {
  char s21_buffer[32];
  char buffer[32];

  int res_s21 = s21_sprintf(s21_buffer, "%%");
  int res_orig = sprintf(buffer, "%%");
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(s21_buffer, buffer);

  res_s21 = s21_sprintf(s21_buffer, "abc%%defg");
  res_orig = sprintf(buffer, "abc%%defg");
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(s21_buffer, buffer);

  double bbt = -1.0 / 0.0;
  res_orig = sprintf(buffer, "%10.0f", bbt);
  res_s21 = s21_sprintf(s21_buffer, "%10.0f", bbt);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(s21_buffer, buffer);

  double bbr = 1.0 / 0.0;
  res_orig = sprintf(buffer, "%10.0f", bbr);
  res_s21 = s21_sprintf(s21_buffer, "%10.0f", bbr);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(s21_buffer, buffer);

  if (!isProcessorArchitectureM1orM2()) {
    int ca = -10, cb = -20, cc = ca + cb;
    res_orig = sprintf(buffer, "Sum of %05d and %0d is %5d", ca, cb, cc);
    res_s21 = s21_sprintf(s21_buffer, "Sum of %05d and %0d is %5d", ca, cb, cc);
    ck_assert_int_eq(res_s21, res_orig);
    ck_assert_str_eq(s21_buffer, buffer);
  }

  double kaka = 0.00000002345;
  res_orig = sprintf(buffer, "%+-10.3f", kaka);
  res_s21 = s21_sprintf(s21_buffer, "%+-10.3f", kaka);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(s21_buffer, buffer);

  double easy = -23.087654;
  res_orig = sprintf(buffer, "%20.10f", easy);
  res_s21 = s21_sprintf(s21_buffer, "%20.10f", easy);
  ck_assert_int_eq(res_s21, res_orig);
  ck_assert_str_eq(s21_buffer, buffer);
}

//_________________________________________________NEXT COME THE ADDITIONAL TASK
// PART 2 TESTS: S21_SSCANF

START_TEST(s21_sscanf_d_test) {
  int n1, n2, n3;
  int res_n1, res_n2, res_n3;

  sscanf("12 13", "%d%d", &n1, &n2);
  s21_sscanf("12 13", "%d%d", &res_n1, &res_n2);
  ck_assert_int_eq(n1, res_n1);
  ck_assert_int_eq(n2, res_n2);

  sscanf("999 321", "%2d%4d", &n1, &n2);
  s21_sscanf("999 321", "%2d%4d", &res_n1, &res_n2);
  ck_assert_int_eq(n1, res_n1);
  ck_assert_int_eq(n2, res_n2);

  sscanf("-999 321", "%d%d", &n1, &n2);
  s21_sscanf("-999 321", "%d%d", &res_n1, &res_n2);
  ck_assert_int_eq(n1, res_n1);
  ck_assert_int_eq(n2, res_n2);

  sscanf("-999 321", "%2d%1d", &n1, &n2);
  s21_sscanf("-999 321", "%2d%1d", &res_n1, &res_n2);
  ck_assert_int_eq(n1, res_n1);
  ck_assert_int_eq(n2, res_n2);

  sscanf("-999 123    -321", "%d%d%d", &n1, &n2, &n3);
  s21_sscanf("-999 123    -321", "%d%d%d", &res_n1, &res_n2, &res_n3);
  ck_assert_int_eq(n1, res_n1);
  ck_assert_int_eq(n2, res_n2);
  ck_assert_int_eq(n3, res_n3);
}

START_TEST(s21_sscanf_u_test) {
  unsigned int u1, u2;
  unsigned int res_u1, res_u2;

  sscanf("999 123", "%u%u", &u1, &u2);
  s21_sscanf("999 123", "%u%u", &res_u1, &res_u2);
  ck_assert_uint_eq(u1, res_u1);
  ck_assert_uint_eq(u2, res_u2);

  sscanf("999 -123", "%u%u", &u1, &u2);
  s21_sscanf("999 -123", "%u%u", &res_u1, &res_u2);
  ck_assert_uint_eq(u1, res_u1);
  ck_assert_uint_eq(u2, res_u2);

  sscanf("-999 123", "%u%u", &u1, &u2);
  s21_sscanf("-999 123", "%u%u", &res_u1, &res_u2);
  ck_assert_uint_eq(u1, res_u1);
  ck_assert_uint_eq(u2, res_u2);
}

START_TEST(s21_sscanf_i_test) {
  int n1, n2, n3;
  int res_n1, res_n2, res_n3;

  sscanf("0x12 012 12", "%i %i %i", &n1, &n2, &n3);
  s21_sscanf("0x12 012 12", "%i %i %i", &res_n1, &res_n2, &res_n3);
  ck_assert_int_eq(n1, res_n1);
  ck_assert_int_eq(n2, res_n2);
  ck_assert_int_eq(n3, res_n3);
}

START_TEST(s21_sscanf_c_test) {
  char c1, c2, c3;
  char res_c1, res_c2, res_c3;

  sscanf("3 4 s", "%c %c %c", &c1, &c2, &c3);
  s21_sscanf("3 4 s", "%c %c %c", &res_c1, &res_c2, &res_c3);

  char str[1024];
  char res_str[1024];
  sprintf(str, "%c %c %c", c1, c2, c3);
  sprintf(res_str, "%c %c %c", c1, c2, c3);
  ck_assert_str_eq(str, res_str);
}

START_TEST(s21_sscanf_s_test) {
  char s1[1024], s2[1024], s3[1024];
  char res_s1[1024], res_s2[1024], res_s3[1024];

  sscanf("a12basd2-c", "%3s%2s%1s", s1, s2, s3);
  s21_sscanf("a12basd2-c", "%3s%2s%1s", res_s1, res_s2, res_s3);
  ck_assert_str_eq(s1, res_s1);
  ck_assert_str_eq(s2, res_s2);
  ck_assert_str_eq(s3, res_s3);

  sscanf("a  b   c", "%10s%10s%10s", s1, s2, s3);
  s21_sscanf("a  b   c", "%10s%10s%10s", res_s1, res_s2, res_s3);
  ck_assert_str_eq(s1, res_s1);
  ck_assert_str_eq(s2, res_s2);
  ck_assert_str_eq(s3, res_s3);

  sscanf("a  b   c", "%5s%9s%12s", s1, s2, s3);
  s21_sscanf("a  b   c", "%5s%9s%12s", res_s1, res_s2, res_s3);
  ck_assert_str_eq(s1, res_s1);
  ck_assert_str_eq(s2, res_s2);
  ck_assert_str_eq(s3, res_s3);
}

START_TEST(s21_sscanf_xX_test) {
  unsigned int u1, u2;
  unsigned int res_u1, res_u2;

  sscanf("-2b 3a", "%X%x", &u1, &u2);
  s21_sscanf("-2b 3a", "%X%x", &res_u1, &res_u2);
  ck_assert_uint_eq(u1, res_u1);
  ck_assert_uint_eq(u2, res_u2);

  sscanf("44a  57f", "%x %X", &u1, &u2);
  s21_sscanf("44a  57f", "%x %X", &res_u1, &res_u2);
  ck_assert_uint_eq(u1, res_u1);
  ck_assert_uint_eq(u2, res_u2);
}

START_TEST(s21_sscanf_o_test) {
  unsigned int u1, u2;
  unsigned int res_u1, res_u2;

  sscanf("342 27", "%o%o", &u1, &u2);
  s21_sscanf("342 27", "%o%o", &res_u1, &res_u2);
  ck_assert_uint_eq(u1, res_u1);
  ck_assert_uint_eq(u2, res_u2);

  sscanf("-342 -27", "%o%o", &u1, &u2);
  s21_sscanf("-342 -27", "%o%o", &res_u1, &res_u2);
  ck_assert_uint_eq(u1, res_u1);
  ck_assert_uint_eq(u2, res_u2);
}

START_TEST(s21_sscanf_f_test) {
  float f1, f2;
  float res_f1, res_f2;

  sscanf("1.23 1.23E-03", "%f%f", &f1, &f2);
  s21_sscanf("1.23 1.23E-03", "%f%f", &res_f1, &res_f2);
  ck_assert_uint_eq(f1, res_f1);
  ck_assert_uint_eq(f2, res_f2);

  sscanf("-1.23 -1.2324e+03", "%f%f", &f1, &f2);
  s21_sscanf("-1.23 -1.2324e+03", "%f%f", &res_f1, &res_f2);
  ck_assert_uint_eq(f1, res_f1);
  ck_assert_uint_eq(f2, res_f2);
}

START_TEST(s21_sscanf_n_test) {
  float f1, f2;
  float res_f1, res_f2;
  int n1, n2;
  int res_n1, res_n2;

  sscanf("1.23 1.23E-03", "%f%f%n", &f1, &f2, &n1);
  s21_sscanf("1.23 1.23E-03", "%f%f%n", &res_f1, &res_f2, &res_n1);
  ck_assert_uint_eq(n1, res_n1);

  sscanf("1.23 1.23E-03", "%f%f%n%n", &f1, &f2, &n1, &n2);
  s21_sscanf("1.23 1.23E-03", "%f%f%n%n", &res_f1, &res_f2, &res_n1, &res_n2);
  ck_assert_uint_eq(n1, res_n1);
  ck_assert_uint_eq(n2, res_n2);
}

START_TEST(s21_sscanf_p_test) {
  void *a;
  void *res_a;
  sscanf("0x16fa9b16c", "%p", &a);
  s21_sscanf("0x16fa9b16c", "%p", &res_a);
  ck_assert_ptr_eq(a, res_a);
}

//_________________________________________________NEXT COME THE ADDITIONAL TASK
// PART 3 TESTS: LIKE C# FUNCTIONS

START_TEST(s21_insert_test) {
  const char *src = "Hello!";
  const char *str = ", world";
  char *new_str = (char *)s21_insert(src, str, 5);

  if (new_str) {
    ck_assert_str_eq(new_str, "Hello, world!");
    free(new_str);
  }

  new_str = (char *)s21_insert("Hello!", ", world!!", 5);
  if (new_str) {
    ck_assert_str_eq(new_str, "Hello, world!!!");
    free(new_str);
  }

  char *new_str3 = NULL;
  new_str3 = s21_insert("", ", world!!", 0);
  if (new_str3) {
    ck_assert_str_eq(new_str3, ", world!!");
    free(new_str3);
  }

  new_str = s21_insert("Hello!", ", world!!", 25);
  ck_assert(new_str == s21_NULL);

  new_str = s21_insert("", "", 0);
  ck_assert_str_eq(new_str, "");
}
END_TEST

START_TEST(s21_trim_test) {
  char *trimmed_str;
  const char *str_to_trim = " \n   Hello, world!  \n!";
  trimmed_str = (char *)s21_trim(str_to_trim, " H!\nd");
  if (trimmed_str) {
    ck_assert_str_eq(trimmed_str, "ello, worl");
    free(trimmed_str);
  }

  const char *str_to_trim2 = "xxx Hello, world! xxx ---";
  const char *format_str = "x -";
  trimmed_str = s21_trim(str_to_trim2, format_str);
  if (trimmed_str) {
    ck_assert_str_eq(trimmed_str, "Hello, world!");
    free(trimmed_str);
  }
}
END_TEST

START_TEST(s21_to_upper_test) {
  const char str1[] = "hella hello motherfucker";
  const char str2[] = "pRiVeT";

  char *new_str = s21_to_upper(str1);

  if (new_str) {
    ck_assert_str_eq(new_str, "HELLA HELLO MOTHERFUCKER");
    free(new_str);
  }

  new_str = s21_to_upper(str2);

  if (new_str) {
    ck_assert_str_eq(new_str, "PRIVET");
    free(new_str);
  }
}
END_TEST

START_TEST(s21_to_lower_test) {
  const char str1[] = "hella HELLO motherfucker";
  const char str2[] = "pRiVeT";

  char *new_str = s21_to_lower(str1);

  if (new_str) {
    ck_assert_str_eq(new_str, "hella hello motherfucker");
    free(new_str);
  }

  new_str = s21_to_lower(str2);

  if (new_str) {
    ck_assert_str_eq(new_str, "privet");
    free(new_str);
  }
}
END_TEST

//_________________________________________________NEXT COME MAIN AND FUNCTION

Suite *string_test_suite(void) {
  Suite *string_suite = suite_create("string_functions");
  TCase *test_case = tcase_create("string_test");

  // тесты функций s21_string.a: обязательное задание №1

  tcase_add_test(test_case, s21_strlen_test);
  tcase_add_test(test_case, s21_strcspn_test);
  tcase_add_test(test_case, s21_strerror_test);

  tcase_add_test(test_case, s21_strtok_test);
  tcase_add_test(test_case, s21_memcmp_test);
  tcase_add_test(test_case, s21_strncmp_test);

  tcase_add_test(test_case, s21_strstr_test);
  tcase_add_test(test_case, s21_strrchr_test);
  tcase_add_test(test_case, s21_strpbrk_test);

  tcase_add_test(test_case, s21_memcpy_test);
  tcase_add_test(test_case, s21_memset_test);
  tcase_add_test(test_case, s21_strncpy_test);

  tcase_add_test(test_case, s21_memchr_test);
  tcase_add_test(test_case, s21_strchr_test);
  tcase_add_test(test_case, s21_strncat_test);

  // тесты sprintf: обязательное задание №2

  tcase_add_test(test_case, s21_sprintf_c_test);
  tcase_add_test(test_case, s21_sprintf_d_test);
  tcase_add_test(test_case, s21_sprintf_f_test);
  tcase_add_test(test_case, s21_sprintf_s_test);
  tcase_add_test(test_case, s21_sprintf_u_test);
  tcase_add_test(test_case, s21_sprintf_trash_test);

  // тесты sprintf: дополнительное задание №1

  tcase_add_test(test_case, s21_sprintf_e_test);
  tcase_add_test(test_case, s21_sprintf_g_test);
  tcase_add_test(test_case, s21_sprintf_o_test);
  tcase_add_test(test_case, s21_sprintf_p_test);
  tcase_add_test(test_case, s21_sprintf_x_hex_sharp_star_test);

  // тесты sscanf: дополнительное задание №2

  tcase_add_test(test_case, s21_sscanf_d_test);
  tcase_add_test(test_case, s21_sscanf_u_test);
  tcase_add_test(test_case, s21_sscanf_i_test);
  tcase_add_test(test_case, s21_sscanf_c_test);
  tcase_add_test(test_case, s21_sscanf_s_test);
  tcase_add_test(test_case, s21_sscanf_o_test);
  tcase_add_test(test_case, s21_sscanf_f_test);
  tcase_add_test(test_case, s21_sscanf_n_test);
  tcase_add_test(test_case, s21_sscanf_p_test);
  tcase_add_test(test_case, s21_sscanf_xX_test);

  // тесты функций С#: дополнительное задание №3

  tcase_add_test(test_case, s21_trim_test);
  tcase_add_test(test_case, s21_insert_test);
  tcase_add_test(test_case, s21_to_upper_test);
  tcase_add_test(test_case, s21_to_lower_test);

  suite_add_tcase(string_suite, test_case);

  return string_suite;
}

int main(void) {
  s21_size_t tests_failed = 0;
  Suite *test_suite;
  SRunner *sr;

  test_suite = string_test_suite();
  sr = srunner_create(test_suite);
  srunner_run_all(sr, CK_VERBOSE);
  tests_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (tests_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
