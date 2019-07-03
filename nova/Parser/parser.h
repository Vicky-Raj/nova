#include "../Lexer/lexer.h"

typedef enum{
    BLOCK,
    EXPRESSION,
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
    INDEX,
    LIST,
    DICT,
    DICTPAIR,
    VARSTAT,
    DEC,
    DEF,
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
ASTNode* parseBlock(Token** tokens);
ASTNode* parseVarStat(Token** tokens);
ASTNode* parseDec(Token** tokens);
ASTNode** parseDef(Token** tokens);
ASTNode* parseExpression(Token** tokens);
ASTNode* parseIdentifier(Token** tokens);//symbolParser.c
ASTNode* parseVar(Token** tokens);//symbolParser.c
ASTNode* parseIndex(Token** tokens);//symbolParser.c
ASTNode* parseList(Token** tokens);//parseComposite.c
ASTNode* parseDict(Token** tokens);//parseComposite.c

//helpers --helper.c
void match(Token** tokens,TokenType type);
void addChild(ASTNode* node,ASTNode* child);
ASTNode* createASTNode(ASTNodeType type,Token* token);
