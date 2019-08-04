#include "parser.h"

ASTNode* parseIdentifier(Token** tokens){
    if((*tokens)->next->tokenType == OSQUARE){
        return parseIndex(tokens);
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

ASTNode* parseIndex(Token** tokens){
    ASTNode* index = createASTNode(INDEX,NULL);
    ASTNode* lastIndex = NULL;
    addChild(index,parseVar(tokens));
    while((*tokens)->tokenType == OSQUARE)
    {
        match(tokens,OSQUARE);
        addChild(index,parseExpression(tokens,0));
        lastIndex = index;
        index = createASTNode(INDEX,NULL);
        addChild(index,lastIndex);
        match(tokens,CSQUARE);
    }
    return lastIndex;
}

ASTNode* parseFuncCall(Token** tokens){
    ASTNode* callframe = createASTNode(CALL,NULL);
    addChild(callframe,parseVar(tokens));
    ASTNode* paramlist = createASTNode(PARAMLIST,NULL);
    match(tokens,OPARA);
    if((*tokens)->tokenType == CPARA){
        match(tokens,CPARA);
        addChild(callframe,paramlist);
        return callframe;
    }
    addChild(paramlist,parseExpression(tokens,1));
    while ((*tokens)->tokenType == COMMA){
        match(tokens,COMMA);
        addChild(paramlist,parseExpression(tokens,1));
    }
    if((*tokens)->tokenType != CPARA)invalidSyntax(tokens);
    match(tokens,CPARA);
    addChild(callframe,paramlist);
    return callframe;

}