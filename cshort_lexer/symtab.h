// symtab.h
#ifndef SYMTAB_H
#define SYMTAB_H

// Tipos de símbolos que podem existir na tabela
typedef enum
{
    SYMBOL_VAR,   // Variável
    SYMBOL_PARAM, // Parâmetro de função
    SYMBOL_FUNC,  // Função
    SYMBOL_VETOR  // Vetor
} SymbolKind;
typedef enum
{
    SCOPO_GLOBAL, // Global
    SCOPO_LOCAL   // Local
} Scopo;

typedef enum
{
    ESTADO_VIVO,
    ESTADO_ZOMBIE
} SymbolState;

// Estrutura de um símbolo
typedef struct Symbol
{
    // char name[64];
    char type[32];
    char name[256];  // Nome do identificador
    SymbolKind kind; // Tipo: variável, função, parâmetro
    Scopo scopo;
    SymbolState state;
    int size;
    struct Symbol *next; // Encadeamento para próxima entrada

} Symbol;

// Funções principais
void insertSymbol(const char *name, const char *type, SymbolKind kind, Scopo, int size);
Symbol *lookupSymbol(const char *name, Scopo lookupScopo);
void exitScopo(); // Zumbifica locais
void printSymbolTable();

// Funções auxiliares para registrar
void registerGlobalVar(const char *type, const char *name, int isVector, int size);
void registerFunction(const char *type, const char *name);
void registerParam(const char *type, const char *name, SymbolKind kind);
void registerLocalVar(const char *type, const char *name, int isVector, int size);

#endif