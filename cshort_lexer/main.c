// main.c
#include <stdio.h>
#include "lexer.h"

const char* tokenToStr(TokenType type) {
    switch (type) {
        case TOKEN_ID: return "ID";
        case TOKEN_INT: return "INT";
        case TOKEN_REAL: return "REAL";
        case TOKEN_CHAR: return "CHAR";
        case TOKEN_STRING: return "STRING";
        case TOKEN_IF: return "IF";
        case TOKEN_ELSE: return "ELSE";
        case TOKEN_WHILE: return "WHILE";
        case TOKEN_FOR: return "FOR";
        case TOKEN_RETURN: return "RETURN";
        case TOKEN_VOID: return "VOID";
        case TOKEN_CHAR_T: return "CHAR_T";
        case TOKEN_INT_T: return "INT_T";
        case TOKEN_FLOAT_T: return "FLOAT_T";
        case TOKEN_BOOL_T: return "BOOL_T";
        case TOKEN_MAIS: return "MAIS";
        case TOKEN_MENOS: return "MINUS";
        case TOKEN_MUL: return "MULTIPLICACAO";
        case TOKEN_DIV: return "DIV";
        case TOKEN_IGUAL: return "IGUAL";
        case TOKEN_IGUALDADE: return "IGUALDADE";
        case TOKEN_NE: return "NEGACAO";
        case TOKEN_MAIORQUE: return "MAIOR QUE";
        case TOKEN_MENORQUE: return "MENOR QUE";
        case TOKEN_MAIORouIGUAL: return "MAIOR OU IGUAL";
        case TOKEN_MENORouIGUAL: return "MENOR OU IGUAL";
        case TOKEN_AND: return "AND";
        case TOKEN_OR: return "OR";
        case TOKEN_NOT: return "NOT";
        case TOKEN_ABREPAR: return "ABRE PARENTESE";
        case TOKEN_FECHAPAR: return "FECHA PARENTESE";
        case TOKEN_ABRECOL: return "ABRE COLCHETE";
        case TOKEN_FECHACOL: return "FECHA COLCHETE";
        case TOKEN_ABRECHAVE: return "ABRE CHAVE";
        case TOKEN_FECHACHAVE: return "FECHA CHAVE";
        case TOKEN_PONTOVIRGULA: return "PONTO E VIRGULA";
        case TOKEN_VIRGULA: return "VIRGULA";
        case TOKEN_COMMENT: return "COMENTARIO";
        case TOKEN_INVALID: return "INVALID";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

int main() {
    FILE *file = fopen("entrada.cshort", "r");
    if (!file) {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    initLexer(file);
    Token token;
    do {
        token = getNextToken();
        printf("[Linha %d] Token: %-15s Lexema: %s\n", token.line, tokenToStr(token.type), token.lexeme);
    } while (token.type != TOKEN_EOF);

    fclose(file);
    return 0;
}
