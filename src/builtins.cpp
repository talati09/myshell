#include "builtins.h"
#include <iostream>
#include <unistd.h>    // for getcwd, chdir
#include <stdlib.h>    // for getenv
using namespace std;

bool runBuiltin(Command cmd) {

    if(cmd.program == "pwd") {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        cout << cwd << "\n";
        return true;
    }

    if(cmd.program == "echo") {
        for(int i = 0; i < cmd.args.size(); i++) {
            cout << cmd.args[i] << " ";
        }
        cout << "\n";
        return true;
    }

    if(cmd.program == "cd") {
        if(cmd.args.empty()) {
            chdir(getenv("HOME"));
        } else {
            if(chdir(cmd.args[0].c_str()) != 0) {
                cout << "cd: " << cmd.args[0] << ": No such file or directory\n";
            }
        }
        return true;
    }

    return false;
}