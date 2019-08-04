#include "lexer.h"

int line = 1;
Token* tokenize(char* data){
    TokenList* tokens = initTokenList();
    for(int i=0;data[i] != '\0';){
        if(data[i] == ' '){
            i = spaceHandler(i,data);
        }
        else if(isDigit(data[i])){
            i = numHandler(i,data,tokens);
        }
        else if(data[i] == '"' || data[i] == '\''){
            i = strConstHandler(i,data,tokens,data[i]);
        }
        else if(isAlphabet(data[i]) || data[i] == '_'){
            i = wordHandler(i,data,tokens);
        }
        else if(isDelim(data[i])){
            i = delimHandler(i,data,tokens);
        }
        else if(isArithmetic(data[i])){
            i = arithmeticHandler(i,data,tokens);
        }
        else if(isRelational(data[i])){
            i = relationalHandler(i,data,tokens);
        }
        else if(data[i] == '='){
            if(data[++i] == '='){addToken(tokens,createToken(EQUALEQ,null,NULL));i++;}
            else addToken(tokens,createToken(EQUAL,null,NULL));
        }
        else if(data[i] == '.'){
            addToken(tokens,createToken(DOT,null,NULL));
            i++;
        }
        else if(data[i] == '\n')
        {
            line++;
            i++;
        }
        else
        {   
            printf("Lexical Error:Unrecognized token\n");
            exit(0);
        }
        

    }
    addToken(tokens,createToken(eof,null,NULL));
    return tokens->root;
}

