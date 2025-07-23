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

void static parseExpr();
void static parseCmd();
DeclKind static parseFunc();
static void parseTermo();

static int labelCounter = 0;

static int generateLabel()
{
    return labelCounter++;
}

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
        if (!lookupSymbol(t.lexeme, SCOPO_LOCAL))
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

static void parseFator()
{
    if (t.type == ID)
    {
        char nome[256];
        strcpy(nome, t.lexeme);
        printf("\n LOAD %s\n", t.lexeme);

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
            // Chamada de função: id ( [expr {, expr}] )
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
        else if (t.type == PONTOVIRGULA)
        {
            // Atribuição ou comando vazio
            // advanceToken();
        }
        // else if(t.type == MAIS || t.type == MENOS || t.type == OR) {
        // if (t.type == MAIS)
        // {
        //     printf("\n ADD \n");
        // }
        // else if (t.type == MENOS)
        // {
        //     printf("\n SUB \n");
        // }
        // else if (t.type == OR)
        // {
        //     printf("\n OR \n");
        // }
        // advanceToken();
        // parseTermo();
        // }
    }
    else if (t.type == INT || t.type == REAL || t.type == CHAR)
    {
        // Constantes inteiras, reais ou caracteres
        printf("PUSH %s\n", t.lexeme);
        advanceToken();
    }
    else if (t.type == ABREPARENTESE)
    {
        // Expressão entre parênteses: ( expr )
        advanceToken();
        parseExpr();
        match(FECHAPARENTESE);
    }
    else if (t.type == NOT)
    {
        // Negação: ! fator
        advanceToken();
        parseFator();
    }
    else if (t.type == PONTOVIRGULA)
    {
        advanceToken();
    }

    else
    {
        // Erro de sintaxe para fatores inválidos
        printf("Erro de sintaxe na linha %d: fator inesperado: '%s'\n", t.line, t.lexeme);
        exit(1);
    }
}

// <termo> ::= fator {(* | / | &&) fator};
static void parseTermo()
{
    parseFator();

    while (t.type == MUL || t.type == DIV || t.type == AND)
    {
        if (t.type == MUL)
        {
            printf("\n MUL \n");
        }
        else if (t.type == DIV)
        {
            printf("\n DIV \n");
        }
        else if (t.type == AND)
        {
            printf("\n AND \n");
        }

        advanceToken();
        parseFator();
    }
}

// <expreSimp> ::= [+ | – ] termo {(+ | – | ||) termo};
static void parseExprSimp()
{
    if (t.type == MAIS || t.type == MENOS)
    {
        if (t.type == MENOS)
        {
            printf("\n SUB \n");
        }
        else if (t.type == MAIS)
        {
            printf("\n ADD \n");
        }
        advanceToken();
    }

    parseTermo();

    while (t.type == MAIS || t.type == MENOS || t.type == OR)
    {
        if (t.type == MAIS)
        {
            printf("\n ADD \n");
        }
        else if (t.type == MENOS)
        {
            printf("\n SUB \n");
        }
        else if (t.type == OR)
        {
            printf("\n OR \n");
        }
        advanceToken();

        parseTermo();
    }
}

// <expre> ::= expr_simp [ op_rel expr_simp ];
static void parseExpr()
{
    parseExprSimp();

    if (t.type == IGUAL ||
        t.type == IGUALDADE ||
        t.type == NEGACAO ||
        t.type == MENORQUE ||
        t.type == MAIORQUE ||
        t.type == MENORouIGUAL ||
        t.type == MAIORouIGUAL)
    {
        TokenType op = t.type;
        advanceToken();
        parseExprSimp();
        switch (op)
        {
        case IGUALDADE:
            printf("EQ\n");
            break;
        case NEGACAO:
            printf("NE\n");
            break;
        case MENORQUE:
            printf("LT\n");
            break;
        case MAIORQUE:
            printf("GT\n");
            break;
        case MENORouIGUAL:
            printf("LE\n");
            break;
        case MAIORouIGUAL:
            printf("GE\n");
            break;
        default:
            break;
        }
    }
}

// <atrib> ::= id [ '[' expr ']' ] = expr

