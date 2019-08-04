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
        }
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
            }
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