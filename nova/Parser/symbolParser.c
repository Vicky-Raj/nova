#include "parser.h"

ASTNode* parseIdentifier(Token** tokens){
    return parseVar(tokens);
}


ASTNode* parseVar(Token** tokens){
    return createASTNode(DEREF,*tokens);
    match(tokens,IDENTIFIER);
}