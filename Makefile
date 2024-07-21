SHELL = /bin/sh

CC = gcc
CFLAGS = -std=c11 -Wall -Werror -Wextra
NCUR = -lncurses
LFLAGS = -lcheck -lm -lsubunit
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
TEST_FILE_NAME = test
LIB_NAME = s21_tetris.a
GCOV_NAME = gcov_tests.info
GAME_NAME = ./tetris
MAIN_FILE = s21_tetris.c

LIB_SRC = $(wildcard brick_game/tetris/*.c)
FRONT_SRC = $(wildcard gui/cli/*.c)
TEST_SRC = $(wildcard tests/*.c)

LIB_O = $(LIB_SRC:.c=.o)
TEST_O = $(TEST_SRC:.c=.o)

.PHONY: all install uninstall clean dvi dist test gcov_report clang_check clang_fix valgrind s21_tetris.a valgrind dvi_install

all: clean install run

clean:
	@rm -f $(LIB_NAME) $(TEST_FILE_NAME) ./lib/*.o
	@rm -rf ./brick_game/tetris/*.o ./tests/*.o *.gcno *.gcda ./report *.o ./lib/*.gcno
	@rm -rf $(TEST_FILE_NAME)
	@rm -rf html/
	@rm -rf latex/
	@rm -rf $(GAME_NAME).tgz
	@rm -rf $(GAME_NAME)

install: s21_tetris.a
	$(CC) $(CFLAGS) -c $(FRONT_SRC) -L. -l:$(LIB_NAME)
	$(CC) $(CFLAGS) $(MAIN_FILE) *.o -o tetris -lsubunit -L. -l:$(LIB_NAME) $(NCUR)
#	make clean
run:
	$(GAME_NAME)

uninstall:
	-rm -rf $(GAME_NAME) high_score.txt

s21_tetris.a: $(LIB_O)
	@ar rc $(LIB_NAME) $(LIB_O)
	@ranlib $(LIB_NAME)
	@rm -rf lib/*.o


test: $(TEST_O) $(LIB_NAME)
	@$(CC) $(CFLAGS) $(TEST_O) -o $(TEST_FILE_NAME) -L. -l:$(LIB_NAME) $(LFLAGS)
	@./$(TEST_FILE_NAME)

gcov_report: clean
	$(CC) $(CFLAGS) -c $(LIB_SRC) --coverage
	$(CC) $(CFLAGS) -c $(TEST_SRC) 
	$(CC) *.o -o $(TEST_FILE_NAME) $(LFLAGS) $(GCOV_FLAGS)
	./$(TEST_FILE_NAME)
	lcov -t "gcov_tests" -o $(GCOV_NAME) -c -d .
	genhtml -o report $(GCOV_NAME)
	rm -rf *.o *.gcno *.gcda *.gcov $(GCOV_NAME)
	rm -rf $(TEST_FILE_NAME)

valgrind: test
	-valgrind --tool=memcheck --leak-check=yes ./test

clang_check:    
	@cp ../materials/linters/.clang-format .clang-format
	find . -name '*.c' -exec clang-format -n {} \;
	find . -name '*.h' -exec clang-format -n {} \;
	@rm -rf .clang-format

clang_fix:
	@cp ../materials/linters/.clang-format .clang-format
	find . -name '*.c' -exec clang-format -i {} \;
	find . -name '*.h' -exec clang-format -i {} \;
	@rm -rf .clang-format

dist: uninstall install
	mkdir Tetris/
	cp $(GAME_NAME) Tetris/
	tar cvzf $(GAME_NAME).tgz Tetris/
	-rm -rf Tetris/

dvi:
	@doxygen Doxyfile
	@cd latex && xelatex refman.tex && cd ../

dvi_install:
	sudo apt-get install texlive-xetex
	sudo apt-get install texlive-lang-cyrillic