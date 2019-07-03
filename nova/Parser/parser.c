#include "parser.h"

ASTNode* parseBlock(Token** tokens){
    ASTNode* block = createASTNode(BLOCK,NULL);
    while(1)
    {
        if((*tokens)->tokenType == VAR){
            match(tokens,VAR);
            addChild(block,parseVarStat(tokens));
            match(tokens,SEMICOLON);
        }else
        {
            break;
        }
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
        ASTNode* value = parseExpression(tokens);
        addChild(def,id);
        addChild(def,value);
        addChild(varstat,def);
    }
    else if((*tokens)->tokenType == IDENTIFIER){
        ASTNode* dec = createASTNode(DEC,*tokens);
        match(tokens,IDENTIFIER);
        addChild(varstat,dec);
    }
    else{printf("Expected an identifier\n");exit(0);}

    while((*tokens)->tokenType == COMMA)
    {
        match(tokens,COMMA);
        if((*tokens)->tokenType == IDENTIFIER && (*tokens)->next->tokenType == EQUAL){
            ASTNode* def = createASTNode(DEF,NULL);
            ASTNode* id = createASTNode(TOKEN,*tokens);
            match(tokens,IDENTIFIER);
            match(tokens,EQUAL);
            ASTNode* value = parseExpression(tokens);
            addChild(def,id);
            addChild(def,value);
            addChild(varstat,def);
        }
        else if((*tokens)->tokenType == IDENTIFIER){
            ASTNode* dec = createASTNode(DEC,*tokens);
            match(tokens,IDENTIFIER);
            addChild(varstat,dec);
        }
        else{printf("Expected an identifier\n");exit(0);}
    }

    return varstat;
}

int main(){

    Token* head = tokenize("-6+7");
    ASTNode* root = parseExpression(&head);     
}