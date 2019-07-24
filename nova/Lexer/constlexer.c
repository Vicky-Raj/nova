#include "lexer.h"

//Used by lexer to handle numbers

int numHandler(int ptr,char* data,TokenList* tokens){
    char* numString = NULL;
    while (isDigit(data[ptr]))
    {   
        if(numString == NULL){
            numString = (char*)malloc(sizeof(char)*2);
            numString[0] = data[ptr++];
            numString[1] = '\0';
        }
        else
        {
            int len = strlen(numString);
            numString = (char*)realloc(numString,sizeof(char)*(len+2));
            numString[len] = data[ptr++];
            numString[len+1] = '\0';
        }
        
    }
    if (data[ptr] == '.')
    {   ptr++;
        int len = strlen(numString);
        numString = (char*)realloc(numString,sizeof(char)*(len+3));
        numString[len] = '.';
        numString[len+1] = '0';
        numString[len+2] = '\0';
        if(isDigit(data[ptr])){
            int len = strlen(numString);
            numString[len-1] = data[ptr++];
            while (isDigit(data[ptr]))
            {
                int len = strlen(numString);
                numString = (char*)realloc(numString,sizeof(char)*(len+2));
                numString[len] = data[ptr++];
                numString[len+1] = '\0';
            }
        }
        addToken(tokens,createToken(CONSTANT,DOUBLE,numString));
        return ptr;
    }
    addToken(tokens,createToken(CONSTANT,INT,numString));
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
