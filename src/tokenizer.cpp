#include "tokenizer.h"
#include <iostream>
using namespace std;

vector<Token> tokenize(string input) {
    vector<Token> tokens;
    int i = 0;

    while(i < input.length()) {
        if(input[i] == ' ') {
            i++;
            continue;
        }

        if(input[i] == '|') {
            Token t;
            t.type = PIPE;
            t.value = "|";
            tokens.push_back(t);
            i++;
            continue;
        }
        
          // >> append (check BEFORE > so we dont miss it)
         if(input[i] == '>' && i+1 < input.length() && input[i+1] == '>') {
            Token t;
            t.type = REDIR_APPEND;
            t.value = ">>";
            tokens.push_back(t);
            i += 2;  // skip both > characters
            continue;
        }
            // > output redirect
           if(input[i] == '>') {
            Token t;
            t.type = REDIR_OUT;
            t.value = ">";
            tokens.push_back(t);
            i++;
            continue;
        }
        //input redirect
         if(input[i] == '<') {
            Token t;
            t.type = REDIR_IN;
            t.value = "<";
            tokens.push_back(t);
            i++;
            continue;
        }


        string word = "";
        while(i < input.length() && input[i] != ' ' && input[i] != '|'&&input[i]!='<'&&input[i]!='>') {
            word += input[i];
            i++;
        }
        Token t;
        t.type = WORD;
        t.value = word;
        tokens.push_back(t);
    }

    Token endtoken;
    endtoken.type = END;
    endtoken.value = "";
    tokens.push_back(endtoken);

    return tokens;
}