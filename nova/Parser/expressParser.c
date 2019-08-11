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
Token* popToken(TokenStack* stack,Token** tokens);
void reduce(TokenStack* opstack,TreeStack* resultstack,Token** tokens);
Token* createNegToken();
ASTNode* popNode(TreeStack* stack,Token** tokens);
int emptyTreeStack(TreeStack* stack);
int emptyTokenStack(TokenStack* stack);
int isArithmeticToken(TokenType type);
int isRealationalToken(TokenType type);
int isLogicalToken(TokenType type);
int isHigher(TokenType stack,TokenType buff);
int opToNum(TokenType type);
ASTNode* createOpNode(TokenType type,ASTNode* left,ASTNode* right);

ASTNode* parseExpression(Token** tokens,bool strayCPARA){
    TokenStack* arithmeticStack = initTokenStack();
    TokenStack* relationalStack = initTokenStack();
    TokenStack* logicalStack = initTokenStack();
    TreeStack* resultStack = initTreeStack();
    Token* last = NULL;

    while(1)
    {
        TokenType lookahead = (*tokens)->tokenType;

        if(last != NULL && (last->tokenType == CONSTANT || last->tokenType == IDENTIFIER) && (lookahead == CONSTANT || lookahead == IDENTIFIER)){
            invalidSyntax(tokens);
        }
        
        // +,-,*,/.....
        if(isArithmeticToken(lookahead)){
            
            if((last == NULL && lookahead == MINUS) || last != NULL && lookahead == MINUS &&(isArithmeticToken(last->tokenType) || last->tokenType == OPARA)){
                pushToken(arithmeticStack,createNegToken());
                last = *tokens;
                *tokens = (*tokens)->next;
                continue;
            }
                
            while (!emptyTokenStack(arithmeticStack) && 
            arithmeticStack->top->token->tokenType != OPARA &&
            isHigher(arithmeticStack->top->token->tokenType,lookahead)){

            reduce(arithmeticStack,resultStack,tokens);
            
            }

            pushToken(arithmeticStack,*tokens);
        }

        else if(isRealationalToken(lookahead)){
            while (!emptyTokenStack(arithmeticStack) && arithmeticStack->top->token->tokenType != OPARA)
            reduce(arithmeticStack,resultStack,tokens);
            
            while (!emptyTokenStack(relationalStack) && relationalStack->top->token->tokenType != OPARA)
            reduce(relationalStack,resultStack,tokens);

            pushToken(relationalStack,*tokens);
            
        }

        else if(isLogicalToken(lookahead)){
            while (!emptyTokenStack(arithmeticStack) && arithmeticStack->top->token->tokenType != OPARA)
            reduce(arithmeticStack,resultStack,tokens);

            while (!emptyTokenStack(relationalStack) && relationalStack->top->token->tokenType != OPARA)
            reduce(relationalStack,resultStack,tokens);

            while(!emptyTokenStack(logicalStack) && logicalStack->top->token->tokenType != OPARA && isHigher(logicalStack->top->token->tokenType,lookahead)){
                reduce(logicalStack,resultStack,tokens);
            }

            pushToken(logicalStack,*tokens);
        }

        // (
        else if(lookahead == OPARA){
            pushToken(arithmeticStack,*tokens);
            pushToken(relationalStack,*tokens);
            pushToken(logicalStack,*tokens);
        }

        else if(lookahead == CPARA){



            while(!emptyTokenStack(arithmeticStack) && arithmeticStack->top->token->tokenType != OPARA)
            reduce(arithmeticStack,resultStack,tokens);
            if(emptyTokenStack(arithmeticStack) && !strayCPARA)invalidSyntax(tokens);
            else if(!emptyTokenStack(arithmeticStack) && arithmeticStack->top->token->tokenType == OPARA)popToken(arithmeticStack,tokens);


            while (!emptyTokenStack(relationalStack) && relationalStack->top->token->tokenType != OPARA)
            reduce(relationalStack,resultStack,tokens);
            if(emptyTokenStack(relationalStack) && !strayCPARA)invalidSyntax(tokens);
            else if(!emptyTokenStack(relationalStack) && relationalStack->top->token->tokenType == OPARA)popToken(relationalStack,tokens);



            while (!emptyTokenStack(logicalStack) && logicalStack->top->token->tokenType != OPARA)
            reduce(logicalStack,resultStack,tokens);
            if(emptyTokenStack(logicalStack) && !strayCPARA)invalidSyntax(tokens);
            else if(!emptyTokenStack(logicalStack) && logicalStack->top->token->tokenType == OPARA)popToken(logicalStack,tokens);

            if(strayCPARA && emptyTokenStack(arithmeticStack) && emptyTokenStack(relationalStack) && emptyTokenStack(logicalStack)){
                return popNode(resultStack,tokens);
            }
        }

        else if(lookahead == CONSTANT){
            pushNode(resultStack,createASTNode(TOKEN,*tokens));
        }
        else if(lookahead == IDENTIFIER){
            last = *tokens;
            ASTNode* lvalue = parseIdentifier(tokens);
            if((*tokens)->tokenType == DOT)pushNode(resultStack,parseMethodCall(tokens,lvalue));
            else pushNode(resultStack,lvalue);
            continue;
        }
        else if(lookahead == OSQUARE)
        {
            last = *tokens;
            pushNode(resultStack,parseList(tokens));
            continue;
        }
        else if(lookahead == OCURL){
            last = *tokens;
            pushNode(resultStack,parseDict(tokens));
            continue;
        }
        else
        {
            if(last == NULL){
                invalidSyntax(tokens);
            }
            break;
        }
        
        last = *tokens;
        *tokens = (*tokens)->next;
    }
    while (!emptyTokenStack(arithmeticStack))
    reduce(arithmeticStack,resultStack,tokens);

    while (!emptyTokenStack(relationalStack))
    reduce(relationalStack,resultStack,tokens);

    while(!emptyTokenStack(logicalStack))
    reduce(logicalStack,resultStack,tokens);
    
    return popNode(resultStack,tokens);
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

Token* popToken(TokenStack* stack,Token** tokens){
    if(stack->top == NULL){
        invalidSyntax(tokens);
    }
    TokenStackNode* temp = stack->top->next;
    Token* token = stack->top->token;
    free(stack->top);
    stack->top = temp;
    return token;
}

ASTNode* popNode(TreeStack* stack,Token** tokens){
    if(stack->top == NULL){
        invalidSyntax(tokens);
    }
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
    if(type == NEG)return 4;
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


Token* createNegToken(){
    Token* negtoken = (Token*)malloc(sizeof(Token));
    negtoken->tokenType = NEG;
    negtoken->lineNum = line;
    negtoken->next = NULL;
    negtoken->value = NULL;
    return negtoken;
}


void reduce(TokenStack* opstack,TreeStack* resultstack,Token** tokens){
    if(opstack->top->token->tokenType == NEG){
        ASTNode* negative = createASTNode(NEGATIVE,NULL);
        addChild(negative,popNode(resultstack,tokens));
        pushNode(resultstack,negative);
        popToken(opstack,tokens);
    }
    else if(opstack->top->token->tokenType == NOT){
        ASTNode* negate = createASTNode(NEGATE,NULL);
        addChild(negate,popNode(resultstack,tokens));
        pushNode(resultstack,negate);
        popToken(opstack,tokens);
    }
    else{
    ASTNode* right = popNode(resultstack,tokens);
    ASTNode* left = popNode(resultstack,tokens);
    pushNode(resultstack,createOpNode(popToken(opstack,tokens)->tokenType,left,right));
    }
}