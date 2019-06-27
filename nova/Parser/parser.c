#include "parser.h"

int main(){
    Token* head = tokenize("a + 5");    
    ASTNode* root = parseExpression(&head,eof);
}