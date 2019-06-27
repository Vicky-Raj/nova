#include "../Lexer/lexer.h"

typedef enum{
    ADD,
    SUB,
    MUL,
    DIV,
    MODULO,
    POW,
    LESSTHANOP,
    GREATERTHANOP,
    LESSERTHANEQOP,
    GREATERTHANEQOP,
    EQUALEQOP,
    NOTEQOP,
    CONJUCT,
    DISJUNCT,
    NEGATE,
    DEREF,
    TOKEN,
}ASTNodeType;

typedef struct ASTNode{
    ASTNodeType type;
    Token* token;
    int maxChildren;
    int numOfChildren;
    struct ASTNode** children;
}ASTNode;

//parsers
ASTNode* parseExpression(Token** tokens,TokenType end);
ASTNode* parseIdentifier(Token** tokens);//symbolParser.c
ASTNode* parseVar(Token** tokens);//expressParser.c

//helpers --helper.c
void match(Token** tokens,TokenType type);
void addChild(ASTNode* node,ASTNode* child);
ASTNode* createASTNode(ASTNodeType type,Token* token);
