#include "semantic.h"

bool inSymbolTable(SymbolNode* head,char* symbol);


void notDeclared(char* symbol,int lineNum){
    printf("undeclared identifier '%s' at line %d\n",symbol,lineNum);
    exit(0);
}

void redefinition(char* symbol,int lineNum){
    printf("redefinition of identifier '%s' at line %d\n",symbol,lineNum);
    exit(0);
}

HashMap* initHashMap(){
    HashMap* hashmap = (HashMap*)malloc(sizeof(HashMap));
    hashmap->slots = (SymbolNode**)calloc(SLOTS,sizeof(HashMap*));
    return hashmap;
}

Scope* initScope(Scope* enclosing){
    Scope* scope = (Scope*)malloc(sizeof(Scope));
    scope->symbolTable = initHashMap();
    scope->enclosing = enclosing;
}

void pushScope(Scope** scope){
    *scope = initScope(*scope);
}

void popScope(Scope** scope){
    *scope = (*scope)->enclosing;
}

int hashSymbol(char* symbol){
    int hashIndex = 0;
    for(int i=0; symbol[i] != '\0';i++)hashIndex += symbol[i];
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
void appendSymbol(Scope* scope,char* symbol,int lineNum){
    int hashIndex = hashSymbol(symbol);
    if(scope->symbolTable->slots[hashIndex] == NULL)
    scope->symbolTable->slots[hashIndex] = createSymbolNode(symbol);
    else
    {
        SymbolNode* temp = scope->symbolTable->slots[hashIndex];
        if(!strcmp(symbol,temp->symbol))redefinition(symbol,lineNum);
        while(temp->next != NULL){
            temp = temp->next;
            if(!strcmp(symbol,temp->symbol))redefinition(symbol,lineNum);
        }
        temp->next = createSymbolNode(symbol);
    }
}
void checkSymbol(Scope* scope,char* symbol,int lineNum){
    int hashIndex = hashSymbol(symbol);
    while (scope != NULL)
    {
        if(scope->symbolTable->slots[hashIndex] != NULL){
            if(!strcmp(symbol,scope->symbolTable->slots[hashIndex]->symbol))return;
            if(inSymbolTable(scope->symbolTable->slots[hashIndex]->next,symbol))return;
        }

        scope = scope->enclosing;
    }
    notDeclared(symbol,lineNum);
}

bool inSymbolTable(SymbolNode* head,char* symbol){
    while(head != NULL){
        if(!strcmp(symbol,head->symbol))return true;
        head = head->next;
    }
    return false;
}