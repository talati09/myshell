#include "executor.h"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <array>
#include <signal.h>     // ← add this for SIG_DFL
using namespace std;

void executeCommand(Command cmd) {

    // build args array for execvp
    vector<char*> args;
    args.push_back((char*)cmd.program.c_str());
    for(int i = 0; i < cmd.args.size(); i++) {
        args.push_back((char*)cmd.args[i].c_str());
    }
    args.push_back(NULL);

    int pid = fork();

    if(pid == 0) {
        // ---- CHILD PROCESS ----

        // reset signal to default so Ctrl+C kills child
        signal(SIGINT, SIG_DFL);   // ← new line

        // handle output redirection
        if(!cmd.outputFile.empty()) {
            int flags;
            if(cmd.appendOutput) {
                flags = O_WRONLY | O_CREAT | O_APPEND;
            }
            else {
                flags = O_WRONLY | O_CREAT | O_TRUNC;
            }
            int fd = open(cmd.outputFile.c_str(), flags, 0644);
            if(fd < 0) {
                cout << "cannot open file: " << cmd.outputFile << "\n";
                exit(1);
            }
            dup2(fd, 1);
            close(fd);
        }

        // handle input redirection
        if(!cmd.inputFile.empty()) {
            int fd = open(cmd.inputFile.c_str(), O_RDONLY);
            if(fd < 0) {
                cout << "cannot open file: " << cmd.inputFile << "\n";
                exit(1);
            }
            dup2(fd, 0);
            close(fd);
        }

        // run the program
        execvp(args[0], args.data());
        cout << cmd.program << ": command not found\n";
        exit(1);
    }
    else if(pid > 0) {
        // ---- PARENT PROCESS ----
        int status;
        waitpid(pid, &status, 0);
    }
    else {
        cout << "fork failed\n";
    }
}

void executePipeline(vector<Command> commands) {

    int n = commands.size();
    vector<array<int, 2>> pipes(n - 1);

    for(int i = 0; i < n - 1; i++) {
        if(pipe(pipes[i].data()) < 0) {
            cout << "pipe creation failed\n";
            return;
        }
    }

    vector<int> pids;

    for(int i = 0; i < n; i++) {

        int pid = fork();

        if(pid == 0) {
            // ---- CHILD PROCESS ----

            // reset signal to default
            signal(SIGINT, SIG_DFL);   // ← new line

            if(i > 0) {
                dup2(pipes[i-1][0], 0);
            }

            if(i < n - 1) {
                dup2(pipes[i][1], 1);
            }

            for(int j = 0; j < n - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            if(!commands[i].outputFile.empty()) {
                int flags = commands[i].appendOutput
                    ? O_WRONLY | O_CREAT | O_APPEND
                    : O_WRONLY | O_CREAT | O_TRUNC;
                int fd = open(commands[i].outputFile.c_str(), flags, 0644);
                dup2(fd, 1);
                close(fd);
            }

            if(!commands[i].inputFile.empty()) {
                int fd = open(commands[i].inputFile.c_str(), O_RDONLY);
                dup2(fd, 0);
                close(fd);
            }

            vector<char*> args;
            args.push_back((char*)commands[i].program.c_str());
            for(int j = 0; j < commands[i].args.size(); j++) {
                args.push_back((char*)commands[i].args[j].c_str());
            }
            args.push_back(NULL);

            execvp(args[0], args.data());
            cout << commands[i].program << ": command not found\n";
            exit(1);
        }
        else if(pid > 0) {
            pids.push_back(pid);
        }
        else {
            cout << "fork failed\n";
            return;
        }
    }

    for(int i = 0; i < n - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    for(int i = 0; i < pids.size(); i++) {
        int status;
        waitpid(pids[i], &status, 0);
    }
}