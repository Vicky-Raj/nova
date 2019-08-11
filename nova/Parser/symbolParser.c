#include "parser.h"

ASTNode* parseIdentifier(Token** tokens){
    if((*tokens)->next->tokenType == OSQUARE){
        return parseIndex(tokens,parseVar(tokens));
    }else if((*tokens)->next->tokenType == OPARA){
        ASTNode* lvalue = parseFuncCall(tokens);
        if((*tokens)->tokenType == DOT)return parseMethodCall(tokens,lvalue);
        else return lvalue;
    }
    else{
        return parseVar(tokens);
    }
    
}


ASTNode* parseVar(Token** tokens){
    ASTNode* node =  createASTNode(DEREF,*tokens);
    match(tokens,IDENTIFIER);
    return node;
}

ASTNode* parseIndex(Token** tokens,ASTNode* id){
    ASTNode* index = createASTNode(INDEX,NULL);
    ASTNode* lastIndex = NULL;
    addChild(index,id);
    while((*tokens)->tokenType == OSQUARE)
    {
        match(tokens,OSQUARE);
        if((*tokens)->tokenType == CSQUARE)invalidSyntax(tokens);
        addChild(index,parseExpression(tokens,false));
        lastIndex = index;
        index = createASTNode(INDEX,NULL);
        addChild(index,lastIndex);
        match(tokens,CSQUARE);
    }
    return lastIndex;
}

