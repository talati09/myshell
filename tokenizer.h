#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>
using namespace std;

// token types
enum TokenType {
     WORD,
    PIPE,
    REDIR_OUT,     // >
    REDIR_IN,      // 
    REDIR_APPEND,  // >>
    END
};

// what a token looks like
struct Token {
    TokenType type;
    string value;
};

// declaration — tells other files this function exists
vector<Token> tokenize(string input);

#endif