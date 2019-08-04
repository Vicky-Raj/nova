#include "../Parser/parser.h"

#define SLOTS 47

typedef enum{GLOBAL,LOCAL}ScopeName;

typedef struct SymbolNode{
    char* symbol;
    struct Value* next;
}SymbolNode;

typedef struct HashMap{
    SymbolNode** slots;
}HashMap;

typedef struct Scope{
    struct Scope* enclosing;
    ScopeName name;
    HashMap* symbolTable;
}Scope;


HashMap* initHashMap();
Scope* initScope(ScopeName name,Scope* enclosing);
void pushScope(Scope** scope);
void popScope(Scope** scope);
int hashSymbol(char* symbol);
SymbolNode* createSymbolNode(char* symbol);
void append(Scope* scope,char* symbol);
void checkSymbol(Scope* scope,char* symbol);
void redefinition(char* symbol);
void notDeclared(char* symbol);