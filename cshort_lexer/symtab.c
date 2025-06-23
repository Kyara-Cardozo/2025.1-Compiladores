// symtab.c
#include "symtab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Symbol* symbolTable = NULL;  // Início da tabela (lista ligada)

// Insere um novo símbolo, verificando duplicação
void insertSymbol(const char* name, SymbolKind kind) {
    Symbol* existing = lookupSymbol(name);
    if (existing) {
        printf("Erro semântico: identificador '%s' já foi declarado antes.\n", name);
        exit(1);
    }

    Symbol* newSymbol = (Symbol*)malloc(sizeof(Symbol));
    strncpy(newSymbol->name, name, sizeof(newSymbol->name));
    newSymbol->kind = kind;
    newSymbol->next = symbolTable;
    symbolTable = newSymbol;
}

// Busca um símbolo pelo nome
Symbol* lookupSymbol(const char* name) {
    Symbol* current = symbolTable;
    while (current) {
        if (strcmp(current->name, name) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

// Imprime a tabela de símbolos
void printSymbolTable() {
    Symbol* current = symbolTable;
    printf("\n--- Tabela de Símbolos ---\n");
    while (current) {
        const char* kindStr = (current->kind == SYMBOL_VAR) ? "VAR" :
                              (current->kind == SYMBOL_PARAM) ? "PARAM" :
                              (current->kind == SYMBOL_FUNC) ? "FUNC" : "UNKNOWN";
        printf("%s - %s\n", current->name, kindStr);
        current = current->next;
    }
}
