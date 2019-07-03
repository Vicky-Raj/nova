#include "parser.h"

ASTNode* parseIdentifier(Token** tokens){
    if((*tokens)->next->tokenType == OSQUARE){
        return parseIndex(tokens);
    }else
    {
        return parseVar(tokens);
    }
    
}


ASTNode* parseVar(Token** tokens){
    ASTNode* node =  createASTNode(DEREF,*tokens);
    match(tokens,IDENTIFIER);
    return node;
}

ASTNode* parseIndex(Token** tokens){
    ASTNode* index = createASTNode(INDEX,NULL);
    ASTNode* lastIndex = NULL;
    addChild(index,parseVar(tokens));
    while((*tokens)->tokenType == OSQUARE)
    {
        match(tokens,OSQUARE);
        addChild(index,parseExpression(tokens));
        lastIndex = index;
        index = createASTNode(INDEX,NULL);
        addChild(index,lastIndex);
        match(tokens,CSQUARE);
    }
    return lastIndex;
}