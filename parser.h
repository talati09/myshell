#ifndef PARSER_H
#define PARSER_H

#include "tokenizer.h"
#include <vector>
using namespace std;

// what a command looks like
struct Command {
    string program;
    vector<string> args;
};

// declaration
vector<Command> parse(vector<Token> tokens);

#endif