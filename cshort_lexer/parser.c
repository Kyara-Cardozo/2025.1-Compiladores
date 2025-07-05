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

// <tipo> ::= int | float | char | bool
static void parseTipoSemVoid()
{
    if (t.type == INT_T || t.type == FLOAT_T || t.type == CHAR_T ||
        t.type == BOOL_T)
    {
        advanceToken();
    }
    else
    {
        printf("Erro de sintaxe na linha %d: o tipo de parametro '%s' é inválido\n",
               t.line, t.lexeme);
        exit(1);
    }
}

// <op_rel> ::= == | != | <= | < | => | >
static void parseOpRel()
{
    if (t.type == IGUALDADE || t.type == NEGACAO || t.type == MENORouIGUAL || t.type == MENORQUE ||
        t.type == MAIORouIGUAL || t.type == MAIORQUE)
    {
        advanceToken();
    }
    else
    {
        printf("Erro de sintaxe na linha %d: o operador '%s' é inválido\n",
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

// to do
//  <fator> ::= { = id [ '[' expr ']' ] | intcon | realcon | charcon | id '(' [expr { ',' expr } ] ')' | '(' expr ')' | '!' fator}
static void parseFator()
{
    if (t.type == ID)
    {
        char nome[256];
        strcpy(nome, t.lexeme);
        advanceToken();

        if (t.type == ABRECOLCHETE)
        {
            // Vetor: id [ expr ]
            advanceToken();
            parseExpr();
            match(FECHACOLCHETE);
        }
        else if (t.type == ABREPARENTESE)
        {
            // chama de função: id ( [expr {, expr}] )
            advanceToken();

            if (t.type != FECHAPARENTESE)
            {
                parseExpr();

                while (t.type == VIRGULA)
                {
                    advanceToken();
                    parseExpr();
                }
            }

            match(FECHAPARENTESE);
        }
        else
        {
        }
    }
    else if (t.type == INT || t.type == REAL || t.type == CHAR)
    {
        advanceToken();
    }
    else if (t.type == ABREPARENTESE)
    {
        advanceToken();
        parseExpr();
        match(FECHAPARENTESE);
    }
    else if (t.type == NOT)
    {
        advanceToken();
        parseFator();
    }
    else
    {
        printf("Erro de sintaxe na linha %d: fator inesperado: '%s'\n", t.line, t.lexeme);
        exit(1);
    }
}

// to do
// <termo> ::= fator {(* | / | &&) fator};
static void parseTermo()
{
    parseFator();

    while (t.type == MUL || t.type == DIV || t.type == AND)
    {
        advanceToken();
        parseFator();
    }
    exit(1);
}

// to do
// <expreSimp> ::= [+ | – ] termo {(+ | – | ||) termo};
static void parseExprSimp()
{
    if (t.type == MAIS || t.type == MENOS)
    {
        advanceToken();
    }

    parseTermo();

    while (t.type == MAIS || t.type == MENOS || t.type == OR)
    {
        advanceToken();
        parseTermo();
    }
    exit(1);
}

// to do
// <expre> ::= expr_simp [ op_rel expr_simp ];
static void parseExpr()
{
    parseExprSimp();

    if (t.type == IGUAL ||
        t.type == NEGACAO ||
        t.type == MENORQUE ||
        t.type == MAIORQUE ||
        t.type == MENORouIGUAL ||
        t.type == MAIORouIGUAL)
    {
        advanceToken();
        parseExprSimp();
    }
    exit(1);
}

// <atrib> ::= id [ '[' expr ']' ] = expr
// to do

static void parseAtrib()
{
    if (t.type != ID)
    {
        printf("Erro de sintaxe na linha %d: identificador esperado na atribuição.\n", t.line);
        exit(1);
    }

    advanceToken();

    if (t.type == ABRECOLCHETE)
    {
        advanceToken();
        parseExpr();
        match(FECHACOLCHETE);
    }

    match(IGUAL);
    parseExpr();
    exit(1);
}

static void parseCmd()
{
    if (t.type == IF)
    {
        advanceToken();
        match(ABREPARENTESE);
        parseExpr();
        match(FECHAPARENTESE);
        parseCmd();

        if (t.type == ELSE)
        {
            advanceToken();
            parseCmd();
        }
    }
    else if (t.type == WHILE)
    {
        advanceToken();
        match(ABREPARENTESE);
        parseExpr();
        match(FECHAPARENTESE);
        parseCmd();
    }
    else if (t.type == FOR)
    {
        advanceToken();
        match(ABREPARENTESE);

        if (t.type == ID)
        {
            parseAtrib();
        }
        match(PONTOVIRGULA);

        if (t.type != PONTOVIRGULA)
        {
            parseExpr();
        }
        match(PONTOVIRGULA);

        if (t.type == ID)
        {
            parseAtrib();
        }
        match(FECHAPARENTESE);
        parseCmd();
    }
    else if (t.type == RETURN)
    {
        advanceToken();

        if (t.type != PONTOVIRGULA)
        {
            parseExpr();
        }
        match(PONTOVIRGULA);
    }
    else if (t.type == ID)
    {
        char nome[256];
        strcpy(nome, t.lexeme);
        advanceToken();

        if (t.type == ABREPARENTESE)
        {
            // Chamada de função: id ( [expr { ',' expr}] )
            advanceToken();

            if (t.type != FECHAPARENTESE)
            {
                parseExpr();

                while (t.type == VIRGULA)
                {
                    advanceToken();
                    parseExpr();
                }
            }
            match(FECHAPARENTESE);
            match(PONTOVIRGULA);
        }
        else
        {
            // Atribuição: id = expr
            parseAtrib();
            match(PONTOVIRGULA);
        }
    }
    else if (t.type == ABRECHAVE)
    {
        parseBloco();
    }
    else if (t.type == PONTOVIRGULA)
    {
        // Comando vazio
        advanceToken();
    }
    else
    {
        printf("Erro de sintaxe na linha %d: comando inválido, encontrado '%s'\n", t.line, t.lexeme);
        exit(1);
    }
}

// <decl> ::= <tipo> ID ( ) | <tipo> ID ;
static DeclKind decl()
{
    char nome[256];
    strcpy(nome, t.lexeme);

    parseTipo();

    if (t.type != ID)
    {
        printf("Erro de sintaxe na linha %d: identificador esperado após tipo.\n", t.line);
        exit(1);
    }

    strcpy(nome, t.lexeme);
    advanceToken();

    if (t.type == ABREPARENTESE)
    {
        insertSymbol(nome, SYMBOL_FUNC);

        advanceToken();

        if (t.type != FECHAPARENTESE)
        {
            do
            {
                parseTipoSemVoid();

                if (t.type != ID)
                {
                    printf("Erro de sintaxe na linha %d: identificador esperado após tipo do parâmetro.\n", t.line);
                    exit(1);
                }

                insertSymbol(t.lexeme, SYMBOL_PARAM);
                advanceToken();

                if (t.type == VIRGULA) // Se houver uma vírgula, avança para o próximo parâmetro
                {
                    advanceToken();
                    // TODO: precisamos validar o cenário int func(int x, )
                }
                else if (t.type != FECHAPARENTESE) // Se não for vírgula ou fechamento de parêntese, é um erro
                {
                    printf("Erro de sintaxe na linha %d: esperado ',' ou ')' após parâmetro.\n", t.line);
                    exit(1);
                }

            } while (t.type != FECHAPARENTESE); // Continua até encontrar o fechamento do parêntese
        }

        match(FECHAPARENTESE);

        if (t.type == ABRECHAVE) // Definição da função
        {
            return DECL_PROT_UNICO;
        }
        else if (t.type == PONTOVIRGULA) // Protótipo da função
        {
            return DECL_PROT;
        }
        else
        {
            printf("Erro de sintaxe na linha %d: esperado '{' ou ';' após cabeçalho da função.\n", t.line);
            exit(1);
        }
    }
    else if (t.type == ABRECOLCHETE)
    {
        advanceToken();
        if (t.type != INT)
        {
            printf("Erro de sintaxe na linha %d: esperado constante inteira.\n");
            exit(0);
        }
        insertSymbol(nome, SYMBOL_VAR);
        advanceToken();
        if (t.type != FECHACOLCHETE)
        {
            printf("Erro de sintaxe na linha %d: esperado ']'.\n");
            exit(0);
        }
        advanceToken();
    }
    else if (t.type == PONTOVIRGULA) // Caso seja uma variável
    {
        insertSymbol(nome, SYMBOL_VAR);
        return DECL_VAR;
    }
    else if (t.type == VIRGULA) // Caso seja uma lista de variáveis
    {
        insertSymbol(nome, SYMBOL_VAR);
        do
        {
            advanceToken();
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
