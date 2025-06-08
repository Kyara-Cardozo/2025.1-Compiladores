// parser.c
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "lexer.h"

// Token atual sendo analisado
static Token currentToken;

// Avança para o próximo token
static void advance()
{
    currentToken = getNextToken();
}

extern const char* tokenToStr(TokenType type);  // garante visibilidade

// Verifica se o token atual é o esperado e avança
static void match(TokenType expected)
{
    if (currentToken.type == expected)
    {
        advance();
    }
    else
    {
        printf("Erro de sintaxe na linha %d: esperado '%s', mas encontrei '%s'\n",
               currentToken.line,
               tokenToStr(expected),
               currentToken.lexeme);
        exit(1);
    }
}

// <tipo> ::= int | float | char | bool | void
static void parseTipo()
{
    switch (currentToken.type)
    {
    case TOKEN_INT_T:
    case TOKEN_FLOAT_T:
    case TOKEN_CHAR_T:
    case TOKEN_BOOL_T:
    case TOKEN_VOID:
        advance();
        break;
    default:
        printf("Erro na linha %d: tipo esperado, encontrado '%s'\n", currentToken.line, currentToken.lexeme);
        exit(1);
    }
}

// <expressao> ::= int | ID
static void parseExpressao()
{
    if (currentToken.type == TOKEN_INT || currentToken.type == TOKEN_ID)
    {
        advance();
    }
    else
    {
        printf("Erro na linha %d: expressão esperada, encontrado '%s'\n", currentToken.line, currentToken.lexeme);
        exit(1);
    }
}

// <comando> ::= return <expressao> ;
static void parseComando()
{
    if (currentToken.type == TOKEN_RETURN)
    {
        advance();
        parseExpressao();
        match(TOKEN_PONTOVIRGULA);
    }
    else
    {
        printf("Erro: comando inválido na linha %d\n", currentToken.line);
        exit(1);
    }
}

// <bloco> ::= { <comando>* }
static void parseBloco()
{
    match(TOKEN_ABRECHAVE);
    while (currentToken.type != TOKEN_FECHACHAVE && currentToken.type != TOKEN_EOF)
    {
        parseComando();
    }
    match(TOKEN_FECHACHAVE);
}

// <decl_funcao> ::= <tipo> ID ( ) <bloco>
static void parseDeclFuncao()
{
    parseTipo();
    match(TOKEN_ID);
    match(TOKEN_ABREPAR);
    match(TOKEN_FECHAPAR);
    parseBloco();
}

// <decl_variavel> ::= <tipo> ID ;
static void parseDeclVariavel()
{
    parseTipo();
    match(TOKEN_ID);
    match(TOKEN_PONTOVIRGULA);
}

// <programa> ::= <decl_funcao> | <decl_variavel>
void parseProgram()
{
    advance(); // inicia o primeiro token

    if (currentToken.type == TOKEN_INT_T || currentToken.type == TOKEN_FLOAT_T ||
        currentToken.type == TOKEN_CHAR_T || currentToken.type == TOKEN_BOOL_T ||
        currentToken.type == TOKEN_VOID)
    {
        Token lookahead = currentToken;
        advance(); // tipo
        if (currentToken.type != TOKEN_ID)
        {
            printf("Erro: identificador esperado após tipo (linha %d)\n", currentToken.line);
            exit(1);
        }

        advance(); // ID

        if (currentToken.type == TOKEN_ABREPAR)
        {
            // é uma função
            match(TOKEN_ABREPAR);
            match(TOKEN_FECHAPAR);
            parseBloco();
            printf("Função reconhecida com sucesso.\n");
        }
        else if (currentToken.type == TOKEN_PONTOVIRGULA)
        {
            // é uma variável
            printf("Declaração de variável reconhecida com sucesso.\n");
            match(TOKEN_PONTOVIRGULA);
        }
        else
        {
            printf("Erro: esperado '(' ou ';' após nome (linha %d)\n", currentToken.line);
            exit(1);
        }
    }
    else
    {
        printf("Erro: tipo esperado no início do programa (linha %d)\n", currentToken.line);
        exit(1);
    }

    if (currentToken.type != TOKEN_EOF)
    {
        printf("Erro: conteúdo inesperado após fim do programa (linha %d)\n", currentToken.line);
        exit(1);
    }
}
