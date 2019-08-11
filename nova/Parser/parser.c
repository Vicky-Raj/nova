#include "parser.h"

ASTNode* parseBlock(Token** tokens,bool global,bool as_stat){
    ASTNode* block = createASTNode(BLOCK,NULL);
    while(1)
    {
        if((*tokens)->tokenType == VAR){
            match(tokens,VAR);
            addChild(block,parseVarStat(tokens));
            match(tokens,SEMICOLON);
        }
        else if((*tokens)->tokenType == IDENTIFIER){
            addChild(block,parseAssignorCall(tokens));
            match(tokens,SEMICOLON);
            
        }
        else if((*tokens)->tokenType == WHILE)
        {
            match(tokens,WHILE);
            addChild(block,parseWhileLoop(tokens));
        }
        else if((*tokens)->tokenType == FOR){

            match(tokens,FOR);
            addChild(block,parseFor(tokens));
        }
        else if((*tokens)->tokenType == IF){
            match(tokens,IF);
            addChild(block,parseIf(tokens));
        }
        else if((*tokens)->tokenType == FUNC && global){
            match(tokens,FUNC);
            addChild(block,parseFunc(tokens));
        }
        else if((*tokens)->tokenType == BREAK && !global){
            match(tokens,BREAK);
            addChild(block,createASTNode(BREAKSTAT,NULL));
            match(tokens,SEMICOLON);
        }
        else if((*tokens)->tokenType == CONTINUE && !global){
            match(tokens,CONTINUE);
            addChild(block,createASTNode(CONTINUESTAT,NULL));
            match(tokens,SEMICOLON);
        }
        else if((*tokens)->tokenType == RETURN && !global){
            match(tokens,RETURN);
            ASTNode* returnstat = createASTNode(RETURNSTAT,NULL);
            if((*tokens)->tokenType != SEMICOLON)addChild(returnstat,parseExpression(tokens,false));
            addChild(block,returnstat);
            match(tokens,SEMICOLON);
        }
        else if((!global && (*tokens)->tokenType == CCURL) || (*tokens)->tokenType == eof){
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
        ASTNode* value = parseExpression(tokens,true);
        addChild(def,id);
        addChild(def,value);
        addChild(varstat,def);
    }
    else if((*tokens)->tokenType == IDENTIFIER && ((*tokens)->next->tokenType != COMMA && (*tokens)->next->tokenType != SEMICOLON)){
        invalidSyntax(tokens);
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
            ASTNode* value = parseExpression(tokens,false);
            addChild(def,id);
            addChild(def,value);
            addChild(varstat,def);
        }
        else if((*tokens)->tokenType == IDENTIFIER && ((*tokens)->next->tokenType != COMMA && (*tokens)->next->tokenType != SEMICOLON)){
            invalidSyntax(tokens);
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

ASTNode* parseAssignorCall(Token** tokens){
    ASTNode* lvalue = parseExpression(tokens,true);
    if(lvalue->type == DEREF || lvalue->type == INDEX){ 
        return parseAssign(tokens,lvalue);
    }
    else if(lvalue->type == CALL || lvalue->type == MCALL){
        return lvalue;
    }
    else
    {
        invalidSyntax(tokens);
    }
}
