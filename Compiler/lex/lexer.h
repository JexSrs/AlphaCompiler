#ifndef PHASE1_LEXER_H
#define PHASE1_LEXER_H

#include "stdio.h"
#include "string.h"
#include <stdlib.h>

enum token_enum{
    KEYWORD,
    INTCONST,
    OPERATOR,
    DOUBLECONST,
    STRINGCONST,
    PUNCTUATION,
    IDENT,
    COMMENTS
};

typedef struct alpha_token_s {
    unsigned int numline;
    unsigned int numToken;
    char *content;
    enum token_enum type;
    struct alpha_token_s *next;
} *alpha_token_t;

int line_number_glb = 1; // o ari8mos gia tin grammi
int token_number_glb = 0; // o ari8mos gia to periexomeno
int string_starting_line;

void multiline_comments_check(alpha_token_t token, char* token_content, enum token_enum token_type);
void string_check(alpha_token_t token ,char* token_content, enum token_enum token_type);

#endif //PHASE1_LEXER_H
