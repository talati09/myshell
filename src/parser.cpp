#include "parser.h"
using namespace std;

vector<Command> parse(vector<Token> tokens) {
    vector<Command> commands;
    Command current;

    current.appendOutput=false; //default;

    for(int i = 0; i < tokens.size(); i++) {
        Token t = tokens[i];

        if(t.type == WORD) {
            if(current.program.empty()) {
                current.program = t.value;
            } else {
                current.args.push_back(t.value);
            }
        }
        else if(t.type==REDIR_OUT){
            i++;// move to next toekn the file name
            current.outputFile=tokens[i].value;
            current.appendOutput=false;
        }
       
        // >> append redirect
        else if(t.type==REDIR_APPEND){
            i++;  // move to next token (the filename)
            current.outputFile = tokens[i].value;
            current.appendOutput = true;
        }
        

        //input redirect

        else if(t.type==REDIR_IN){
            i++;
            current.inputFile=tokens[i].value;
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