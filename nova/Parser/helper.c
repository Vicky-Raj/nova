#include "parser.h"

ASTNode* createASTNode(ASTNodeType type,Token* token){
    ASTNode* ast = (ASTNode*)malloc(sizeof(ASTNode));
    ast->type = type;
    ast->token =  token;
    ast->numOfChildren = 0;
    ast->maxChildren = 2;
    ast->children = (ASTNode**)malloc(sizeof(ASTNode*)*2);
    return ast;
}

void addChild(ASTNode* node,ASTNode* child){
    if(node->numOfChildren == node->maxChildren){
        node->children = (ASTNode**)realloc(node->children,sizeof(ASTNode*)*(node->maxChildren+2));
        node->maxChildren += 2;
    }
    node->children[node->numOfChildren++] = child;
}

void match(Token** tokens,TokenType type){
    if((*tokens)->tokenType == type)*tokens = (*tokens)->next;
    else missingToken(type,tokens);
}



