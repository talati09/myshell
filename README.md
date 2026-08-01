# MyShell 🐚

A Unix shell built from scratch in C++. MyShell replicates core functionality of shells like bash — reading input, parsing commands, executing programs, handling pipes, redirections, signals, and command history. Every line written to deeply understand how a real shell works under the hood.
---

## Features

- **Command execution** — runs any program available on your system (`ls`, `grep`, `python3`, etc.)
- **Builtin commands** — `cd`, `pwd`, `echo` handled natively by the shell
- **Pipes** — chain commands together with `|` (e.g. `ls | grep cpp | wc -l`)
- **Redirections** — `>` (output), `<` (input), `>>` (append)
- **Signal handling** — Ctrl+C cancels the current command without killing the shell
- **Command history** — press ↑↓ arrow keys to navigate previous commands
- **Dynamic prompt** — always shows your current working directory

---

## Demo

```bash
mysh:/home/user/myshell> ls -la | grep cpp
-rw-r--r-- 1 user user  2048 Jan 01 12:00 shell.cpp
-rw-r--r-- 1 user user  1024 Jan 01 12:00 executor.cpp

mysh:/home/user/myshell> echo hello world
hello world

mysh:/home/user/myshell> ls > output.txt
mysh:/home/user/myshell> cat output.txt
executor.cpp
executor.h
...

mysh:/home/user/myshell> sleep 5
^C
mysh:/home/user/myshell>   ← shell survives Ctrl+C!
```

---

## Build and Run

**Requirements:** Linux (Ubuntu/Debian), g++ compiler

```bash
# clone the repo
git clone https://github.com/talati09/myshell.git
cd myshell

# build
make

# run
./myshell
```

To clean the build:
```bash
make clean
```

---

## Project Structure

```
myshell/
├── shell.cpp        → main REPL loop, signal handling
├── tokenizer.cpp    → breaks input string into tokens
├── tokenizer.h
├── parser.cpp       → builds command structs from tokens
├── parser.h
├── executor.cpp     → fork, execvp, pipes, redirections
├── executor.h
├── builtins.cpp     → cd, pwd, echo
├── builtins.h
├── history.cpp      → raw terminal mode, arrow key handling
├── history.h
└── Makefile
```

---

## Architecture

```
User types: "ls -la | grep cpp > out.txt"
                    │
                    ▼
            ┌───────────────┐
            │   TOKENIZER   │
            │               │
            │ breaks input  │
            │ into tokens   │
            └───────┬───────┘
                    │
        [WORD:ls][WORD:-la][PIPE]
        [WORD:grep][WORD:cpp]
        [REDIR_OUT][WORD:out.txt]
                    │
                    ▼
            ┌───────────────┐
            │    PARSER     │
            │               │
            │ builds command│
            │ structs       │
            └───────┬───────┘
                    │
        Command 1: ls -la
        Command 2: grep cpp → out.txt
                    │
                    ▼
            ┌───────────────┐
            │   EXECUTOR    │
            │               │
            │ fork+execvp   │
            │ pipes+dup2    │
            └───────────────┘
```

---

## How It Works — Key Concepts

### Command Execution
Every external command goes through three system calls:
- `fork()` — creates a child process (exact clone of shell)
- `execvp()` — replaces child process with the actual program
- `waitpid()` — parent shell waits for child to finish

### Pipes
For `ls | grep cpp`:
- Creates a pipe (`pipe()` syscall) — a pair of file descriptors
- Child 1 (`ls`) writes stdout into the pipe's write end
- Child 2 (`grep`) reads stdin from the pipe's read end
- Parent closes both ends after forking to signal EOF

### Redirections
For `ls > out.txt`:
- Child opens `out.txt` with `open()`
- `dup2(fd, 1)` replaces stdout (fd 1) with the file
- `execvp("ls")` runs — ls writes to stdout, which now points to the file

### Signal Handling
- Shell registers `handleSIGINT` via `signal(SIGINT, handler)`
- When Ctrl+C is pressed — shell ignores it (`SIG_IGN`)
- Child processes reset to default (`SIG_DFL`) so they die on Ctrl+C

### History + Arrow Keys
- Terminal switched to raw mode via `termios`
- Every keypress read individually with `read()`
- Arrow keys send 3-byte escape sequences (`27, 91, 65/66`)
- Shell intercepts these and navigates the history vector

---

## Possible Improvements

- Background jobs with `&` operator
- Environment variable expansion (`$HOME`, `$PATH`)
- Tab completion for commands and filenames
- `&&` and `||` operators (conditional execution)
- Stderr redirection (`2>`, `2>&1`)
- Persistent history saved to `~/.myshell_history`
---
