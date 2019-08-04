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
    ASSIGN,
    PLUSASSIGN,
    MINUSASSIGN,
    MULASSIGN,
    DIVASSIGN,
    MODASSIGN,
    POWASSIGN,
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
    CALL,
    MCALL,
    WHILELOOP,
    FORLOOP,
    FORITER,
    FORPRE,
    FORCON,
    FORPOST,
    ITERVAR,
    ITERARR,
    CONDSTAT,
    IFSTAT,
    ELIFSTAT,
    ELSESTAT,
    FUNCTION,
    PARAMLIST,
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
ASTNode* parseBlock(Token** tokens,bool global,bool as_stat);
ASTNode* parseVarStat(Token** tokens);
ASTNode* parseDec(Token** tokens);
ASTNode** parseDef(Token** tokens);
ASTNode* parseExpression(Token** tokens,bool strayCPARA);
ASTNode* parseIdentifier(Token** tokens);//symbolParser.c
ASTNode* parseVar(Token** tokens);//symbolParser.c
ASTNode* parseIndex(Token** tokens);//symbolParser.c
ASTNode* parseFuncCall(Token** tokens);//symbolParser.c
ASTNode* parseList(Token** tokens);//parseComposite.c
ASTNode* parseDict(Token** tokens);//parseComposite.c
ASTNode* parseAssign(Token** tokens,ASTNode* lvalue);
ASTNode* parseWhileLoop(Token** tokens);
ASTNode* parseIf(Token** tokens);
ASTNode* parseFor(Token** tokens);
ASTNode* parseForLoop(Token** tokens);
ASTNode* parseForIter(Token** tokens);
ASTNode* parseAssignorCall(Token** tokens);
ASTNode* parseFunc(Token** tokens);
ASTNode* parseMethodCall(Token** tokens,ASTNode* lvalue);

//helpers --helper.c
void match(Token** tokens,TokenType type);
void addChild(ASTNode* node,ASTNode* child);
ASTNode* createASTNode(ASTNodeType type,Token* token);

//errors
void invalidSyntax(Token** tokens);
void missingToken(TokenType type,Token** tokens);
void missingCond(Token** tokens);
