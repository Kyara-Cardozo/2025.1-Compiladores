#include <stdio.h>
#include "analex.h"

const char* tokenTypeToString(TokenType type) {
    switch (type) {
        case TK_ID: return "ID";
        case TK_INT: return "INT";
        case TK_FLOAT: return "FLOAT";
        case TK_CHAR: return "CHAR";
        case TK_STRING: return "STRING";
        case TK_OPERATOR: return "OPERATOR";
        case TK_DELIMITER: return "DELIMITER";
        case TK_KEYWORD: return "KEYWORD";
        case TK_EOF: return "EOF";
        case TK_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main() {
    FILE *source = fopen("programa.cshort", "r");
    if (!source) {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    Token token;
    do {
        token = nextToken(source);
        printf("Linha %d: [%s] ", line_number, tokenTypeToString(token.type));
        
        switch (token.type) {
            case TK_ID:
                printf("%s\n", token.lexema);
                break;
            case TK_INT:
                printf("%d\n", token.int_value);
                break;
            case TK_FLOAT:
                printf("%f\n", token.float_value);
                break;
            case TK_CHAR:
            case TK_STRING:
                printf("%s\n", token.lexema);
                break;
            case TK_OPERATOR:
                printf("%d\n", token.op);
                break;
            case TK_DELIMITER:
                printf("%d\n", token.delim);
                break;
            case TK_KEYWORD:
                printf("%d\n", token.kw);
                break;
            case TK_ERROR:
                printf("Erro léxico\n");
                break;
        }
    } while (token.type != TK_EOF && token.type != TK_ERROR);

    fclose(source);
    return 0;
}