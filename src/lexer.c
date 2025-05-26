#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"

int line_number = 1;  // Contador de linhas

// Mapeia strings para palavras-chave
KeywordType checkKeyword(const char *lexema) {
    const char *keywords[] = {"int", "char", "float", "bool", "void", "if", "else", "while", "for", "return"};
    const KeywordType kw_types[] = {KW_INT, KW_CHAR, KW_FLOAT, KW_BOOL, KW_VOID, KW_IF, KW_ELSE, KW_WHILE, KW_FOR, KW_RETURN};
    
    for (int i = 0; i < 10; i++) {
        if (strcmp(lexema, keywords[i]) == 0) {
            return kw_types[i];
        }
    }
    return -1; // Não é palavra-chave
}

// Função principal do lexer
Token nextToken(FILE *source) {
    Token token;
    char c;
    int lexema_pos = 0;
    char lexema[MAX_LEXEMA] = {0};

    while ((c = fgetc(source)) != EOF) {
        // Ignora espaços em branco
        if (isspace(c)) {
            if (c == '\n') line_number++;
            continue;
        }

        // Estado inicial (q0)
        // Identificadores ou palavras-chave
        if (isalpha(c) || c == '_') {
            lexema[lexema_pos++] = c;
            while ((c = fgetc(source)) != EOF && (isalnum(c) || c == '_')) {
                lexema[lexema_pos++] = c;
            }
            ungetc(c, source);

            KeywordType kw = checkKeyword(lexema);
            if (kw != -1) {
                token.type = TK_KEYWORD;
                token.kw = kw;
            } else {
                token.type = TK_ID;
                strcpy(token.lexema, lexema);
            }
            return token;
        }
        // Números inteiros ou reais
        else if (isdigit(c)) {
            lexema[lexema_pos++] = c;
            while ((c = fgetc(source)) != EOF && isdigit(c)) {
                lexema[lexema_pos++] = c;
            }
            if (c == '.') {
                lexema[lexema_pos++] = c;
                while ((c = fgetc(source)) != EOF && isdigit(c)) {
                    lexema[lexema_pos++] = c;
                }
                ungetc(c, source);
                token.type = TK_FLOAT;
                token.float_value = atof(lexema);
            } else {
                ungetc(c, source);
                token.type = TK_INT;
                token.int_value = atoi(lexema);
            }
            return token;
        }
        // Constantes char
        else if (c == '\'') {
            lexema[lexema_pos++] = c;
            c = fgetc(source);
            if (c == '\\') { // Trata escapes como '\n'
                lexema[lexema_pos++] = c;
                c = fgetc(source);
                if (c == 'n' || c == '0' || c == 't' || c == '\'') {
                    lexema[lexema_pos++] = c;
                } else {
                    token.type = TK_ERROR;
                    return token;
                }
            } else if (c != '\'') {
                lexema[lexema_pos++] = c;
            } else {
                token.type = TK_ERROR;
                return token;
            }
            c = fgetc(source);
            if (c == '\'') {
                lexema[lexema_pos++] = c;
                token.type = TK_CHAR;
                strcpy(token.lexema, lexema);
            } else {
                token.type = TK_ERROR;
            }
            return token;
        }
        // Constantes string
        else if (c == '"') {
            lexema[lexema_pos++] = c;
            while ((c = fgetc(source)) != EOF && c != '"') {
                if (c == '\\') { // Trata escapes
                    lexema[lexema_pos++] = c;
                    c = fgetc(source);
                    if (c == 'n' || c == '0' || c == 't' || c == '"') {
                        lexema[lexema_pos++] = c;
                    } else {
                        token.type = TK_ERROR;
                        return token;
                    }
                } else {
                    lexema[lexema_pos++] = c;
                }
            }
            if (c == '"') {
                lexema[lexema_pos++] = c;
                token.type = TK_STRING;
                strcpy(token.lexema, lexema);
            } else {
                token.type = TK_ERROR;
            }
            return token;
        }
        // Operadores e delimitadores
        else {
            switch (c) {
                case '=':
                    c = fgetc(source);
                    if (c == '=') {
                        token.type = TK_OPERATOR;
                        token.op = OP_EQ;
                    } else {
                        ungetc(c, source);
                        token.type = TK_OPERATOR;
                        token.op = OP_ASSIGN;
                    }
                    return token;
                case '+':
                    token.type = TK_OPERATOR;
                    token.op = OP_ADD;
                    return token;
                case '-':
                    token.type = TK_OPERATOR;
                    token.op = OP_SUB;
                    return token;
                case '*':
                    token.type = TK_OPERATOR;
                    token.op = OP_MUL;
                    return token;
                case '/':
                    c = fgetc(source);
                    if (c == '*') {
                        // Trata comentários /* ... */
                        while ((c = fgetc(source)) != EOF) {
                            if (c == '*') {
                                c = fgetc(source);
                                if (c == '/') break;
                            }
                        }
                        if (c == EOF) {
                            token.type = TK_ERROR;
                            return token;
                        }
                        continue; // Volta ao início para pegar o próximo token
                    } else {
                        ungetc(c, source);
                        token.type = TK_OPERATOR;
                        token.op = OP_DIV;
                        return token;
                    }
                case ';':
                    token.type = TK_DELIMITER;
                    token.delim = DELIM_SEMICOLON;
                    return token;
                // ... (implemente outros operadores/delimitadores)
                default:
                    token.type = TK_ERROR;
                    return token;
            }
        }
    }

    token.type = TK_EOF;
    return token;
}