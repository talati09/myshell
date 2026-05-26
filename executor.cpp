#include "executor.h"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
using namespace std;

void executeCommand(Command cmd) {
    // ... your existing code stays exactly the same
}

void executePipeline(vector<Command> commands) {

    // total commands
    int n = commands.size();

    // we need n-1 pipes
    // each pipe has 2 file descriptors [read, write]
    // so we make a 2D array
    vector<array<int, 2>> pipes(n - 1);

    // create all pipes first
    for(int i = 0; i < n - 1; i++) {
        if(pipe(pipes[i].data()) < 0) {
            cout << "pipe creation failed\n";
            return;
        }
    }

    // store all child pids so parent can wait for them
    vector<int> pids;

    // fork a child for each command
    for(int i = 0; i < n; i++) {

        int pid = fork();

        if(pid == 0) {
            // ---- CHILD PROCESS ----

            // if NOT the first command
            // read input from previous pipe
            if(i > 0) {
                dup2(pipes[i-1][0], 0);  // stdin = read end of previous pipe
            }

            // if NOT the last command
            // write output to current pipe
            if(i < n - 1) {
                dup2(pipes[i][1], 1);    // stdout = write end of current pipe
            }

            // close ALL pipe ends in child
            // child only needs the ones it dup2'd
            for(int j = 0; j < n - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            // handle redirections if any
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

            // build args and execute
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
            // ---- PARENT PROCESS ----
            pids.push_back(pid);  // save child pid
        }
        else {
            cout << "fork failed\n";
            return;
        }
    }

    // parent closes ALL pipe ends
    // very important! otherwise shell hangs forever
    for(int i = 0; i < n - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // wait for ALL children to finish
    for(int i = 0; i < pids.size(); i++) {
        int status;
        waitpid(pids[i], &status, 0);
    }
}