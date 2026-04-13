all:
	g++ -o myshell shell.cpp tokenizer.cpp parser.cpp builtins.cpp

clean:
	rm -f myshell