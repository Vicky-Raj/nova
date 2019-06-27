#include "parser.h"

typedef struct TokenStackNode{
    Token* token;
    struct TokenStackNode* next;
}TokenStackNode;

typedef struct TreeStackNode{
    ASTNode* node;
    struct TreeStackNode* next;
}TreeStackNode;

typedef struct TokenStack{
    TokenStackNode* top;
}TokenStack;

typedef struct TreeStack{
    TreeStackNode* top;
}TreeStack;

TreeStack* initTreeStack();
TokenStack* initTokenStack();
void pushToken(TokenStack* stack,Token* token);
void pushNode(TreeStack* stack, ASTNode* node);
Token* popToken(TokenStack* stack);
Token* createZeroToken();
ASTNode* popNode(TreeStack* stack);
int emptyTreeStack(TreeStack* stack);
int emptyTokenStack(TokenStack* stack);
int isArithmeticToken(TokenType type);
int isRealationalToken(TokenType type);
int isLogicalToken(TokenType type);
int isHigher(TokenType stack,TokenType buff);
int opToNum(TokenType type);
ASTNode* createOpNode(TokenType type,ASTNode* left,ASTNode* right);

ASTNode* parseExpression(Token** tokens,TokenType end){
    TokenStack* arithmeticStack = initTokenStack();
    TokenStack* relationalStack = initTokenStack();
    TokenStack* logicalStack = initTokenStack();
    TreeStack* resultStack = initTreeStack();
    Token* last = NULL;
    while((*tokens)->tokenType != end)
    {
        TokenType lookahead = (*tokens)->tokenType;

        if(isRealationalToken(lookahead)){
            if(!emptyTokenStack(relationalStack)){
                printf("Invalid Syntax\n");
            }else
            {
                while(!emptyTokenStack(arithmeticStack) && arithmeticStack->top->token->tokenType != OPARA)
                {
                    ASTNode* right = popNode(resultStack);
                    ASTNode* left = popNode(resultStack);
                    pushNode(resultStack,createOpNode(popToken(arithmeticStack)->tokenType,left,right));
                }
                
                pushToken(relationalStack,*tokens);
            }
        }

        else if(isLogicalToken(lookahead)){
            while(!emptyTokenStack(arithmeticStack) && arithmeticStack->top->token->tokenType != OPARA){
                ASTNode* right = popNode(resultStack);
                ASTNode* left = popNode(resultStack);
                pushNode(resultStack,createOpNode(popToken(arithmeticStack)->tokenType,left,right));
            }
            if(!emptyTokenStack(relationalStack))
            {
                ASTNode* right = popNode(resultStack);
                ASTNode* left = popNode(resultStack);
                pushNode(resultStack,createOpNode(popToken(relationalStack)->tokenType,left,right));   
            }
            while(!emptyTokenStack(logicalStack) && 
            isHigher(logicalStack->top->token->tokenType,lookahead)&&
            logicalStack->top->token->tokenType != OPARA
            )
            {
                if(logicalStack->top->token->tokenType == NOT){
                    ASTNode* notOperand = popNode(resultStack);
                    ASTNode* notOperator = createASTNode(NEGATE,NULL);
                    addChild(notOperator,notOperand);
                    pushNode(resultStack,notOperator);
                    popToken(logicalStack);
                }else
                {
                    ASTNode* right = popNode(resultStack);
                    ASTNode* left = popNode(resultStack);
                    pushNode(resultStack,createOpNode(popToken(logicalStack)->tokenType,left,right));   
                }
                
            }
            pushToken(logicalStack,*tokens);                     
        }

        else if(isArithmeticToken(lookahead)){
            if((last == NULL && lookahead == MINUS) || last != NULL && lookahead == MINUS &&(isArithmeticToken(last->tokenType) || last->tokenType == OPARA)){
                pushNode(resultStack,createASTNode(TOKEN,createZeroToken()));
                pushToken(arithmeticStack,*tokens);
                last = *tokens;
                *tokens = (*tokens)->next;
                continue;
            }

            while(!emptyTokenStack(arithmeticStack) &&
            arithmeticStack->top->token->tokenType != OPARA &&
            isHigher(arithmeticStack->top->token->tokenType,lookahead)){
                ASTNode* right = popNode(resultStack);
                ASTNode* left = popNode(resultStack);
                pushNode(resultStack,createOpNode(popToken(arithmeticStack)->tokenType,left,right));
            }
            pushToken(arithmeticStack,*tokens);
        }

        else if(lookahead == OPARA){
        pushToken(arithmeticStack,*tokens);
        pushToken(logicalStack,*tokens);
        }

        else if(lookahead == CPARA){
            while(arithmeticStack->top->token->tokenType != OPARA){
                ASTNode* right = popNode(resultStack);
                ASTNode* left = popNode(resultStack);
                pushNode(resultStack,createOpNode(popToken(arithmeticStack)->tokenType,left,right));
            }
            if(!emptyTokenStack(relationalStack))
            {
                ASTNode* right = popNode(resultStack);
                ASTNode* left = popNode(resultStack);
                pushNode(resultStack,createOpNode(popToken(relationalStack)->tokenType,left,right));
            }
            while(logicalStack->top->token->tokenType != OPARA)
            {
                if(logicalStack->top->token->tokenType == NOT){
                    ASTNode* notOperand = popNode(resultStack);
                    ASTNode* notOperator = createASTNode(NEGATE,NULL);
                    addChild(notOperator,notOperand);
                    pushNode(resultStack,notOperator);
                    popToken(logicalStack);
                }else
                {
                    ASTNode* right = popNode(resultStack);
                    ASTNode* left = popNode(resultStack);
                    pushNode(resultStack,createOpNode(popToken(logicalStack)->tokenType,left,right));   
                }
            }
            popToken(logicalStack);
            popToken(arithmeticStack);
        }

        else if(lookahead == CONSTANT){
            pushNode(resultStack,createASTNode(TOKEN,*tokens));
        }
        else if(lookahead == IDENTIFIER){
            pushNode(resultStack,parseIdentifier(tokens));
        }

        last = *tokens;
        *tokens = (*tokens)->next;
    }
    while (!emptyTokenStack(arithmeticStack))
    {
        ASTNode* right = popNode(resultStack);
        ASTNode* left = popNode(resultStack);
        pushNode(resultStack,createOpNode(popToken(arithmeticStack)->tokenType,left,right));
    }
    if(!emptyTokenStack(relationalStack))
    {
        ASTNode* right = popNode(resultStack);
        ASTNode* left = popNode(resultStack);
        pushNode(resultStack,createOpNode(popToken(relationalStack)->tokenType,left,right));   
    }
    while(!emptyTokenStack(logicalStack))
    {
        if(logicalStack->top->token->tokenType == NOT){
            ASTNode* notOperand = popNode(resultStack);
            ASTNode* notOperator = createASTNode(NEGATE,NULL);
            addChild(notOperator,notOperand);
            pushNode(resultStack,notOperator);
            popToken(logicalStack);
        }else
        {
            ASTNode* right = popNode(resultStack);
            ASTNode* left = popNode(resultStack);
            pushNode(resultStack,createOpNode(popToken(logicalStack)->tokenType,left,right));   
        }
    }
    
    return popNode(resultStack);
}

