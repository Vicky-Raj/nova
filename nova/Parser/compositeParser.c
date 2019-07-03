#include "parser.h"

ASTNode* parseDict(Token** tokens){
    match(tokens,OCURL);
    ASTNode* dict = createASTNode(DICT,NULL);
    if((*tokens)->tokenType == CCURL){
        match(tokens,CCURL);
        return dict;
    }
    ASTNode* pair = createASTNode(DICTPAIR,NULL);
    ASTNode* key = parseExpression(tokens);
    match(tokens,COLON);
    ASTNode* value = parseExpression(tokens);
    addChild(pair,key);
    addChild(pair,value);
    addChild(dict,pair);
    while((*tokens)->tokenType == COMMA)
    {
        match(tokens,COMMA);
        if((*tokens)->tokenType == CCURL)break;
        pair = createASTNode(DICTPAIR,NULL);
        key = parseExpression(tokens);
        match(tokens,COLON);
        value = parseExpression(tokens);
        addChild(pair,key);
        addChild(pair,value);
        addChild(dict,pair);
    }
    
    match(tokens,CCURL);
    return dict;
}

ASTNode* parseList(Token** tokens){
    match(tokens,OSQUARE);
    ASTNode* list = createASTNode(LIST,NULL);
    if((*tokens)->tokenType == CSQUARE){
        match(tokens,CSQUARE);
        return list;
    }
    addChild(list,parseExpression(tokens));
    while((*tokens)->tokenType == COMMA)
    {
        match(tokens,COMMA);
        if((*tokens)->tokenType == CSQUARE)break;
        addChild(list,parseExpression(tokens));

    }
    match(tokens,CSQUARE);
    return list;
}