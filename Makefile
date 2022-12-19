.DEFAULT_GOAL := build
build: bison main.c
	cc parse.tab.c main.c -o main

bison: parse.y
	bison -d parse.y

.PHONY: clean
clean:
	rm parse.tab.* main
