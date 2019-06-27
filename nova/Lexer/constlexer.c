#include "lexer.h"

//Used by lexer to handle numbers

int numHandler(int ptr,char* data,TokenList* tokens){
    int64 lvalue=0,rvalue=0;double weight = 1;
    while(isDigit(data[ptr])){
        lvalue = lvalue * 10 + charToNum(data[ptr++]);
    }
    if(data[ptr] == '.'){
        ptr++;
        while (data[ptr])
        {
            rvalue = rvalue * 10 + charToNum(data[ptr++]);
            weight = weight/10;
        }
        double* dvalue = (double*)malloc(sizeof(double));
        *dvalue = lvalue + weight * rvalue;
        addToken(tokens,createToken(CONSTANT,DOUBLE,dvalue));
        return ptr;
    }
    int64* ivalue = (int64*)malloc(sizeof(int64));
    *ivalue = lvalue;
    addToken(tokens,createToken(CONSTANT,INT,ivalue));
    return ptr;
}

int strConstHandler(int ptr,char*data,TokenList* tokens,char delim){
    char* string = NULL;ptr++;
    while(data[ptr] != delim && data[ptr] != '\0'){
        if(string == NULL){
            string = (char*)malloc(sizeof(char)*2);
            string[0] = data[ptr++];
            string[1] = '\0';
        }else
        {   
            int len = strlen(string);
            string = (char*)realloc(string,sizeof(char)*(len+2));
            string[len] = data[ptr++];
            string[len+1] = '\0';
        }
    }
    if(data[ptr] == '\0'){printf("Lexical Error: Cannot find %c\n",delim);exit(0);}else ptr++;    
    addToken(tokens,createToken(CONSTANT,STRING,string));
    return ptr;
}
