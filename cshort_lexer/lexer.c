// lexer.c
#include "lexer.h"
#include <ctype.h>
#include <string.h>

// fonte de entrada e controle de posição

static FILE *source;
static int currentChar;
static int currentLine = 1;

void initLexer(FILE *sourceFile)
{
    source = sourceFile;
    currentChar = fgetc(source);
    currentLine = 1;
}

// avança para o próximo caractere na entrada

static void advance()
{
    currentChar = fgetc(source);
    if (currentChar == '\n')
        currentLine++;
}

// cria e retorna um token com tipo, lexema e linha atual

static Token makeToken(TokenType type, const char *lexeme)
{
    Token token;
    token.type = type;
    strncpy(token.lexeme, lexeme, sizeof(token.lexeme));
    token.line = currentLine;
    return token;
}

// função principal do analisador léxico: reconhece e retorna o próximo token
Token getNextToken()
{
    // ignora espaços em branco
    // Estado q0: estado inicial (ignora espaços)
    while (isspace(currentChar))
        advance();

    // fim de arquivo
    if (currentChar == EOF)
        return makeToken(TOKEN_EOF, "EOF");

    // Estado q1 → q10: identificadores e palavras-chave
    // identificadores e palavras-chave
    if (isalpha(currentChar) || currentChar == '_')
    {
        char buffer[256] = {0};
        int i = 0;

        // constrói o lexema
        do
        {
            buffer[i++] = currentChar;
            advance();
        } while (isalnum(currentChar) || currentChar == '_');

        // Palavras-chave
        if (strcmp(buffer, "if") == 0)
            return makeToken(IF, buffer);
        if (strcmp(buffer, "else") == 0)
            return makeToken(ELSE, buffer);
        if (strcmp(buffer, "while") == 0)
            return makeToken(WHILE, buffer);
        if (strcmp(buffer, "for") == 0)
            return makeToken(FOR, buffer);
        if (strcmp(buffer, "return") == 0)
            return makeToken(RETURN, buffer);
        if (strcmp(buffer, "void") == 0)
            return makeToken(VOID, buffer);
        if (strcmp(buffer, "char") == 0)
            return makeToken(CHAR_T, buffer);
        if (strcmp(buffer, "int") == 0)
            return makeToken(INT_T, buffer);
        if (strcmp(buffer, "float") == 0)
            return makeToken(FLOAT_T, buffer);
        if (strcmp(buffer, "bool") == 0)
            return makeToken(BOOL_T, buffer);

        return makeToken(ID, buffer);
    }

    // constantes numéricas: int ou real
     // Estado q2 → q11 (inteiro), q12 (real)
    if (isdigit(currentChar))
    {
        char buffer[256] = {0};
        int i = 0;
        do
        {
            buffer[i++] = currentChar;
            advance();
        } while (isdigit(currentChar));

        if (currentChar == '.')
        {
            buffer[i++] = currentChar;
            advance();

            // real inválido (ex: 123.)
            if (!isdigit(currentChar))
            {
                return makeToken(INVALID, "real incompleto");
            }
            while (isdigit(currentChar))
            {
                buffer[i++] = currentChar;
                advance();
            }
            return makeToken(REAL, buffer);
        }

        return makeToken(INT, buffer);
    }

    // constantes de caractere (char)
    // Estado q3 → q13 (char simples), q35 (com escape), q36 (erro)
    if (currentChar == '\'')
    {
        char buffer[4] = {0}; // pode conter até: \n + '\0'
        advance();

        if (currentChar == '\\')
        { // começo de escape: '\'
            buffer[0] = '\\';
            advance();

            if (currentChar == 'n' || currentChar == '0' || currentChar == 't' || currentChar == '\\' || currentChar == '\'' || currentChar == '"')
            {
                buffer[1] = currentChar;
                advance();
            }
            else
            {
                return makeToken(INVALID, "escape inválido em char");
            }
        }
        // caractere normal (ex: 'a')
        else if (isprint(currentChar) && currentChar != '\'' && currentChar != '\\')
        {
            buffer[0] = currentChar;
            advance();
        }
        else
        {
            return makeToken(INVALID, "caractere inválido");
        }

        // fecha aspas simples
        if (currentChar == '\'')
        {
            advance();
            return makeToken(CHAR, buffer);
        }
        else
        {
            return makeToken(INVALID, "char mal formado");
        }
    }

    // constantes de string (ex: "abc")
    // Estado q4 → q5 (string) ou q6 (erro: não fechada)
    if (currentChar == '"')
    {
        char buffer[256] = {0};
        int i = 0;
        advance(); // avança após o primeiro "

        while (currentChar != '"' && currentChar != EOF)
        {
            if (currentChar == '\n')
            {
                return makeToken(INVALID, "string com quebra de linha");
            }

            if (currentChar == '\\')
            {
                buffer[i++] = '\\';
                advance();

                if (currentChar == 'n' || currentChar == 't' || currentChar == '\\' ||
                    currentChar == '"' || currentChar == '0')
                {
                    buffer[i++] = currentChar;
                }
                else
                {
                    return makeToken(INVALID, "escape inválido em string");
                }
            }
            else if (currentChar == '"')
            {
                return makeToken(INVALID, "aspas não escapada dentro de string");
            }
            else
            {
                buffer[i++] = currentChar;
            }

            advance();
        }

        if (currentChar == '"')
        {
            advance(); // fecha string
            return makeToken(STRING, buffer);
        }
        else
        {
            return makeToken(INVALID, "string mal formada (não fechada)");
        }
    }

    // Comentários
    // Estado q8 → q16 (comentário de bloco), q17 (erro), ou comentário de linha
    if (currentChar == '/')
    {
        advance();
        if (currentChar == '*')
        {
             // Estado q16: comentário de bloco
            // Comentário de bloco: /* ... */
            advance();
            while (currentChar != EOF)
            {
                if (currentChar == '*')
                {
                    advance();
                    if (currentChar == '/')
                    {
                        advance();
                        return makeToken(COMMENT, "Comentario em bloco");
                    }
                }
                else
                {
                    advance();
                }
            }
            // Comentário de bloco não fechado
            return makeToken(INVALID, "Comentário de bloco não fechado");
        }
        else if (currentChar == '/')
        {
            // Comentário de linha: // ...
            advance();
            while (currentChar != '\n' && currentChar != EOF)
            {
                advance();
            }
            return makeToken(COMMENT, "Comentario de linha");
        }
        else
        {
            return makeToken(DIV, "/");
        }
    }

    // Operadores e delimitadores
    char ch = currentChar;
    advance();

    switch (ch)
    {
    case '+':
        return makeToken(MAIS, "+");
    case '-':
        return makeToken(MENOS, "-");
    case '*':
        return makeToken(MUL, "*");
    // Estado q39: operadores compostos  
    case '=':
        return (currentChar == '=' ? (advance(), makeToken(IGUALDADE, "==")) : makeToken(IGUAL, "="));
    case '!':
        return (currentChar == '=' ? (advance(), makeToken(NEGACAO, "!=")) : makeToken(NOT, "!"));
    case '<':
        return (currentChar == '=' ? (advance(), makeToken(MENORouIGUAL, "<=")) : makeToken(MENORQUE, "<"));
    case '>':
        return (currentChar == '=' ? (advance(), makeToken(MAIORouIGUAL, ">=")) : makeToken(MAIORQUE, ">"));

    case '&':
        if (currentChar == '&')
        {
            advance();
            return makeToken(AND, "&&");
        }
        break;
    case '|':
        if (currentChar == '|')
        {
            advance();
            return makeToken(OR, "||");
        }
        break;
    // Estado q7: delimitadores
    case ';':
        return makeToken(PONTOVIRGULA, ";");
    case ',':
        return makeToken(VIRGULA, ",");
    case '(':
        return makeToken(ABREPARENTESE, "(");
    case ')':
        return makeToken(FECHAPARENTESE, ")");
    case '[':
        return makeToken(ABRECOLCHETE, "[");
    case ']':
        return makeToken(FECHACOLCHETE, "]");
    case '{':
        return makeToken(ABRECHAVE, "{");
    case '}':
        return makeToken(FECHACHAVE, "}");
    }

    // qualquer outro caractere é inválido
    // Estado q17: erro léxico (qualquer caractere inválido)
    return makeToken(INVALID, "?");
}
