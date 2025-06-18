// tokens.h
#ifndef TOKENS_H
#define TOKENS_H

// Enumeração de todos os tipos de tokens reconhecidos pelo analisador léxico
typedef enum {
    // Identificadores e constantes
    ID,        // identificador: variável, nome de função, etc.
    INT,       // constante inteira
    REAL,      // constante real (ponto flutuante)
    CHAR,      // constante de caractere
    STRING,    // constante de string

    // Palavras-chave da linguagem
    IF,
    ELSE,
    WHILE,
    FOR,
    RETURN,
    VOID,
    CHAR_T,
    INT_T,
    FLOAT_T,
    BOOL_T,

    // Operadores aritméticos, relacionais e lógicos
    MAIS,      // +
    MENOS,     // -
    MUL,       // *
    DIV,       // /
    IGUAL,    // =
    IGUALDADE,        // ==
    NEGACAO,        // !=
    MAIORQUE,        // <
    MENORQUE,        // >
    MAIORouIGUAL,        // <=
    MENORouIGUAL,        // >=
    AND,       // &&
    OR,        // ||
    NOT,       // !

    // Delimitadores
    ABREPARENTESE,    // (
    FECHAPARENTESE,    // )
    ABRECOLCHETE,  // [
    FECHACOLCHETE,  // ]
    ABRECHAVE,    // {
    FECHACHAVE,    // }
    PONTOVIRGULA,      // ;
    VIRGULA,     // ,

    // Comentário
    COMMENT,   // /* ... */

    // Erro léxico ou fim de arquivo
    INVALID,
    TOKEN_EOF
} TokenType;

// Estrutura para representar um token
typedef struct {
    TokenType type;
    char lexeme[256];
    int line;
} Token;

#endif
