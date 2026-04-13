#include <iostream>
#include <string>
#include "tokenizer.h"
#include "parser.h"
#include "builtins.h"
using namespace std;

int main() {
    string input;

    while(true) {
        cout << "mysh> ";
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

        // builtins
        if(commands.size() == 1) {
            bool wasBuiltin = runBuiltin(commands[0]);
            if(!wasBuiltin) {
                cout << "will execute: " << commands[0].program << "\n";
            }
        }
    }

    return 0;
}