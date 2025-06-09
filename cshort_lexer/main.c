// main.c
#include <stdio.h>
#include "lexer.h"

const char* tokenToStr(TokenType type) {
    switch (type) {
        case ID: return "ID";
        case INT: return "INT - PR";
        case REAL: return "REAL - PR";
        case CHAR: return "CHAR - PR";
        case STRING: return "STRING";
        case IF: return "IF - PR";
        case ELSE: return "ELSE - PR";
        case WHILE: return "WHILE - PR";
        case FOR: return "FOR - PR";
        case RETURN: return "RETURN - PR";
        case VOID: return "VOID";
        case CHAR_T: return "CHAR_T";
        case INT_T: return "INT_T";
        case FLOAT_T: return "FLOAT_T";
        case BOOL_T: return "BOOL_T";
        case MAIS: return "SOMA";
        case MENOS: return "MENOS";
        case MUL: return "MULTIPLICACAO";
        case DIV: return "DIVISAO";
        case IGUAL: return "IGUAL";
        case IGUALDADE: return "IGUALDADE";
        case NEGACAO: return "NEGACAO";
        case MAIORQUE: return "MAIOR QUE";
        case MENORQUE: return "MENOR QUE";
        case MAIORouIGUAL: return "MAIOR OU IGUAL";
        case MENORouIGUAL: return "MENOR OU IGUAL";
        case AND: return "AND";
        case OR: return "OR";
        case NOT: return "NOT";
        case ABREPARENTESE: return "ABRE PARENTESE";
        case FECHAPARENTESE: return "FECHA PARENTESE";
        case ABRECOLCHETE: return "ABRE COLCHETE";
        case FECHACOLCHETE: return "FECHA COLCHETE";
        case ABRECHAVE: return "ABRE CHAVE";
        case FECHACHAVE: return "FECHA CHAVE";
        case PONTOVIRGULA: return "PONTO E VIRGULA";
        case VIRGULA: return "VIRGULA";
        case COMMENT: return "COMENTARIO";
        case INVALID: return "INVALID";
        case TOKEN_EOF: return "FIM ARQUIVO";
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
