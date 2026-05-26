#include <iostream>
#include <string>
#include "tokenizer.h"
#include "parser.h"
#include "builtins.h"
#include "executor.h" 
#include <unistd.h>     // ← add this
using namespace std;

int main() {
    string input;

    while(true) {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        cout << "mysh> "<<cwd<<": ";
        flush(cout);

        if(!getline(cin, input)) {
            cout << "\nexit\n";
            break;
        }

        if(input.empty()) continue;

        if(input == "exit") {
            cout << "Good Bye\n";
            break;
        }

        // tokenize
        vector<Token> tokens = tokenize(input);

        // parse
        vector<Command> commands = parse(tokens);

        // execute
         // execute
     if(commands.size() == 1) {
    // single command
    bool wasBuiltin = runBuiltin(commands[0]);
    if(!wasBuiltin) {
        executeCommand(commands[0]);
    }
   }
     else if(commands.size() > 1) {
    // pipeline of commands
    executePipeline(commands);
   }
    }

    return 0;
}
//checking comment---------//