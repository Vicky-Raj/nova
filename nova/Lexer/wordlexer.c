#include "lexer.h"

int wordHandler(int ptr,char* data,TokenList* tokens){
    char* tempbuff = NULL;
    while (isAlphabet(data[ptr]) || isDigit(data[ptr]))
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
    if(!strcmp(tempbuff,"var"))addToken(tokens,createToken(VAR,null,NULL));
    //boolean constants
    else if(!strcmp(tempbuff,"true")){
        bool* state = (bool*)malloc(sizeof(bool));*state =1;
        addToken(tokens,createToken(CONSTANT,BOOL,state));
    }
    else if(!strcmp(tempbuff,"false")){
        bool* state = (bool*)malloc(sizeof(bool));*state = 0;
        addToken(tokens,createToken(CONSTANT,BOOL,state));
    }
    //logical operators
    else if(!strcmp(tempbuff,"and"))addToken(tokens,createToken(AND,null,NULL));
    else if(!strcmp(tempbuff,"or"))addToken(tokens,createToken(OR,null,NULL));
    else if(!strcmp(tempbuff,"not"))addToken(tokens,createToken(NOT,null,NULL));

    else addToken(tokens,createToken(IDENTIFIER,null,tempbuff));
    return ptr;
}