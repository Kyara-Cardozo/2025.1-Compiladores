CC = gcc
CFLAGS = -Iinclude -Wall

all: lexer

lexer: src/lexer.c src/main.c
    $(CC) $(CFLAGS) $^ -o build/lexer

clean:
    rm -f build/*