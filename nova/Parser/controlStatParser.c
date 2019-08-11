#include "parser.h"

bool isForIter(Token** tokens);
ASTNode* parseForStats(Token** tokens);

ASTNode* parseWhileLoop(Token** tokens){
    ASTNode* whileloop = createASTNode(WHILELOOP,NULL);ASTNode* block;ASTNode* condition;
    match(tokens,OPARA);
    if((*tokens)->tokenType == CPARA)missingCond(tokens);
    condition = parseExpression(tokens,true);
    if((*tokens)->tokenType != CPARA)invalidSyntax(tokens);
    match(tokens,CPARA);
    if((*tokens)->tokenType == OCURL){
        match(tokens,OCURL);
        block = parseBlock(tokens,false,false);
        match(tokens,CCURL);
    }else block = parseBlock(tokens,false,true);
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
    ASTNode* forpre = createASTNode(FORPRE,NULL);
    ASTNode* forcon = createASTNode(FORCON,NULL);
    ASTNode* forpost = createASTNode(FORPOST,NULL);
    ASTNode* forloop = createASTNode(FORLOOP,NULL);
    ASTNode* block;

    if((*tokens)->tokenType == CPARA)missingCond(tokens);


    if((*tokens)->tokenType == SEMICOLON)match(tokens,SEMICOLON);
    else{
        addChild(forpre,parseForStats(tokens));
        while((*tokens)->tokenType == COMMA)
        {
            match(tokens,COMMA);
            addChild(forpre,parseForStats(tokens));
        }
        match(tokens,SEMICOLON);
    }
    addChild(forloop,forpre);
    if((*tokens)->tokenType == SEMICOLON)match(tokens,SEMICOLON);
    else {
    addChild(forcon,parseExpression(tokens,true));
    match(tokens,SEMICOLON);
    }
    addChild(forloop,forcon);
    if((*tokens)->tokenType == CPARA)match(tokens,CPARA);
    else
    {
        addChild(forpost,parseForStats(tokens));
        while ((*tokens)->tokenType == COMMA)
        {
            match(tokens,COMMA);
            addChild(forpost,parseForStats(tokens));
        }
        match(tokens,CPARA);
    }
    addChild(forloop,forpost);
    if((*tokens)->tokenType == OCURL){
        match(tokens,OCURL);
        block = parseBlock(tokens,false,false);
        match(tokens,CCURL);
    }else block = parseBlock(tokens,false,true);

    addChild(forloop,block);
    return forloop;
    
}

ASTNode* parseForIter(Token** tokens){
    ASTNode* itervars = createASTNode(ITERVAR,NULL);
    ASTNode* iterarrs = createASTNode(ITERARR,NULL);
    ASTNode* foriter = createASTNode(FORITER,NULL);
    ASTNode* token = createASTNode(TOKEN,*tokens);
    ASTNode* block;
    addChild(itervars,token);

    if((*tokens)->tokenType == CPARA)missingCond(tokens);

    match(tokens,IDENTIFIER);
    while ((*tokens)->tokenType == COMMA)
    {
        match(tokens,COMMA);
        if((*tokens)->tokenType != IDENTIFIER)invalidSyntax(tokens);
        token = createASTNode(TOKEN,*tokens);
        addChild(itervars,token);
        match(tokens,IDENTIFIER);
    }
    match(tokens,COLON);
    if((*tokens)->tokenType == CPARA)invalidSyntax(tokens);
    addChild(iterarrs,parseExpression(tokens,true));
    while ((*tokens)->tokenType == COMMA)
    {
        match(tokens,COMMA);
        if((*tokens)->tokenType == CPARA)invalidSyntax(tokens);
        addChild(iterarrs,parseExpression(tokens,true));
    }
    match(tokens,CPARA);
    if((*tokens)->tokenType == OCURL){
        match(tokens,OCURL);
        block = parseBlock(tokens,false,false);
        match(tokens,CCURL);
    }else block = parseBlock(tokens,false,true);
    addChild(foriter,itervars);
    addChild(foriter,iterarrs);
    addChild(foriter,block);
    return foriter;
}

bool isForIter(Token** tokens){
    match(tokens,IDENTIFIER);
    while((*tokens)->tokenType == COMMA)
    {
        match(tokens,COMMA);
        match(tokens,IDENTIFIER);
    }
    return (*tokens)->tokenType == COLON;
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
        block = parseBlock(tokens,false,false);
        match(tokens,CCURL);
    }else block = parseBlock(tokens,false,true);
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
            block = parseBlock(tokens,false,false);
            match(tokens,CCURL);
        }else block = parseBlock(tokens,false,true);
        addChild(efstat,condition);
        addChild(efstat,block);
        addChild(condstat,efstat);
    }
    if((*tokens)->tokenType != ELSE)return condstat;
    match(tokens,ELSE);
    ASTNode* elsestat = createASTNode(ELSESTAT,NULL);
    if((*tokens)->tokenType == OCURL){
        match(tokens,OCURL);
        block = parseBlock(tokens,false,false);
        match(tokens,CCURL);
    }else block = parseBlock(tokens,false,true);
    addChild(elsestat,block);
    addChild(condstat,elsestat);
    return condstat;
}

ASTNode* parseForStats(Token** tokens){
    if((*tokens)->tokenType == VAR){match(tokens,VAR);return parseVarStat(tokens);}
    else return parseAssignorCall(tokens);
}