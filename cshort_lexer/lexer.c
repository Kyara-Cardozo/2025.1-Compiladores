// lexer.c
#include "lexer.h"
#include <ctype.h>
#include <string.h>

static FILE *source;
static int currentChar;
static int currentLine = 1;

void initLexer(FILE *sourceFile) {
    source = sourceFile;
    currentChar = fgetc(source);
    currentLine = 1;
}

static void advance() {
    currentChar = fgetc(source);
    if (currentChar == '\n') currentLine++;
}

static Token makeToken(TokenType type, const char *lexeme) {
    Token token;
    token.type = type;
    strncpy(token.lexeme, lexeme, sizeof(token.lexeme));
    token.line = currentLine;
    return token;
}

Token getNextToken() {
    while (isspace(currentChar)) advance();

    if (currentChar == EOF)
        return makeToken(TOKEN_EOF, "EOF");

    if (isalpha(currentChar) || currentChar == '_') {
        char buffer[256] = {0};
        int i = 0;
        do {
            buffer[i++] = currentChar;
            advance();
        } while (isalnum(currentChar) || currentChar == '_');
        
        // Palavras-chave
        if (strcmp(buffer, "if") == 0) return makeToken(TOKEN_IF, buffer);
        if (strcmp(buffer, "else") == 0) return makeToken(TOKEN_ELSE, buffer);
        if (strcmp(buffer, "while") == 0) return makeToken(TOKEN_WHILE, buffer);
        if (strcmp(buffer, "for") == 0) return makeToken(TOKEN_FOR, buffer);
        if (strcmp(buffer, "return") == 0) return makeToken(TOKEN_RETURN, buffer);
        if (strcmp(buffer, "void") == 0) return makeToken(TOKEN_VOID, buffer);
        if (strcmp(buffer, "char") == 0) return makeToken(TOKEN_CHAR_T, buffer);
        if (strcmp(buffer, "int") == 0) return makeToken(TOKEN_INT_T, buffer);
        if (strcmp(buffer, "float") == 0) return makeToken(TOKEN_FLOAT_T, buffer);
        if (strcmp(buffer, "bool") == 0) return makeToken(TOKEN_BOOL_T, buffer);
        
        return makeToken(TOKEN_ID, buffer);
    }

    if (isdigit(currentChar)) {
        char buffer[256] = {0};
        int i = 0;
        do {
            buffer[i++] = currentChar;
            advance();
        } while (isdigit(currentChar));

        if (currentChar == '.') {
            buffer[i++] = currentChar;
            advance();
            while (isdigit(currentChar)) {
                buffer[i++] = currentChar;
                advance();
            }
            return makeToken(TOKEN_REAL, buffer);
        }

        return makeToken(TOKEN_INT, buffer);
    }

    if (currentChar == '\'') {
        char buffer[4] = {0};
        advance(); buffer[0] = currentChar;
        advance();
        if (currentChar == '\'') {
            advance();
            return makeToken(TOKEN_CHAR, buffer);
        }
    }

    if (currentChar == '"') {
        char buffer[256] = {0};
        int i = 0;
        advance();
        while (currentChar != '"' && currentChar != EOF) {
            buffer[i++] = currentChar;
            advance();
        }
        advance();
        return makeToken(TOKEN_STRING, buffer);
    }

    // Comentários
    if (currentChar == '/') {
    advance();
    if (currentChar == '*') {
        // Comentário de bloco: /* ... */
        advance();
        while (currentChar != EOF) {
            if (currentChar == '*') {
                advance();
                if (currentChar == '/') {
                    advance();
                    return makeToken(TOKEN_COMMENT, "Comentario em bloco"); 
                }
            } else {
                advance();
            }
        }
        // Comentário de bloco não fechado
        return makeToken(TOKEN_INVALID, "Comentário de bloco não fechado");
    } else if (currentChar == '/') {
        // Comentário de linha: // ...
        advance();
        while (currentChar != '\n' && currentChar != EOF) {
            advance();
        }
        return makeToken(TOKEN_COMMENT, "Comentario de linha"); 
    } else {
        return makeToken(TOKEN_DIV, "/");
    }
}


    // Operadores e delimitadores
    char ch = currentChar;
    advance();

    switch (ch) {
        case '+': return makeToken(TOKEN_MAIS, "+");
        case '-': return makeToken(TOKEN_MENOS, "-");
        case '*': return makeToken(TOKEN_MUL, "*");
        case '=': return (currentChar == '=' ? (advance(), makeToken(TOKEN_IGUALDADE, "==")) : makeToken(TOKEN_IGUAL, "="));
        case '!': return (currentChar == '=' ? (advance(), makeToken(TOKEN_NE, "!=")) : makeToken(TOKEN_NOT, "!"));
        case '<': return (currentChar == '=' ? (advance(), makeToken(TOKEN_MAIORouIGUAL, "<=")) : makeToken(TOKEN_MAIORQUE, "<"));
        case '>': return (currentChar == '=' ? (advance(), makeToken(TOKEN_MENORouIGUAL, ">=")) : makeToken(TOKEN_MENORQUE, ">"));
        case '&': if (currentChar == '&') { advance(); return makeToken(TOKEN_AND, "&&"); }
        case '|': if (currentChar == '|') { advance(); return makeToken(TOKEN_OR, "||"); }
        case ';': return makeToken(TOKEN_PONTOVIRGULA, ";");
        case ',': return makeToken(TOKEN_VIRGULA, ",");
        case '(': return makeToken(TOKEN_ABREPAR, "(");
        case ')': return makeToken(TOKEN_FECHAPAR, ")");
        case '[': return makeToken(TOKEN_ABRECOL, "[");
        case ']': return makeToken(TOKEN_FECHACOL, "]");
        case '{': return makeToken(TOKEN_ABRECHAVE, "{");
        case '}': return makeToken(TOKEN_FECHACHAVE, "}");
    }

    return makeToken(TOKEN_INVALID, "?");
}
