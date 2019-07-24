#include "parser.h"

int isForIter(Token** tokens);

ASTNode* parseWhileLoop(Token** tokens){
    ASTNode* whileloop = createASTNode(WHILELOOP,NULL);ASTNode* block;ASTNode* condition;
    match(tokens,OPARA);
    if((*tokens)->tokenType == CPARA)missingCond(tokens);
    condition = parseExpression(tokens,true);
    if((*tokens)->tokenType != CPARA)invalidSyntax(tokens);
    match(tokens,CPARA);
    if((*tokens)->tokenType == OCURL){
        match(tokens,OCURL);
        block = parseBlock(tokens,false,false,false);
        match(tokens,CCURL);
    }else block = parseBlock(tokens,false,true,false);
    addChild(whileloop,condition);
    addChild(whileloop,block);
    return whileloop;
}

ASTNode* parseFor(Token** tokens){
    match(tokens,OPARA);
    Token* backPoint = *tokens;
    if((*tokens)->tokenType == IDENTIFIER && isForIter(tokens)){
        *tokens = backPoint;
        return parseForIter(tokens);
    }else
    {
        *tokens = backPoint;
        return parseForLoop(tokens);
    }
}

ASTNode* parseForLoop(Token** tokens){

}

ASTNode* parseForIter(Token** tokens){
    
}

int isForIter(Token** tokens){
    match(tokens,IDENTIFIER);
    while((*tokens)->tokenType != COMMA)
    {
        match(tokens,IDENTIFIER);
    }
    return ((*tokens)->tokenType == OF);
}


ASTNode* parseIf(Token** tokens){
    ASTNode* condstat = createASTNode(CONDSTAT,NULL);
    ASTNode* ifstat = createASTNode(IFSTAT,NULL);
    ASTNode* condition;ASTNode* block;
    match(tokens,OPARA);
    if((*tokens)->tokenType == CPARA)missingCond(tokens);
    condition = parseExpression(tokens,true);
    if((*tokens)->tokenType != CPARA)invalidSyntax(tokens);
    match(tokens,CPARA);
    if((*tokens)->tokenType == OCURL){
        match(tokens,OCURL);
        block = parseBlock(tokens,false,false,false);
        match(tokens,CCURL);
    }else block = parseBlock(tokens,false,true,false);
    addChild(ifstat,condition);
    addChild(ifstat,block);
    addChild(condstat,ifstat);
    if((*tokens)->tokenType != ELIF && (*tokens)->tokenType != ELSE)return condstat;
    while((*tokens)->tokenType == ELIF)
    {
        match(tokens,ELIF);
        ASTNode* efstat = createASTNode(ELIFSTAT,NULL);
        match(tokens,OPARA);
        if((*tokens)->tokenType == CPARA)missingCond(tokens);
        condition = parseExpression(tokens,true);
        if((*tokens)->tokenType != CPARA)invalidSyntax(tokens);
        match(tokens,CPARA);
        if((*tokens)->tokenType == OCURL){
            match(tokens,OCURL);
            block = parseBlock(tokens,false,false,false);
            match(tokens,CCURL);
        }else block = parseBlock(tokens,false,true,false);
        addChild(efstat,condition);
        addChild(efstat,block);
        addChild(condstat,efstat);
    }
    if((*tokens)->tokenType != ELSE)return condstat;
    match(tokens,ELSE);
    ASTNode* elsestat = createASTNode(ELSESTAT,NULL);
    if((*tokens)->tokenType == OCURL){
        match(tokens,OCURL);
        block = parseBlock(tokens,false,false,false);
        match(tokens,CCURL);
    }else block = parseBlock(tokens,false,true,false);
    addChild(elsestat,block);
    addChild(condstat,elsestat);
    return condstat;
}