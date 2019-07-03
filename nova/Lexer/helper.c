#include "lexer.h"

int isDigit(char data){
    return(data >= '0' && data <= '9');
}

int isAlphabet(char data){
    return(data >= 'A' && data <= 'Z' || data >= 'a' && data <= 'z');
}

int charToNum(char data){
    return(data - '0');
}

int isDelim(char data){
    return(data == '(' || data == ')'||
    data == '{' || data == '}'||
    data == '[' || data == ']'||
    data == ',' || data == ';'|| 
    data == ':');
}

int isArithmetic(char data){
    return(data == '+' || data == '-' || data == '*' || data == '/' || data == '%' || data == '^');
}
int isRelational(char data){
    return(data == '<' || data == '>' || data == '!');
}

TokenList* initTokenList(){
    TokenList* tokenlist = (TokenList*)malloc(sizeof(TokenList));
    tokenlist->root = NULL;
    tokenlist->end = NULL;
    return tokenlist;
}
Token* createToken(TokenType tokentype,DataType datatype,void* value){
    Token* token = (Token*)malloc(sizeof(Token));
    token->tokenType = tokentype;
    token->dataType = datatype;
    token->value = value;
    return token;
}
void addToken(TokenList* list,Token* token){
    if(list->root == NULL){
        list->root = token;
        list->end = token;
        list->root->next = NULL;
    }else
    {
        list->end->next = token;
        list->end = list->end->next;
        list->end->next = NULL;
    }
}

int spaceHandler(int ptr,char* data){
    while(data[ptr] == ' ')ptr++;
    return ptr;
}