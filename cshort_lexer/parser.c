// parser.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "lexer.h"
#include "symtab.h"

// Enum para representar o tipo de declaração
typedef enum
{
    DECL_VAR,
    DECL_PROT,
    DECL_PROT_UNICO
} DeclKind;

// Função auxiliar para avançar o token
static void match(TokenType expected)
{
    if (t.type == expected)
    {
        advanceToken();
    }
    else
    {
        printf("Erro de sintaxe na linha %d: esperado '%s', encontrado '%s'\n",
               t.line, tokenToStr(expected), t.lexeme);

        exit(1);
    }
}

// <tipo> ::= int | float | char | bool | void
static void parseTipo()
{
    if (t.type == INT_T || t.type == FLOAT_T || t.type == CHAR_T ||
        t.type == BOOL_T || t.type == VOID)
    {
        advanceToken();
    }
    else
    {
        printf("Erro de sintaxe na linha %d: tipo esperado, encontrado '%s'\n",
               t.line, t.lexeme);
        exit(1);
    }
}

// <expressao> ::= INT | ID
static void parseExpressao()
{
    if (t.type == INT)
    {
        advanceToken();
    }
    else if (t.type == ID)
    {
        if (!lookupSymbol(t.lexeme))
        {
            printf("Erro semântico: identificador '%s' usado mas não declarado.\n", t.lexeme);
            exit(1);
        }
        advanceToken();
    }
    else
    {
        printf("Erro de sintaxe na linha %d: expressão inválida, encontrado '%s'\n",
               t.line, t.lexeme);
        exit(1);
    }
}

// <comando> ::= return <expressao> ;
static void parseComando()
{
    if (t.type == RETURN)
    {
        advanceToken();
        parseExpressao();
        match(PONTOVIRGULA);
    }
    else
    {
        printf("Erro de sintaxe na linha %d: comando inválido, encontrado '%s'\n",
               t.line, t.lexeme);
        exit(1);
    }
}

// <bloco> ::= { <comando>* }
static void parseBloco()
{
    match(ABRECHAVE);
    while (t.type != FECHACHAVE && t.type != TOKEN_EOF)
    {
        parseComando();
    }
    match(FECHACHAVE);
}

// <decl> ::= <tipo> ID ( ) | <tipo> ID ;
static DeclKind decl()
{
    parseTipo();

    if (t.type != ID)
    {
        printf("Erro de sintaxe na linha %d: identificador esperado após tipo.\n", t.line);
        exit(1);
    }

    char nome[256];
    strcpy(nome, t.lexeme);
    advanceToken();

    if (t.type == ABREPARENTESE)
    {
        advanceToken();
        match(FECHAPARENTESE);

        if (t.type == ABRECHAVE)
        { // Agora t já aponta pro próximo
            insertSymbol(nome, SYMBOL_FUNC);
            return DECL_PROT_UNICO; // Indica definição
        }
        else if (t.type == PONTOVIRGULA)
        {
            insertSymbol(nome, SYMBOL_FUNC);
            return DECL_PROT; // Indica protótipo
        }
        else
        {
            printf("Erro de sintaxe na linha %d: esperado '{' ou ';' após cabeçalho da função.\n", t.line);
            exit(1);
        }
    }

    else if (t.type == PONTOVIRGULA)
    {
        insertSymbol(nome, SYMBOL_VAR);
        return DECL_VAR;
    }
    else if (t.type == VIRGULA)
    {
         insertSymbol(nome, SYMBOL_VAR); 
        do
        {
            advanceToken(); // Avança para o próximo identificador
            if (t.type != ID)
            {
                printf("Erro de sintaxe na linha %d: identificador esperado após ','.\n", t.line);
                exit(1);
            }
            insertSymbol(t.lexeme, SYMBOL_VAR);
            advanceToken();
        } while (t.type == VIRGULA);

        if (t.type == PONTOVIRGULA)
        {
            return DECL_VAR;
        }
        else
        {
            printf("Erro de sintaxe na linha %d: esperado ';' após declaração de variável.\n", t.line);
            exit(1);
        }
    }
    else
    {
        printf("aqui else");
        printf("aqui2 else:%d \n", t.type);
        printf("Erro de sintaxe na linha %d: esperado '(' ou ';' após identificador.\n", t.line);
        exit(1);
    }
}

// <prog> ::= <decl> ;
void parseProgram()
{
    initParserTokens();

    while (t.type != TOKEN_EOF)
    {
        DeclKind tipoDecl = decl();
        if (tipoDecl == DECL_VAR || tipoDecl == DECL_PROT)
        {
            match(PONTOVIRGULA);
        }
        else if (tipoDecl == DECL_PROT_UNICO)
        {
            if (t.type == PONTOVIRGULA)
            {
                match(PONTOVIRGULA);
            }
            else if (t.type == ABRECHAVE)
            {
                parseBloco();
            }
            else
            {
                printf("Erro de sintaxe na linha %d: esperado '{' ou ';' após cabeçalho da função.\n", t.line);
                exit(1);
            }
        }
    }

    if (t.type != TOKEN_EOF)
    {
        printf("Erro de sintaxe: conteúdo inesperado após o fim do programa (linha %d).\n", t.line);
        printf("token encontrado  para t %d \n", t.type);
        printf("token encontrado  para tLookahead %d \n", tLookahead.type);

        exit(1);
    }

    printSymbolTable();
}
