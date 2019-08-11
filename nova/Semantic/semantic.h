#include "../Parser/parser.h"

#define SLOTS 47


typedef struct SymbolNode{
    char* symbol;
    struct SymbolNode* next;
}SymbolNode;

typedef struct HashMap{
    SymbolNode** slots;
}HashMap;

typedef struct Scope{
    struct Scope* enclosing;
    HashMap* symbolTable;
}Scope;



//helpers.c
HashMap* initHashMap();
Scope* initScope(Scope* enclosing);
void pushScope(Scope** scope);
void popScope(Scope** scope);
int hashSymbol(char* symbol);
SymbolNode* createSymbolNode(char* symbol);
void appendSymbol(Scope* scope,char* symbol,int lineNum);
void checkSymbol(Scope* scope,char* symbol,int lineNum);
void redefinition(char* symbol,int lineNum);
void notDeclared(char* symbol,int lineNum);