void pushToken(TokenStack* stack,Token* token){
    TokenStackNode* temp = stack->top;
    stack->top = (TokenStackNode*)malloc(sizeof(TokenStackNode));
    stack->top->token = token;
    stack->top->next = temp;
}

void pushNode(TreeStack* stack, ASTNode* node){
    TreeStackNode* temp = stack->top; 
    stack->top = (TreeStackNode*)malloc(sizeof(TreeStackNode));
    stack->top->node = node;
    stack->top->next = temp;
}

Token* popToken(TokenStack* stack){
    TokenStackNode* temp = stack->top->next;
    Token* token = stack->top->token;
    free(stack->top);
    stack->top = temp;
    return token;
}

ASTNode* popNode(TreeStack* stack){
    TreeStackNode* temp = stack->top->next;
    ASTNode* node = stack->top->node;
    free(stack->top);
    stack->top = temp;
    return node;
}

int emptyTreeStack(TreeStack* stack){
    return(stack->top == NULL);
}

int emptyTokenStack(TokenStack* stack){
    return(stack->top == NULL);
}

TreeStack* initTreeStack(){
    TreeStack* ts = (TreeStack*)malloc(sizeof(TreeStack));
    ts->top = NULL;
    return ts;
}

TokenStack* initTokenStack(){
    TokenStack* tokenStack = (TokenStack*)malloc(sizeof(TokenStack));
    tokenStack->top = NULL;
    return tokenStack;
}

int isHigher(TokenType stack,TokenType buff){
    if(stack == NOT && buff == NOT)return 0;
    else if(opToNum(stack) == opToNum(buff))return 1;
    else return opToNum(stack) > opToNum(buff);
}

int opToNum(TokenType type){
    if(type == PLUS || type == MINUS || type == OR)return 1;
    if(type == STAR || type == SLASH || type == PERCENTAGE || type == AND)return 2;
    if(type == CARET || type == NOT)return 3;
}

ASTNode* createOpNode(TokenType type,ASTNode* left,ASTNode* right){
    ASTNode* node = NULL;
    switch (type)
    {
        case PLUS:node = createASTNode(ADD,NULL);break;
        case MINUS:node = createASTNode(SUB,NULL);break;
        case STAR:node = createASTNode(MUL,NULL);break;
        case SLASH:node = createASTNode(DIV,NULL);break;
        case PERCENTAGE:node = createASTNode(MODULO,NULL);break;
        case CARET:node = createASTNode(POW,NULL);break;
        case LESSER:node = createASTNode(LESSTHANOP,NULL);break;
        case GREATER:node = createASTNode(GREATERTHANOP,NULL);break;
        case LESSEREQ:node = createASTNode(LESSERTHANEQOP,NULL);break;
        case GREATEREQ:node = createASTNode(GREATERTHANEQOP,NULL);break;
        case EQUALEQ:node = createASTNode(EQUALEQOP,NULL);break;
        case NOTEQ:node = createASTNode(NOTEQOP,NULL);break;
        case AND:node = createASTNode(CONJUCT,NULL);break;
        case OR:node = createASTNode(DISJUNCT,NULL);break;
    }
    addChild(node,left);
    addChild(node,right);
    return node;
}

int isArithmeticToken(TokenType type){
    return(type == PLUS || type == MINUS || type == STAR || type == SLASH || type == CARET || type == PERCENTAGE);
}
int isRealationalToken(TokenType type){
    return(type == GREATER || type == LESSER || type == GREATEREQ || type == LESSEREQ || type == NOTEQ || type == EQUALEQ);
}
int isLogicalToken(TokenType type){
    return(type == AND || type == OR || type == NOT);
}

Token* createZeroToken(){
    Token* token = (Token*)malloc(sizeof(Token));
    token->tokenType = CONSTANT;
    token->dataType = INT;
    token->value = (int64*)malloc(sizeof(int64));
    *((int64*)(token->value)) = 0;
    token->next = NULL;
    return token;
}