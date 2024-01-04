FLAGS_MACOS = -L $(shell brew --prefix)/lib -lcheck
SLFLAGS = -lcheck -lsubunit
MACOS_FLAG = 0

# флаги специальных библиотек для сборки на макос
ifeq ($(shell uname), Darwin)
SLFLAGS = $(FLAGS_MACOS)
MACOS_FLAG = 1
endif

CC=gcc # компилятор
LFLAGS=-g # для сборки с отладочной информацией
LIBFLAGS=-lm # для линковки c математической либой
CFLAGS=-Wall -Wextra -Werror -std=c11 -pedantic # для обработки ошибок
GCOVFLAGS=--coverage -fprofile-arcs -ftest-coverage # для анализа покрытия

# указывает флаги для сборки на маке с камнем M1
ifeq ($(shell uname -m), arm64)
CFLAGS := $(filter-out -pedantic,$(CFLAGS))
CFLAGS += -arch arm64
endif

# указывает флаги для сборки на маке с камнем M2
ifeq ($(shell uname -m), arm64e)
CFLAGS := $(filter-out -pedantic,$(CFLAGS))
endif

# папка с файлами функций
SOURCES=functions/*.c
# сделать объектники файлов функций
OBJECTS=$(SOURCES:%.c=%.o)

rebuild: clean all

all: s21_string.a test gcov_report clean_o clean_a cpp open_report style

s21_string.a: 
	$(CC) $(CFLAGS) -c $(SOURCES)
	ar rc s21_string.a *.o

test: s21_string.a
	$(CC) $(CFLAGS) s21_tests.c s21_string.a $(SLFLAGS) $(LIBFLAGS) -o s21_tests
	./s21_tests

gcov_report:
	$(CC) $(CFLAGS) s21_tests.c $(SOURCES) s21_string.a $(SLFLAGS) $(LIBFLAGS) -o s21_gcov $(GCOVFLAGS)
	./s21_gcov
	lcov -t "gcov_report" -o s21_gcov_report.info -c -d .
	genhtml -o s21_report s21_gcov_report.info

open_report:
	@open s21_report/index.html
# @google-chrome s21_report/index.html 2> /dev/null 


# очистка
clean: clean_s21_report clean_o clean_a
	rm -rf s21_tests s21_gcov *.gcno *.gcda *.info *.html *.css *.png *.deb

clean_o:    
	@rm -f *.o

clean_a:    
	@rm -f *.a

clean_s21_report:
	@rm -rf report s21_report
	

# тесты стиля и cpp
cpp:    
	@cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h
	@cppcheck --enable=all --suppress=missingIncludeSystem --suppress=unusedFunction functions/*.c functions/*.h

style:
	@cd resources/scripts && bash ./style_test.sh || (echo "Style tests failed")

# проверка наличия и установка зависимостей
check:    
	@cd resources/scripts && bash ./install_brew.sh || (echo "install_brew.sh failed")
	@cd resources/scripts && bash ./check_gcov_lcov.sh || (echo "check_gcov_lcov.sh failed")


# исключения для аналогичных имён файлов
.PHONY: make clean cppcheck style memcheck test gcov_report open_report cpp
