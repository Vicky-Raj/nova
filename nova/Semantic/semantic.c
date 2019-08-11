#include "semantic.h"

void checkSemantics(ASTNode* block);
void checkBlock(Scope** scope,ASTNode* block);
void checkExpression(Scope* scope,ASTNode* block);
void checkVarStat(Scope* scope,ASTNode* block);
void checkAssign(Scope* scope,ASTNode* block);
void checkList(Scope* scope,ASTNode* block);
void checkDict(Scope* scope,ASTNode* block);
void checkWhileLoop(Scope** scope,ASTNode* block);
void checkCondStat(Scope** scope,ASTNode* block);
void checkForLoop(Scope** scope,ASTNode* block);
void checkForIter(Scope** scope,ASTNode* block);
void checkFunc(Scope** scope,ASTNode* block);
void checkReturnStat(Scope* scope,ASTNode* block);

int main(){
    FILE* file = fopen("prog.nova","r");
    fseek(file,0,SEEK_END);
    int flen = ftell(file);
    fseek(file,0,SEEK_SET);
    char* codeBuff = (char*)malloc(flen);
    fread(codeBuff,sizeof(char),flen,file);
    Token* head = tokenize(codeBuff);
    ASTNode* root = parseBlock(&head,true,false);
    checkSemantics(root);
}

void checkSemantics(ASTNode* block){
    Scope* globalScope = initScope(NULL);
    checkBlock(&globalScope,block);
}

void checkBlock(Scope** scope,ASTNode* block){
    for(int i=0;i<block->numOfChildren;i++){
        switch(block->children[i]->type){
            case VARSTAT:checkVarStat(*scope,block->children[i]);break;
            case FUNCTION:checkFunc(scope,block->children[i]);break;
            case ASSIGN:checkAssign(*scope,block->children[i]);break;
            case PLUSASSIGN:checkAssign(*scope,block->children[i]);break;
            case MINUSASSIGN:checkAssign(*scope,block->children[i]);break;
            case MULASSIGN:checkAssign(*scope,block->children[i]);break;
            case DIVASSIGN:checkAssign(*scope,block->children[i]);break;
            case MODASSIGN:checkAssign(*scope,block->children[i]);break;
            case POWASSIGN:checkAssign(*scope,block->children[i]);break;
            case WHILELOOP:checkWhileLoop(scope, block->children[i]);break;
            case FORLOOP:checkForLoop(scope,block->children[i]);break;
            case FORITER:checkForIter(scope,block->children[i]);break;
            case CONDSTAT:checkCondStat(scope,block->children[i]);break;
            case CALL:checkExpression(*scope,block->children[i]);break;
            case MCALL:checkExpression(*scope,block->children[i]);break;
            case RETURNSTAT:checkReturnStat(*scope,block->children[i]);break;
        }
    }
}

void checkExpression(Scope* scope,ASTNode* block){
    switch (block->type)
    {
        case INDEX:checkExpression(scope,block->children[0]);checkExpression(scope,block->children[1]);break;
        case CALL:checkExpression(scope,block->children[0]);checkList(scope,block->children[1]);break;
        case MCALL:checkExpression(scope,block->children[0]);checkExpression(scope,block->children[1]);break;
        case LIST:checkList(scope,block);break;
        case DICT:checkDict(scope,block);break;
        case DEREF:checkSymbol(scope,block->token->value,block->token->lineNum);break;
        case TOKEN:break;
        default:checkExpression(scope,block->children[0]);checkExpression(scope,block->children[1]);break;
    }
}

void checkVarStat(Scope* scope,ASTNode* block){
    for(int i=0;i<block->numOfChildren;i++){
        switch(block->children[i]->type){
            case DEC:
            appendSymbol(scope,block->children[i]->token->value,block->children[i]->token->lineNum);break;
            case DEF:
            appendSymbol(scope,block->children[i]->children[0]->token->value,block->children[i]->children[0]->token->lineNum);
            checkExpression(scope,block->children[i]->children[1]);break;
        }
    }
}

void checkAssign(Scope* scope,ASTNode* block){
    checkExpression(scope,block->children[1]);
    checkExpression(scope,block->children[0]);
}

void checkList(Scope* scope,ASTNode* block){
    for(int i=0;i<block->numOfChildren;i++){
        checkExpression(scope,block->children[i]);
    }
}

void checkDict(Scope* scope,ASTNode* block){
    for(int i=0;i<block->numOfChildren;i++){
        checkExpression(scope,block->children[i]->children[0]);
        checkExpression(scope,block->children[i]->children[1]);
    }
}

void checkWhileLoop(Scope** scope,ASTNode* block){
    checkExpression(*scope,block->children[0]);
    pushScope(scope);
    checkBlock(scope,block->children[1]);
    popScope(scope);
}

void checkCondStat(Scope** scope,ASTNode* block){
    for(int i=0;i<block->numOfChildren;i++){
        if(block->children[i]->type != ELSESTAT){
            checkExpression(*scope,block->children[i]->children[0]);
            pushScope(scope);
            checkBlock(scope,block->children[i]->children[1]);
            popScope(scope);
        }else
        {   pushScope(scope);
            checkBlock(scope,block->children[i]->children[0]);
            popScope(scope);
        }
    }
}

void checkForLoop(Scope** scope,ASTNode* block){
    pushScope(scope);
    checkBlock(scope,block->children[0]);

    if(block->children[1]->numOfChildren > 0)
    checkExpression(*scope,block->children[1]->children[0]);

    checkBlock(scope,block->children[2]);
    pushScope(scope);
    checkBlock(scope,block->children[3]);
    popScope(scope);
    popScope(scope);
}

void checkForIter(Scope** scope,ASTNode* block){
    pushScope(scope);
    for(int i=0;i<block->children[0]->numOfChildren;i++){
        appendSymbol(*scope,block->children[0]->children[i]->token->value,block->children[0]->children[i]->token->lineNum);
    }
    for(int i=0;i<block->children[1]->numOfChildren;i++){
        checkExpression(*scope,block->children[1]->children[i]);
    }
    pushScope(scope);
    checkBlock(scope,block->children[2]);
    popScope(scope);
    popScope(scope);
}

void checkFunc(Scope** scope,ASTNode* block){
    appendSymbol(*scope,block->children[0]->token->value,block->children[0]->token->lineNum);
    pushScope(scope);
    for(int i=0;i<block->children[1]->numOfChildren;i++){
        if(block->children[1]->children[i]->type == ASSIGN){
            appendSymbol(*scope,block->children[1]->children[i]->children[0]->token->value,block->children[1]->children[i]->children[0]->token->lineNum);
            Scope** mainscope = scope;
            popScope(scope);
            checkExpression(*scope,block->children[1]->children[i]->children[1]);
            scope = mainscope;
        }else{
            appendSymbol(*scope,block->children[1]->children[i]->token->value,block->children[1]->children[i]->token->lineNum);
        }
    }
    pushScope(scope);
    checkBlock(scope,block->children[2]);
    popScope(scope);
    popScope(scope);
}

void checkReturnStat(Scope* scope,ASTNode* block){
    if(block->numOfChildren > 0)checkExpression(scope,block->children[0]);
}