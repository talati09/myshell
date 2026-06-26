#ifndef PARSER_H
#define PARSER_H

#include "tokenizer.h"
#include <vector>
using namespace std;

// what a command looks like
struct Command {
    string program;
    vector<string> args;
    string inputFile;    // for <   (empty if none)
    string outputFile;   // for >   (empty if none)
    bool appendOutput;   // true if >> false if >
};

// declaration
vector<Command> parse(vector<Token> tokens);

#endif