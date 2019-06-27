#include "lexer.h"

int delimHandler(int ptr, char* data,TokenList* tokens){
    switch (data[ptr])
    {
        case '(':addToken(tokens,createToken(OPARA,null,NULL));break;
        case ')':addToken(tokens,createToken(CPARA,null,NULL));break;
        case '{':addToken(tokens,createToken(OCURL,null,NULL));break;
        case '}':addToken(tokens,createToken(CCURL,null,NULL));break;
        case '[':addToken(tokens,createToken(OSQUARE,null,NULL));break;
        case ']':addToken(tokens,createToken(CSQUARE,null,NULL));break;
        case ',':addToken(tokens,createToken(COMMA,null,NULL));break;
        case ';':addToken(tokens,createToken(SEMICOLON,null,NULL));break;
    }
    return ++ptr;
}