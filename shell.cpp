#include <iostream>
#include <string>
#include <unistd.h>
#include <signal.h>
#include "tokenizer.h"
#include "parser.h"
#include "builtins.h"
#include "executor.h"
#include "history.h"       // ← add this
using namespace std;

void handleSIGINT(int sig) {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    cout << "\nmysh:" << cwd << "> ";
    flush(cout);
    signal(SIGINT, handleSIGINT);
}

int main() {
    string input;
    vector<string> history;    // ← add this

    signal(SIGINT, handleSIGINT);

    while(true) {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        cout << "mysh:" << cwd << "> ";
        flush(cout);

        // use readInput instead of getline
        input = readInput(history);    // ← replace getline with this

        if(input.empty()) continue;

        if(input == "exit") {
            cout << "Good Bye\n";
            break;
        }

        // add to history
        history.push_back(input);     // ← add this

        vector<Token> tokens = tokenize(input);
        vector<Command> commands = parse(tokens);

        if(commands.size() == 1) {
            bool wasBuiltin = runBuiltin(commands[0]);
            if(!wasBuiltin) {
                signal(SIGINT, SIG_IGN);
                executeCommand(commands[0]);
                signal(SIGINT, handleSIGINT);
            }
        }
        else if(commands.size() > 1) {
            signal(SIGINT, SIG_IGN);
            executePipeline(commands);
            signal(SIGINT, handleSIGINT);
        }
    }

    return 0;
}