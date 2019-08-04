#include "sematic.h"

HashMap* initHashMap(){
    HashMap* hashmap = (HashMap*)malloc(sizeof(HashMap));
    hashmap->slots = (SymbolNode**)calloc(SLOTS,sizeof(HashMap*));
    return hashmap;
}

Scope* initScope(ScopeName name,Scope* enclosing){
    Scope* scope = (Scope*)malloc(sizeof(Scope));
    scope->name = name;
    scope->symbolTable = initHashMap();
    scope->enclosing = enclosing;
}

void pushScope(Scope** scope){
    *scope = initScope(LOCAL,*scope);
}

void popScope(Scope** scope){
    *scope = (*scope)->enclosing;
}

int hashSymbol(char* symbol){
    int hashIndex = 0;
    for(int i=0; symbol[i] != '\n';i++)hashIndex += symbol[i];
    return hashIndex % SLOTS;
}
SymbolNode* createSymbolNode(char* symbol){
    SymbolNode* symbolnode = (SymbolNode*)malloc(sizeof(SymbolNode));
    char* temp = (char*)malloc(strlen(symbol));
    strcpy(temp,symbol);
    symbolnode->symbol = temp;
    symbolnode->next = NULL;
    return symbolnode;
}
void append(Scope* scope,char* symbol){
    int hashIndex = hashSymbol(symbol);
    if(scope->symbolTable->slots[hashIndex] == NULL)
    scope->symbolTable->slots[hashIndex] = createSymbolNode(symbol);
    else
    {
        
    }
    
    
}
void checkSymbol(Scope* scope,char* symbol){

}
