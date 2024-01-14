all: build

run:
	./main

build:
	yacc --debug -d -Wall -o src/y.tab.c src/parser.y
	flex -o src/lex.yy.c src/lexer.l
	gcc -g src/main.c src/types.c src/lex.yy.c src/y.tab.c -o main

%PHONY: clean
clean:
	rm -f src/lex.yy.c main src/y.tab.c src/y.tab.h

run:
	./main

.PHONY:	clean
