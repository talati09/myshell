#include "parser.h"
using namespace std;

vector<Command> parse(vector<Token> tokens) {
    vector<Command> commands;
    Command current;

    for(int i = 0; i < tokens.size(); i++) {
        Token t = tokens[i];

        if(t.type == WORD) {
            if(current.program.empty()) {
                current.program = t.value;
            } else {
                current.args.push_back(t.value);
            }
        }
        else if(t.type == PIPE) {
            commands.push_back(current);
            current = Command();
        }
        else if(t.type == END) {
            if(!current.program.empty()) {
                commands.push_back(current);
            }
        }
    }

    return commands;
}