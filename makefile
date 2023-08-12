FLAGS=-g -Wall -Wextra -Werror -std=c11
TEST_FLAGS=-lcheck -lpthread -pthread -lm
TEST_SRC = $(wildcard tests/*.c)
TEST_OBJECTIVE_FILES = $(patsubst tests/test%.c, tests/test%.o, $(TEST_SRC))

ifeq ($(shell uname -s),Linux)
	TEST_FLAGS += -lsubunit -lgcov
endif

all: s21_string.a

s21_string.a: 
	ar rc s21_string.a $^
	ranlib s21_string.a

test.a: $(TEST_OBJECTIVE_FILES)
	ar rc tests/test.a $^
	ranlib tests/test.a

covered:
	gcc $(FLAGS) -c standart_functions/*.c sprint_scan/*.c c_sharp/*.c --coverage
	ar rc s21_string.a *.o
	ranlib s21_string.a 

check.c:
	checkmk asd.check > check.c

style: clean
	cp ../materials/linters/.clang-format ../src/.clang-format
	clang-format -style=Google -n *.c *.h
	rm .clang-format
	
test: clean covered test.a
	gcc $(FLAGS) --coverage -o tests/test_me.o tests/test_me.c -c
	gcc tests/test_me.o tests/test.a s21_string.a -o tests/test --coverage $(FLAGS) $(TEST_FLAGS)
	./tests/test

leaks: test
ifeq ($(shell uname -s), Linux)
	valgrind --leak-check=yes  ./tests/test
else
	leaks --atExit -- ./tests/test
endif

clean:
	rm -rf *.o *.a *.gcda *.gcno *.gcov *.info *.html tests/test*.gcda tests/test*.gcno tests/test*.o tests/test*.a tests/test gcov_report

rebuild: clean all

gcov_report: test
	lcov -t "s21_string" -o s21_string.info -c -d .
	genhtml -o gcov_report s21_string.info