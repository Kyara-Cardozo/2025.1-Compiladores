// lexer.h
#ifndef LEXER_H
#define LEXER_H

#include "tokens.h"
#include <stdio.h>

// Inicializa o analisador léxico com um arquivo
void initLexer(FILE *sourceFile);

// Retorna o próximo token do arquivo
Token getNextToken(void);

#endif
