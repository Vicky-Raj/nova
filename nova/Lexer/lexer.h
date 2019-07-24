#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
typedef long signed int int64;
extern int line;
typedef enum{
    CONSTANT,
    IDENTIFIER,
    //Arithmetic operators
    PLUS,
    MINUS,
    STAR,
    SLASH,
    CARET,
    PERCENTAGE,
    //Assignment Operators
    EQUAL,
    PLUSEQ,
    MINUSEQ,
    STAREQ,
    SLASHEQ,
    PERCENTAGEEQ,
    CARETEQ,
    //Relational Operators
    GREATER,
    LESSER,
    GREATEREQ,
    LESSEREQ,
    EQUALEQ,
    NOTEQ,
    //Logical Operators
    AND,
    OR,
    NOT,
    //Delim
    OPARA,
    CPARA,
    OCURL,
    CCURL,
    OSQUARE,
    CSQUARE,
    COMMA,
    SEMICOLON,
    COLON,
    //Keywords
    VAR,
    WHILE,
    FOR,
    IF,
    ELIF,
    ELSE,
    OF,
    //end of file
    eof
}TokenType;

typedef enum{
    INT,
    DOUBLE,
    STRING,
    BOOL,
    null
}DataType;

typedef struct Token
{
    TokenType tokenType;
    DataType dataType;
    char* value;
    int lineNum;
    struct Token* next;
}Token;

typedef struct TokenList{
    Token* root;
    Token* end;
}TokenList;

//Token functions -- lexerhelper.c
TokenList* initTokenList();
Token* tokenize(char* data);
Token* createToken(TokenType tokentype,DataType datatype,char* value);
void addToken(TokenList* list,Token* token);

//Handlers
int numHandler(int ptr,char*data,TokenList* tokens);//constlexer.c
int strConstHandler(int ptr,char*data,TokenList* tokens,char delim);//constlexer.c
int wordHandler(int ptr,char* data,TokenList* tokens);//wordlexer.c
int delimHandler(int ptr,char* data,TokenList* tokens);
int opHandler(int ptr,char* data,TokenList* tokens);//oplexer.c
int arithmeticHandler(int ptr,char* data,TokenList* tokens);
int relationalHandler(int ptr,char* data,TokenList* tokens);
int spaceHandler(int ptr,char* data);//lexerhelper.c

//Helper Fucntions -- lexerhelper.c
int isDigit(char data);
int isAlphabet(char data);
int charToNum(char data);
int isDelim(char data);
int isArithmetic(char data);
int isRelational(char data);
