// symtab.h
#ifndef SYMTAB_H
#define SYMTAB_H

// Tipos de símbolos que podem existir na tabela
typedef enum {
    SYMBOL_VAR,     // Variável
    SYMBOL_PARAM,   // Parâmetro de função (se você quiser implementar depois)
    SYMBOL_FUNC     // Função
} SymbolKind;

// Estrutura de um símbolo
typedef struct Symbol {
    char name[256];         // Nome do identificador
    SymbolKind kind;        // Tipo: variável, função, parâmetro
    struct Symbol* next;    // Encadeamento para próxima entrada (lista ligada)
} Symbol;

// Insere um novo símbolo na tabela
void insertSymbol(const char* name, SymbolKind kind);

// Busca um símbolo por nome (retorna NULL se não achar)
Symbol* lookupSymbol(const char* name);

// Imprime a tabela de símbolos (opcional, para debug ou exibição final)
void printSymbolTable();

#endif
