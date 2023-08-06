CC=gcc
FLAGS=-g -Wall -Wextra -Werror -std=c11
TEST_FLAGS=-lcheck -lpthread -pthread -lm
TEST = tests/test
SRC = $(wildcard s21_*.c)
OBJ = $(patsubst s21_%.c, s21_%.o, $(SRC))

TEST_SRC = $(wildcard $(TEST)*.c)
TEST_OBJ = $(patsubst $(TEST)%.c, $(TEST)%.o, $(TEST_SRC))

ifeq ($(shell uname -s),Linux)
	TEST_FLAGS += -lsubunit -lgcov
endif

all: s21_string.a

s21_string.a: $(OBJ)
	ar rc s21_string.a $^
	ranlib s21_string.a

test.a: $(TEST_OBJ)
	ar rc $(TEST).a $^
	ranlib $(TEST).a

covered:
	$(CC) $(FLAGS) -c $(SRC) --coverage
	ar rc s21_string.a $(OBJ)
	ranlib s21_string.a 

check.c:
	checkmk asd.check > check.c

style: clean
	cp ../materials/linters/.clang-format ../src/.clang-format
	clang-format -style=Google -n *.c *.h
	rm .clang-format
	
test: clean covered test.a
	$(CC) $(FLAGS) --coverage -o $(TEST)_me.o $(TEST)_me.c -c
	$(CC) $(TEST)_me.o $(TEST).a s21_string.a -o $(TEST) --coverage $(FLAGS) $(TEST_FLAGS)
	./$(TEST)

leaks: test
ifeq ($(shell uname -s), Linux)
	valgrind --leak-check=yes  ./$(TEST)
else
	leaks --atExit -- ./$(TEST)
endif

clean:
	rm -rf *.o *.a *.gcda *.gcno *.gcov *.info *.html $(TEST)*.gcda $(TEST)*.gcno $(TEST)*.o $(TEST)*.a $(TEST) gcov_report

rebuild: clean all

gcov_report: test
	lcov -t "s21_string" -o s21_string.info -c -d .
	genhtml -o gcov_report s21_string.info