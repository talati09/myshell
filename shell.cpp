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
        if(commands.size() == 1){
            // first check if its a builtin
            bool wasBuiltin = runBuiltin(commands[0]);

            // if not a builtin → fork and execute
            if(!wasBuiltin){
                executeCommand(commands[0]);
            }
        }
    }

    return 0;
}
//checking comment---------//