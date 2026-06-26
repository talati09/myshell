#include "history.h"
#include <iostream>
#include <termios.h>    // raw mode
#include <unistd.h>     // read()
using namespace std;

// switches terminal to raw mode
// returns original settings so we can restore later
struct termios enableRawMode() {
    struct termios original;
    tcgetattr(0, &original);      // save original

    struct termios raw = original;
    raw.c_lflag &= ~(ICANON);    // no line buffering
    raw.c_lflag &= ~(ECHO);      // no auto echo
    tcsetattr(0, TCSANOW, &raw); // apply

    return original;
}

// restores terminal to normal mode
void disableRawMode(struct termios original) {
    tcsetattr(0, TCSANOW, &original);
}

string readInput(vector<string>& history) {
    // switch to raw mode
    struct termios original = enableRawMode();

    string input = "";
    int historyIndex = history.size();  // start at end of history

    char c;

    while(true) {
        // read ONE character at a time
        read(0, &c, 1);

        // CASE 1: Enter pressed
        if(c == '\r' || c == '\n') {
            cout << "\n";
            flush(cout);
            break;
        }

        // CASE 2: Ctrl+C (byte value 3)
        if(c == 3) {
            input = "";
            cout << "\n";
            flush(cout);
            break;
        }

        // CASE 3: Backspace (byte value 127)
        if(c == 127) {
            if(!input.empty()) {
                input.pop_back();           // remove last char
                cout << "\b \b";           // erase from screen
                flush(cout);
            }
            continue;
        }

        // CASE 4: ESC sequence (arrow keys)
        // arrow keys send 3 bytes: 27, 91, 65/66/67/68
        if(c == 27) {
            char seq1, seq2;
            read(0, &seq1, 1);   // read [
            read(0, &seq2, 1);   // read A/B/C/D

            // UP arrow (65)
            if(seq1 == '[' && seq2 == 'A') {
                if(historyIndex > 0) {
                    historyIndex--;

                    // clear current line
                    cout << "\r\033[K";    // go to line start, clear line
                    flush(cout);

                    // reprint prompt
                    char cwd[1024];
                    getcwd(cwd, sizeof(cwd));
                    cout << "mysh:" << cwd << "> ";

                    // show history item
                    input = history[historyIndex];
                    cout << input;
                    flush(cout);
                }
            }

            // DOWN arrow (66)
            if(seq1 == '[' && seq2 == 'B') {
                if(historyIndex < (int)history.size() - 1) {
                    historyIndex++;

                    // clear current line
                    cout << "\r\033[K";
                    flush(cout);

                    // reprint prompt
                    char cwd[1024];
                    getcwd(cwd, sizeof(cwd));
                    cout << "mysh:" << cwd << "> ";

                    // show history item
                    input = history[historyIndex];
                    cout << input;
                    flush(cout);
                }
                else {
                    // went past end → clear input
                    historyIndex = history.size();
                    cout << "\r\033[K";
                    flush(cout);

                    char cwd[1024];
                    getcwd(cwd, sizeof(cwd));
                    cout << "mysh:" << cwd << "> ";
                    flush(cout);

                    input = "";
                }
            }
            continue;
        }

        // CASE 5: normal character
        input += c;
        cout << c;
        flush(cout);
    }

    // restore normal terminal mode
    disableRawMode(original);

    return input;
}