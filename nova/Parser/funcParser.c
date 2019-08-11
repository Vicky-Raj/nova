#include "parser.h"

ASTNode* parseFunc(Token** tokens){
    ASTNode* func = createASTNode(FUNCTION,NULL);
    ASTNode* paramlist = createASTNode(PARAMLIST,NULL);
    addChild(func,createASTNode(TOKEN,*tokens));
    match(tokens,IDENTIFIER);
    match(tokens,OPARA);

    if((*tokens)->tokenType != CPARA){
        ASTNode* id = createASTNode(TOKEN,*tokens);
        match(tokens,IDENTIFIER);
        if((*tokens)->tokenType == EQUAL){
            match(tokens,EQUAL);
            if((*tokens)->tokenType == CPARA)invalidSyntax(tokens);
            ASTNode* assign = createASTNode(ASSIGN,NULL);
            addChild(assign,id);
            addChild(assign,parseExpression(tokens,true));
            addChild(paramlist,assign);
        }else addChild(paramlist,id);
        while((*tokens)->tokenType == COMMA){
            match(tokens,COMMA);
            ASTNode* id = createASTNode(TOKEN,*tokens);
            match(tokens,IDENTIFIER);
            if((*tokens)->tokenType == EQUAL){
                match(tokens,EQUAL);
                if((*tokens)->tokenType == CPARA)invalidSyntax(tokens);
                ASTNode* assign = createASTNode(ASSIGN,NULL);
                addChild(assign,id);
                addChild(assign,parseExpression(tokens,true));
                addChild(paramlist,assign);
            }else addChild(paramlist,id);
        }
    }
    match(tokens,CPARA);
    match(tokens,OCURL);
    addChild(func,paramlist);
    addChild(func,parseBlock(tokens,false,false));
    match(tokens,CCURL);
    
    return func;
    
}

ASTNode* parseMethodCall(Token** tokens,ASTNode* lvalue){
    match(tokens,DOT);
    ASTNode* mname = parseFuncCall(tokens);
    ASTNode* mcall = createASTNode(MCALL,NULL);
    addChild(mcall,lvalue);
    addChild(mcall,mname);
    while((*tokens)->tokenType == DOT) 
    {   
        match(tokens,DOT);
        mname = parseFuncCall(tokens);
        ASTNode* temp = mcall;
        mcall = createASTNode(MCALL,NULL);
        addChild(mcall,temp);
        addChild(mcall,mname);
    }
    return mcall;
}


ASTNode* parseFuncCall(Token** tokens){
    ASTNode* callframe = createASTNode(CALL,NULL);
    addChild(callframe,parseVar(tokens));
    ASTNode* paramlist = createASTNode(PARAMLIST,NULL);
    match(tokens,OPARA);
    if((*tokens)->tokenType == CPARA){
        match(tokens,CPARA);
        addChild(callframe,paramlist);
        if((*tokens)->tokenType == OSQUARE)return parseIndex(tokens,callframe);
        return callframe;
    }
    addChild(paramlist,parseExpression(tokens,true));
    while ((*tokens)->tokenType == COMMA){
        match(tokens,COMMA);
        addChild(paramlist,parseExpression(tokens,true));
    }
    if((*tokens)->tokenType != CPARA)invalidSyntax(tokens);
    match(tokens,CPARA);
    addChild(callframe,paramlist);
    if((*tokens)->tokenType == OSQUARE)return parseIndex(tokens,callframe);
    return callframe;
}