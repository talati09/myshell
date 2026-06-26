#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "parser.h"

void executeCommand(Command cmd);   // single command (already built)
void executePipeline(vector<Command> commands);  // multiple commands

#endif