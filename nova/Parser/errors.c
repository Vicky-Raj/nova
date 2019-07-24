#include "parser.h"

void invalidSyntax(Token** tokens){
    printf("Invalid syntax at line %d\n",(*tokens)->lineNum);
    exit(0);
}

void missingToken(TokenType type,Token** tokens){
    char tok;
    switch (type)
    {
        case SEMICOLON:tok = ';';break;
        case COLON:tok = ':';break;
        case CPARA:tok = ')';break;
        case CSQUARE:tok = ']';break;
        case CCURL:tok = '}';break;
        case OPARA:tok = '(';break;
    }
    printf("Missing '%c' at line %d\n",tok,(*tokens)->lineNum);
    exit(0);
}

void missingCond(Token** tokens){
    printf("Missing 'condition' at line %d\n",(*tokens)->lineNum);
    exit(0);
}