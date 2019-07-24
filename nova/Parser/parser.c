#include "parser.h"

ASTNode* parseBlock(Token** tokens,bool global,bool as_stat,bool as_simple){
    ASTNode* block = createASTNode(BLOCK,NULL);
    while(1)
    {
        if((*tokens)->tokenType == VAR){
            match(tokens,VAR);
            addChild(block,parseVarStat(tokens));
            match(tokens,SEMICOLON);
        }
        else if((*tokens)->tokenType == IDENTIFIER){
            ASTNode* lvalue = parseExpression(tokens,0);
            if(lvalue->children[0]->type == DEREF || lvalue->children[0]->type == INDEX){
                addChild(block,parseAssign(tokens,lvalue));
                match(tokens,SEMICOLON);
            }
            else if(lvalue->children[0]->type == CALL){
                addChild(block,lvalue);
                match(tokens,SEMICOLON);
            }
            else
            {
                invalidSyntax(tokens);
            }
            
        }
        else if((*tokens)->tokenType == WHILE && !as_simple)
        {
            match(tokens,WHILE);
            addChild(block,parseWhileLoop(tokens));
        }
        else if((*tokens)->tokenType == FOR && !as_simple){

            match(tokens,FOR);
            addChild(block,parseFor(tokens));
        }
        else if((*tokens)->tokenType == IF && !as_simple){
            match(tokens,IF);
            addChild(block,parseIf(tokens));
        }
        else if((!global && (*tokens)->tokenType == CCURL) || (global && (*tokens)->tokenType == eof)){
            break;
        }
        else
        {
            invalidSyntax(tokens);
        }
        if(as_stat)break;        
    }
    
    return block;
}

ASTNode* parseVarStat(Token** tokens){
    ASTNode* varstat = createASTNode(VARSTAT,NULL);
    if((*tokens)->tokenType == IDENTIFIER && (*tokens)->next->tokenType == EQUAL){
        ASTNode* def = createASTNode(DEF,NULL);
        ASTNode* id = createASTNode(TOKEN,*tokens);
        match(tokens,IDENTIFIER);
        match(tokens,EQUAL);
        ASTNode* value = parseExpression(tokens,0);
        addChild(def,id);
        addChild(def,value);
        addChild(varstat,def);
    }
    else if((*tokens)->tokenType == IDENTIFIER){
        ASTNode* dec = createASTNode(DEC,*tokens);
        match(tokens,IDENTIFIER);
        addChild(varstat,dec);
    }
    else{printf("Expected an identifier at line %d\n",(*tokens)->lineNum);exit(0);}

    while((*tokens)->tokenType == COMMA)
    {
        match(tokens,COMMA);
        if((*tokens)->tokenType == IDENTIFIER && (*tokens)->next->tokenType == EQUAL){
            ASTNode* def = createASTNode(DEF,NULL);
            ASTNode* id = createASTNode(TOKEN,*tokens);
            match(tokens,IDENTIFIER);
            match(tokens,EQUAL);
            ASTNode* value = parseExpression(tokens,0);
            addChild(def,id);
            addChild(def,value);
            addChild(varstat,def);
        }
        else if((*tokens)->tokenType == IDENTIFIER){
            ASTNode* dec = createASTNode(DEC,*tokens);
            match(tokens,IDENTIFIER);
            addChild(varstat,dec);
        }
        else{printf("Expected an identifier at line %d\n",(*tokens)->lineNum);exit(0);}
    }

    return varstat;
}

int main(){
    FILE* file = fopen("prog.nova","r");
    fseek(file,0,SEEK_END);
    int flen = ftell(file);
    fseek(file,0,SEEK_SET);
    char* codeBuff = (char*)malloc(flen);
    fread(codeBuff,sizeof(char),flen,file);
    Token* head = tokenize(codeBuff);
    ASTNode* root = parseBlock(&head,true,false,false);
}