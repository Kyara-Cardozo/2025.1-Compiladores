// tokens.h
#ifndef TOKENS_H
#define TOKENS_H

// Enumeração de todos os tipos de tokens reconhecidos pelo analisador léxico
typedef enum {
    // Identificadores e constantes
    TOKEN_ID,        // identificador: variável, nome de função, etc.
    TOKEN_INT,       // constante inteira
    TOKEN_REAL,      // constante real (ponto flutuante)
    TOKEN_CHAR,      // constante de caractere
    TOKEN_STRING,    // constante de string

    // Palavras-chave da linguagem
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_WHILE,
    TOKEN_FOR,
    TOKEN_RETURN,
    TOKEN_VOID,
    TOKEN_CHAR_T,
    TOKEN_INT_T,
    TOKEN_FLOAT_T,
    TOKEN_BOOL_T,

    // Operadores aritméticos, relacionais e lógicos
    TOKEN_MAIS,      // +
    TOKEN_MENOS,     // -
    TOKEN_MUL,       // *
    TOKEN_DIV,       // /
    TOKEN_IGUAL,    // =
    TOKEN_IGUALDADE,        // ==
    TOKEN_NE,        // !=
    TOKEN_MAIORQUE,        // <
    TOKEN_MENORQUE,        // >
    TOKEN_MAIORouIGUAL,        // <=
    TOKEN_MENORouIGUAL,        // >=
    TOKEN_AND,       // &&
    TOKEN_OR,        // ||
    TOKEN_NOT,       // !

    // Delimitadores
    TOKEN_ABREPAR,    // (
    TOKEN_FECHAPAR,    // )
    TOKEN_ABRECOL,  // [
    TOKEN_FECHACOL,  // ]
    TOKEN_ABRECHAVE,    // {
    TOKEN_FECHACHAVE,    // }
    TOKEN_PONTOVIRGULA,      // ;
    TOKEN_VIRGULA,     // ,

    // Comentário
    TOKEN_COMMENT,   // /* ... */

    // Erro léxico ou fim de arquivo
    TOKEN_INVALID,
    TOKEN_EOF
} TokenType;

// Estrutura para representar um token
typedef struct {
    TokenType type;
    char lexeme[256];
    int line;
} Token;

#endif
