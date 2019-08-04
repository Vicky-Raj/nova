#include "lexer.h"

int arithmeticHandler(int ptr,char* data,TokenList* tokens){
    if(data[ptr+1] == '='){
        switch (data[ptr++])
        {
            case '+':addToken(tokens,createToken(PLUSEQ,null,NULL));break;
            case '-':addToken(tokens,createToken(MINUSEQ,null,NULL));break;
            case '*':addToken(tokens,createToken(STAREQ,null,NULL));break;
            case '/':addToken(tokens,createToken(SLASHEQ,null,NULL));break;
            case '%':addToken(tokens,createToken(PERCENTAGEEQ,null,NULL));break;
            case '^':addToken(tokens,createToken(CARETEQ,null,NULL));break;
        }
        return ++ptr;
    }else
    {
        switch (data[ptr])
        {
            case '+':addToken(tokens,createToken(PLUS,null,NULL));break;
            case '-':addToken(tokens,createToken(MINUS,null,NULL));break;
            case '*':addToken(tokens,createToken(STAR,null,NULL));break;
            case '/':addToken(tokens,createToken(SLASH,null,NULL));break;
            case '%':addToken(tokens,createToken(PERCENTAGE,null,NULL));break;
            case '^':addToken(tokens,createToken(CARET,null,NULL));break;
        }
        return ++ptr;
    }
}


int relationalHandler(int ptr,char* data,TokenList* tokens){
    if(data[ptr+1] == '='){
        switch (data[ptr++])
        {
            case '<':addToken(tokens,createToken(LESSEREQ,null,NULL));break;
            case '>':addToken(tokens,createToken(GREATEREQ,null,NULL));break;
            case '!':addToken(tokens,createToken(NOTEQ,null,NULL));break;
        }
        return ++ptr;
    }else
    {
        switch (data[ptr])
        {
            case '<':addToken(tokens,createToken(LESSER,null,NULL));break;
            case '>':addToken(tokens,createToken(GREATER,null,NULL));break;
        }
        return ++ptr;
    }
}