#include "parser.h"

int isAssignOp(TokenType type);

ASTNode* parseAssign(Token** tokens,ASTNode* lvalue){
    ASTNode* node;ASTNode* rvalue;
    if (isAssignOp((*tokens)->tokenType))
    {
        switch((*tokens)->tokenType){
            case EQUAL:node = createASTNode(ASSIGN,NULL);match(tokens,EQUAL);break;
            case PLUSEQ:node = createASTNode(PLUSASSIGN,NULL);match(tokens,PLUSEQ);break;
            case MINUSEQ:node = createASTNode(MINUSASSIGN,NULL);match(tokens,MINUSEQ);break;
            case STAREQ:node = createASTNode(MULASSIGN,NULL);match(tokens,STAREQ);break;
            case SLASHEQ:node = createASTNode(DIVASSIGN,NULL);match(tokens,SLASHEQ);break;
            case PERCENTAGEEQ:node = createASTNode(MODASSIGN,NULL);match(tokens,PERCENTAGEEQ);break;
            case CARETEQ:node = createASTNode(POWASSIGN,NULL);match(tokens,CARETEQ);break;
        }
        rvalue = parseExpression(tokens,0);
        addChild(node,lvalue);
        addChild(node,rvalue);
        return node;
    }
    invalidSyntax(tokens);
}

int isAssignOp(TokenType type){
    return(
        type == EQUAL||
        type == PLUSEQ||
        type == MINUSEQ||
        type == STAREQ||
        type == SLASHEQ||
        type == PERCENTAGEEQ||
        type == CARETEQ
    );
}