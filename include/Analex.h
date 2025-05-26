#ifndef ANALEX_H
#define ANALEX_H
#include <stdio.h>

#define MAX_LEXEMA 100

// Categorias de tokens
typedef enum {
    TK_ID,          // Identificadores (ex: 'x', 'var1')
    TK_INT,         // Constantes inteiras (ex: 42)
    TK_FLOAT,       // Constantes reais (ex: 3.14)
    TK_CHAR,        // Constantes char (ex: 'a')
    TK_STRING,      // Constantes string (ex: "texto")
    TK_OPERATOR,    // Operadores (+, -, *, /, ==, etc.)
    TK_DELIMITER,   // Delimitadores (;, ,, (), {}, [])
    TK_KEYWORD,     // Palavras-chave (int, char, if, else)
    TK_EOF,         // Fim de arquivo
    TK_ERROR        // Erro léxico
} TokenType;

// Palavras-chave (enum para facilitar verificação)
typedef enum {
    KW_INT,
    KW_CHAR,
    KW_FLOAT,
    KW_BOOL,
    KW_VOID,
    KW_IF,
    KW_ELSE,
    KW_WHILE,
    KW_FOR,
    KW_RETURN
} KeywordType;

// Operadores (enum para códigos)
typedef enum {
    OP_ASSIGN,      // =
    OP_EQ,          // ==
    OP_NE,          // !=
    OP_LT,          // <
    OP_LE,          // <=
    OP_GT,          // >
    OP_GE,          // >=
    OP_ADD,         // +
    OP_SUB,         // -
    OP_MUL,         // *
    OP_DIV,         // /
    OP_AND,         // &&
    OP_OR,          // ||
    OP_NOT          // !
} OperatorType;

// Delimitadores (enum para códigos)
typedef enum {
    DELIM_SEMICOLON,    // ;
    DELIM_COMMA,        // ,
    DELIM_LPAREN,       // (
    DELIM_RPAREN,       // )
    DELIM_LBRACE,       // {
    DELIM_RBRACE,       // }
    DELIM_LBRACKET,     // [
    DELIM_RBRACKET      // ]
} DelimiterType;

// Estrutura do token
typedef struct {
    TokenType type;
    union {
        char lexema[MAX_LEXEMA];  // Para IDs, strings
        int int_value;            // Para TK_INT
        float float_value;        // Para TK_FLOAT
        char char_value;          // Para TK_CHAR
        OperatorType op;          // Para TK_OPERATOR
        DelimiterType delim;      // Para TK_DELIMITER
        KeywordType kw;           // Para TK_KEYWORD
    };
} Token;

// Protótipos
Token nextToken(FILE *source);
const char* tokenTypeToString(TokenType type);

#endif