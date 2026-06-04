all:
	g++ -o myshell shell.cpp tokenizer.cpp parser.cpp builtins.cpp executor.cpp history.cpp

clean:
	rm -f myshell