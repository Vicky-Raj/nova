#include "lexer.h"

int wordHandler(int ptr,char* data,TokenList* tokens){
    char* tempbuff = NULL;
    while (isAlphabet(data[ptr]) || isDigit(data[ptr]) || data[ptr] == '_')
    {
        if(tempbuff == NULL){
            tempbuff = (char*)malloc(sizeof(char)*2);
            tempbuff[0] = data[ptr++];
            tempbuff[1] = '\0';
        }else
        {
            int len = strlen(tempbuff);
            tempbuff = (char*)realloc(tempbuff,sizeof(char)*(len+2));
            tempbuff[len] = data[ptr++];
            tempbuff[len+1] = '\0';
        }       
    }
    //keywords
    if(!strcmp(tempbuff,"var"))addToken(tokens,createToken(VAR,null,NULL));
    else if (!strcmp(tempbuff,"while"))addToken(tokens,createToken(WHILE,null,NULL));
    else if (!strcmp(tempbuff,"for"))addToken(tokens,createToken(FOR,null,NULL));
    else if (!strcmp(tempbuff,"if"))addToken(tokens,createToken(IF,null,NULL));
    else if(!strcmp(tempbuff,"elif"))addToken(tokens,createToken(ELIF,null,NULL));
    else if(!strcmp(tempbuff,"else"))addToken(tokens,createToken(ELSE,null,NULL));
    else if(!strcmp(tempbuff,"of"))addToken(tokens,createToken(OF,null,NULL));
    //boolean constants
    else if(!strcmp(tempbuff,"true")){
        char* value = (char*)malloc(sizeof(char));*value = '1';
        addToken(tokens,createToken(CONSTANT,BOOL,value));
    }
    else if(!strcmp(tempbuff,"false")){
        char* value = (char*)malloc(sizeof(char));*value = '0';
        addToken(tokens,createToken(CONSTANT,BOOL,value));
    }
    //null
    else if(!strcmp(tempbuff,"null"))addToken(tokens,createToken(CONSTANT,null,NULL));
    //logical operators
    else if(!strcmp(tempbuff,"and"))addToken(tokens,createToken(AND,null,NULL));
    else if(!strcmp(tempbuff,"or"))addToken(tokens,createToken(OR,null,NULL));
    else if(!strcmp(tempbuff,"not"))addToken(tokens,createToken(NOT,null,NULL));

    else addToken(tokens,createToken(IDENTIFIER,null,tempbuff));
    return ptr;
}