#include <iostream>
#include <string>
#include <unistd.h>
#include <signal.h>        // ← add this for signals
#include "tokenizer.h"
#include "parser.h"
#include "builtins.h"
#include "executor.h"
using namespace std;

// this function runs when Ctrl+C is pressed
// and no command is running
void handleSIGINT(int sig) {
    cout << "\nmysh> ";
    flush(cout);
}

int main() {
    string input;

    // tell shell to call handleSIGINT when Ctrl+C pressed
    signal(SIGINT, handleSIGINT);

    while(true) {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        cout << "mysh:" << cwd << "> ";
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

        vector<Token> tokens = tokenize(input);
        vector<Command> commands = parse(tokens);

        if(commands.size() == 1) {
            bool wasBuiltin = runBuiltin(commands[0]);
            if(!wasBuiltin) {
                // ignore SIGINT in parent while child runs
                signal(SIGINT, SIG_IGN);

                executeCommand(commands[0]);

                // restore handler after child finishes
                signal(SIGINT, handleSIGINT);
            }
        }
        else if(commands.size() > 1) {
            // ignore SIGINT in parent while pipeline runs
            signal(SIGINT, SIG_IGN);

            executePipeline(commands);

            // restore handler after pipeline finishes
            signal(SIGINT, handleSIGINT);
        }
    }

    return 0;
}