// symtab.c
#include "symtab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Symbol* symbolTable = NULL;  // Início da tabela 

// Insere um novo símbolo, verificando duplicação
// void insertSymbol(const char* name, SymbolKind kind) {
//     Symbol* existing = lookupSymbol(name);
//     if (existing) {
//         printf("Erro semântico: identificador '%s' já foi declarado antes.\n", name);
//         exit(1);
//     }

//     Symbol* newSymbol = (Symbol*)malloc(sizeof(Symbol));
//     strncpy(newSymbol->name, name, sizeof(newSymbol->name));
//     newSymbol->kind = kind;
//     newSymbol->next = symbolTable;
//     symbolTable = newSymbol;
// }
void insertSymbol(const char* name, const char* type, SymbolKind kind,Scopo scopo, int size) {
    for (Symbol* curr = symbolTable; curr != NULL; curr = curr->next) {
        if (strcmp(curr->name, name) == 0 && curr->scopo == scopo && curr->state ==ESTADO_VIVO) {
            fprintf(stderr, "Erro: símbolo '%s' já declarado neste escopo.\n", name);
            exit(1);
        }
    }

    Symbol* newSymbol = malloc(sizeof(Symbol));
    if (!newSymbol) {
        fprintf(stderr, "Erro de alocação de memória.\n");
        exit(1);
    }

    strncpy(newSymbol->name, name, sizeof(newSymbol->name));
    strncpy(newSymbol->type, type, sizeof(newSymbol->type));
    newSymbol->kind = kind;
    newSymbol->scopo = scopo;
    newSymbol->state =ESTADO_VIVO;
    newSymbol->size = size;
    newSymbol->next = symbolTable;
    symbolTable = newSymbol;
}

// Busca por nome e escopo 
Symbol* lookupSymbol(const char* name,Scopo lookupScopo) {
    for (Symbol* curr = symbolTable; curr != NULL; curr = curr->next) {
        if (strcmp(curr->name, name) == 0 && curr->state ==ESTADO_VIVO) {
            if (lookupScopo ==SCOPO_LOCAL || (lookupScopo ==SCOPO_GLOBAL && curr->scopo ==SCOPO_GLOBAL)) {
                return curr;
            }
        }
    }
    return NULL;
}

// // Busca um símbolo pelo nome
// Symbol* lookupSymbol(const char* name, Scopo lookupScopo) {
//     Symbol* current = symbolTable;
//     while (current) {
//         if (strcmp(current->name, name) == 0)
//             return current;
//         current = current->next;
//     }
//     return NULL;
// }

// Zumbifica todos os símbolos locais ativos 
void exitScopo() {
    for (Symbol* curr = symbolTable; curr != NULL; curr = curr->next) {
        if (curr->scopo ==SCOPO_LOCAL && curr->state ==ESTADO_VIVO) {
            curr->state = ESTADO_ZOMBIE;
        }
        if (curr->scopo ==SCOPO_GLOBAL) break; 
    }
}

// Imprime a tabela de símbolos
void printSymbolTable() {
    Symbol* curr = symbolTable;
    printf("======= TABELA DE SÍMBOLOS =======\n");
    while (curr) {
        const char* kindStr = (curr->kind == SYMBOL_VAR) ? "var" :
                              (curr->kind == SYMBOL_VETOR) ? "vetor" :
                              (curr->kind == SYMBOL_FUNC) ? "funcao" :
                              "param";
        const char*ScopoStr = (curr->scopo ==SCOPO_GLOBAL) ? "global" : "local";
        const char* stateStr = (curr->state ==ESTADO_VIVO) ? "ATIVO" : "ZUMBI";

       printf("Nome: %-10s | Tipo: %-6s | Classe: %-7s | Escopo: %-6s | Tamanho: %-3d | Estado: %s\n",
    curr->name, curr->type, kindStr, ScopoStr, curr->size, stateStr);
        curr = curr->next;
    }
    printf("==================================\n");
}

Tipo getTipo(const char *nome) {
    Symbol *sym = lookupSymbol(nome, SCOPO_LOCAL);
    if (!sym) sym = lookupSymbol(nome, SCOPO_GLOBAL);
    if (!sym) {
        printf("Erro semântico: identificador '%s' não declarado.\n", nome);
        exit(1);
    }
    // Converte string para enum Tipo
    if (strcmp(sym->type, "int") == 0) return TIPO_INT;
    if (strcmp(sym->type, "float") == 0) return TIPO_FLOAT;
    if (strcmp(sym->type, "char") == 0) return TIPO_CHAR;
    if (strcmp(sym->type, "bool") == 0) return TIPO_BOOL;
    if (strcmp(sym->type, "void") == 0) return TIPO_VOID;
    return TIPO_INDEFINIDO;
}