static void parseAtrib(char *nomeVariavel)
{
    // char nomeVariavel[256];
    // strcpy(nomeVariavel, t.lexeme);
    // advanceToken();

    if (t.type == ABRECOLCHETE)
    {
        printf("// acesso a vetor (indice) - opcional\n");
        advanceToken();
        parseExpr();
        match(FECHACOLCHETE);
    }

    match(IGUAL);
    parseExpr();
    printf("STORE %s\n", nomeVariavel);
}

static void parseCmd()
{

    if (t.type == IF)
    {
        // if '(' expr ')' cmd [ else cmd ]
        advanceToken();
        match(ABREPARENTESE);

        int elseLabel = generateLabel();
        int endLabel = generateLabel();
        // esquema traducao
        parseExpr();
        printf("GOFALSE L%d\n", endLabel);

        match(FECHAPARENTESE);
        parseCmd();

        printf("GOTO L%d\n", endLabel);
        printf("LABEL L%d\n", elseLabel);

        if (t.type == ELSE)
        {
            advanceToken();
            parseCmd();
        }

        printf("LABEL L%d\n", endLabel);
    }
    else if (t.type == WHILE)
    {
        // while '(' expr ')' cmd
        advanceToken();
        match(ABREPARENTESE);

        int startLabel = generateLabel();
        int endLabel = generateLabel();

        printf("LABEL L%d\n", startLabel);

        parseExpr();

        printf("GOFALSE L%d\n", endLabel);

        match(FECHAPARENTESE);
        parseCmd();

        printf("GOTO L%d\n", startLabel);
        printf("LABEL L%d\n", endLabel);
    }
    else if (t.type == FOR)
    {
        // for '(' [ atrib ] ';' [ expr ] ';' [ atrib ] ')' cmd
        char nomeFor[256];
        strcpy(nomeFor, t.lexeme);

        advanceToken();
        match(ABREPARENTESE);

        int startLabel = generateLabel();
        int endLabel = generateLabel();
        int incrementLabel = generateLabel();

        if (t.type == ID)
        {
            char nomeInc[256];
            strcpy(nomeInc, t.lexeme);
            advanceToken();
            parseAtrib(nomeInc); // Atribuição opcional
        }
        match(PONTOVIRGULA);
        printf("LABEL L%d\n", startLabel); // Marca o início da condição

        if (t.type != PONTOVIRGULA)
        {
            parseExpr();                       // Expressão opcional
            printf("GOFALSE L%d\n", endLabel); // Salta para o final se a condição for falsa
        }
        match(PONTOVIRGULA);

        printf("LABEL L%d\n", incrementLabel); // Marca o início do incremento
        if (t.type == ID)
        {
            parseAtrib(nomeFor); // Atribuição opcional
        }
        match(FECHAPARENTESE);
        parseCmd();
        printf("GOTO L%d\n", startLabel); // volta para a condicão
        printf("LABEL L%d\n", endLabel);  // final do for
    }
    else if (t.type == RETURN)
    {
        // return [ expr ] ';'
        advanceToken();

        if (t.type != PONTOVIRGULA)
        {
            parseExpr(); // Expressão opcional
            printf("RET\n");
        }
        // match(PONTOVIRGULA); TODO: testar
    }
    else if (t.type == ID) // existe uma ambiguidade, porque id pode ser também uma atribuição
    {
        // id '(' [ expr { ',' expr } ] ')' ';' | id '[' expr ']' '=' expr ';' | atrib ';'
        char nome[256];
        strcpy(nome, t.lexeme);
        advanceToken();

        if (t.type == ABREPARENTESE)
        {
            // Chamada de função
            advanceToken();
            printf("CALL %s\n", nome);

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
        else if (t.type == ABRECOLCHETE)
        {
            parseAtrib(nome);
            match(PONTOVIRGULA);
        }
        else if (t.type == IGUAL)
        {
            // Atribuição simples
            parseAtrib(nome);
            match(PONTOVIRGULA);
        }
        else
        {
            printf("Erro de sintaxe na linha %d: comando inválido, encontrado '%s'\n", t.line, t.lexeme);
            exit(1);
        }
    }
    else if (t.type == PONTOVIRGULA)
    {
        // Comando vazio
        advanceToken();
    }
    else if (t.type == FECHACHAVE || t.type == TOKEN_EOF)
    {
        // Final de bloco - não é um comando, mas quem chamou vai lidar com isso.
        return;
    }
    else if (t.type == ABRECHAVE)
    {
        // Bloco de comandos
        advanceToken();

        while (t.type != FECHACHAVE && t.type != TOKEN_EOF)
        {
            parseCmd();
        }

        match(FECHACHAVE);
    }
    else
    {
        printf("Erro de sintaxe na linha %d: comando inválido, encontrado '%s'\n", t.line, t.lexeme);
        exit(1);
    }
}

// <decl> ::= <tipo> ID ( ) | <tipo> ID ;
static DeclKind decl(const char *tipoAtual)
{
    char nome[256];
    strcpy(nome, t.lexeme);

    if (t.type != ID)
    {
        printf("Erro de sintaxe na linha %d: identificador esperado após tipo.\n", t.line);
        exit(1);
    }

    strcpy(nome, t.lexeme);
    advanceToken();

    if (t.type == ABREPARENTESE)
    {
        insertSymbol(nome, tipoAtual, SYMBOL_FUNC, SCOPO_GLOBAL, 0);

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

                insertSymbol(t.lexeme, tipoAtual, SYMBOL_PARAM, SCOPO_LOCAL, 1);
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
            printf("aquiiii Antes %s\n", nome);
            printf("Erro de sintaxe na linha %d: esperado constante inteira.\n", t.line);
            exit(0);
        }
        // insertSymbol(nome, tipoAtual, SYMBOL_VAR, SCOPO_GLOBAL, 0);
        // printf("aquiiii  Depois%s\n", nome);

        int tamanho = atoi(t.lexeme);
        advanceToken();
        if (t.type != FECHACOLCHETE)
        {
            printf("Erro de sintaxe na linha %d: esperado ']'.\n", t.line);
            exit(0);
        }
        advanceToken();
        insertSymbol(nome, tipoAtual, SYMBOL_VETOR, SCOPO_GLOBAL, tamanho);
    }
    else if (t.type == PONTOVIRGULA) // Caso seja uma variável
    {

        insertSymbol(nome, tipoAtual, SYMBOL_VAR, SCOPO_GLOBAL, 1);

        return DECL_VAR;
    }
    else if (t.type == VIRGULA) // Caso seja uma lista de variáveis
    {
        insertSymbol(tipoAtual, "int", SYMBOL_VAR, SCOPO_LOCAL, 1);
        do
        {
            advanceToken();
            if (t.type != ID)
            {
                printf("Erro de sintaxe na linha %d: identificador esperado após ','.\n", t.line);
                exit(1);
            }
            insertSymbol(t.lexeme, "int", SYMBOL_VAR, SCOPO_GLOBAL, 1);
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

static DeclKind parseFunc()
{
    // func = tipo id '(' tipos_param ')' '{' { tipo decl_var { ',' decl_var } ';' } { cmd } '}'

    // Processa o identificador da função
    if (t.type != ID)
    {
        printf("Erro de sintaxe na linha %d: identificador esperado após tipo.\n", t.line);
        exit(1);
    }

    char nomeFunc[256];
    char tipoAtual[256];
    strcpy(nomeFunc, t.lexeme);
    insertSymbol(nomeFunc, tipoAtual, SYMBOL_FUNC, SCOPO_GLOBAL, 0); // Insere a função na tabela de símbolos
    advanceToken();

    // Processa os parâmetros da função
    match(ABREPARENTESE);         // '('
    if (t.type != FECHAPARENTESE) // Verifica se há parâmetros
    {
        do
        {
            char tipoParam[32];
            if (t.type == INT_T)
                strcpy(tipoParam, "int");
            else if (t.type == FLOAT_T)
                strcpy(tipoParam, "float");
            else if (t.type == CHAR_T)
                strcpy(tipoParam, "char");
            else if (t.type == BOOL_T)
                strcpy(tipoParam, "bool");
            else
            {
                printf("Erro de sintaxe na linha %d: tipo de parâmetro inválido.\n", t.line);
                exit(1);
            }

            advanceToken();
            // parseTipoSemVoid(); // Processa o tipo do parâmetro

            if (t.type != ID)
            {
                printf("Erro de sintaxe na linha %d: identificador esperado após tipo do parâmetro.\n", t.line);
                exit(1);
            }
            insertSymbol(t.lexeme, tipoParam, SYMBOL_PARAM, SCOPO_LOCAL, 1);
            advanceToken();

            if (t.type == VIRGULA) // ',' indica mais parâmetros
            {
                advanceToken();
            }
            else if (t.type != FECHAPARENTESE) // Erro se não for ',' ou ')'
            {
                printf("Erro de sintaxe na linha %d: esperado ',' ou ')' após parâmetro.\n", t.line);
                exit(1);
            }
        } while (t.type != FECHAPARENTESE);
    }
    match(FECHAPARENTESE); // ')'

    // Processa o corpo da função

    match(ABRECHAVE); // '{'

    // Processa declarações de variáveis locais
    while (t.type == INT_T || t.type == FLOAT_T || t.type == CHAR_T || t.type == BOOL_T)
    {
        char tipoVar[32];
        if (t.type == INT_T)
            strcpy(tipoVar, "int");
        else if (t.type == FLOAT_T)
            strcpy(tipoVar, "float");
        else if (t.type == CHAR_T)
            strcpy(tipoVar, "char");
        else if (t.type == BOOL_T)
            strcpy(tipoVar, "bool");
        advanceToken();
        do
        {
            if (t.type != ID)
            {
                printf("Erro de sintaxe na linha %d: identificador esperado após tipo.\n", t.line);
                exit(1);
            }
            insertSymbol(t.lexeme, tipoVar, SYMBOL_VAR, SCOPO_LOCAL, 1);
            advanceToken();

            if (t.type == VIRGULA) // ',' indica mais variáveis
            {
                advanceToken();
            }
            else if (t.type == ABRECOLCHETE)
            {
                advanceToken();
                if (t.type != INT)
                {
                    printf("Erro de sintaxe na linha %d: esperado constante inteira.\n", t.line);
                    exit(0);
                }
                // TODO: inserir na tabela
                //  insertSymbol(nome, SYMBOL_VAR);
                advanceToken();
                if (t.type != FECHACOLCHETE)
                {
                    printf("Erro de sintaxe na linha %d: esperado ']'.\n", t.line);
                    exit(0);
                }
                advanceToken();
            }
            else if (t.type != PONTOVIRGULA) // Erro se não for ',' ou ';'
            {
                printf("Erro de sintaxe na linha %d: esperado ',' ou ';' após declaração de variável e foi encontrado %s.\n", t.line, t.lexeme);
                exit(1);
            }
        } while (t.type == VIRGULA);
        match(PONTOVIRGULA);
    }

    // Processa comandos
    while (t.type != FECHACHAVE && t.type != TOKEN_EOF)
    {
        // printf("\n\nCai pra processar comando por que encontrei %s\n\n", t.lexeme);
        parseCmd(); // Processa cada comando
    }

    match(FECHACHAVE); // '}'
}

// <prog> ::= {decl ';' | func}
void parseProgram()
{
    initParserTokens();

    while (t.type != TOKEN_EOF)
    {
        if (t.type == INT_T || t.type == FLOAT_T || t.type == CHAR_T || t.type == BOOL_T || t.type == VOID)
        {
            char tipoAtual[20];
            if (t.type == INT_T)
                strcpy(tipoAtual, "int");
            else if (t.type == FLOAT_T)
                strcpy(tipoAtual, "float");
            else if (t.type == CHAR_T)
                strcpy(tipoAtual, "char");
            else if (t.type == BOOL_T)
                strcpy(tipoAtual, "bool");
            else if (t.type == VOID)
                strcpy(tipoAtual, "void");

            // Verifica se é uma declaração ou uma função
            advanceToken();
            if (t.type == ID && tLookahead.type == ABREPARENTESE)
            {
                // É uma função
                parseFunc();
            }
            else if (t.type == ID)
            {
                // É uma declaração
                DeclKind tipoDecl = decl(tipoAtual);
                match(PONTOVIRGULA);
            }
        }
        else
        {
            printf("Erro de sintaxe na linha %d: identificador esperado após tipo.\n", t.line);
            exit(1);
        }
    }

    if (t.type != TOKEN_EOF)
    {
        printf("Erro de sintaxe: conteúdo inesperado após o fim do programa (linha %d).\n", t.line);
        exit(1);
    }

    printSymbolTable();
}
