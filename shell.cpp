#include<bits/stdc++.h>
using namespace std;


enum TokenType{ //just giving names to our numbers 
    WORD,
    PIPE,
    END
};
struct Token{
    TokenType type;
    string value;   
};

struct Command{
    string program ;//like ls , grep etc
    vector<string>args;
};

vector<Token> tokenize(string input){
    vector<Token>tokens;

    int i=0;
    while(i<input.length()){

        //case 1 spaces
        if(input[i]==' '){
            i++;
            continue;
        }

        if(input[i]=='|'){
            Token t;
            t.type=PIPE;
            t.value="|";
            tokens.push_back(t);
            i++;
            continue;
        }

        //normal word
        string word="";
        while(i<input.length()&&input[i]!=' '&&input[i]!='|'){
            word+=input[i];
            i++;
        }
        Token t;
        t.type=WORD;
        t.value=word;
        tokens.push_back(t);
    }
    Token endtoken;
    endtoken.type=END;
    endtoken.value="";
    tokens.push_back(endtoken);

    return tokens;
}


vector<Command>parse(vector<Token>tokens){

    vector<Command>commands;

    Command current;

    for(int i=0;i<tokens.size();i++){
        Token t=tokens[i];

        if(t.type==WORD){
             // first word = program name
            if(current.program.empty()){
                current.program=t.value;
            }
            else{
                current.args.push_back(t.value);
            }
        }
         // pipe = current command is done, start new one
        else if(t.type == PIPE){
            commands.push_back(current);
            current = Command();
        }
          // end = save the last command
        else if(t.type == END){
            if(!current.program.empty()){
                commands.push_back(current);
            }
        }
    }
    return commands;

}


int main(){
     string input;
     
     while(true){
        cout<<"mysh> ";
        flush(cout);

        if(!getline(cin,input)){//cin stops at spaces
            cout<<"\nexit\n";
            break;
        }
        if(input.empty()){
            continue;
        }
        if(input=="exit"){
            cout<<"Good Bye"<<endl;
            break;
        }

       //tokenize
        vector<Token>tokens=tokenize(input);

        //parse
        vector<Command>commands=parse(tokens);

        cout<<"tokens found and commands found:\n";

        //printing about the tookens
        for(int i=0;i<tokens.size();i++){
            Token t=tokens[i];
            if(t.type==WORD)
            cout<<" Word"<<t.value<<endl;
            
            else if(t.type==PIPE)
            cout<<" Pipe: "<<t.value<<endl;

            else if(t.type==END)
            cout<<" End"<<endl;
        }
        //printing about the parsed
        cout<<"Commands found:"<<commands.size()<<endl;

        for(int i=0;i<commands.size();i++){
            cout<<"Command "<<i+1<<" is :"<<endl;
            cout<<"    program: "<<commands[i].program<<endl;
            cout<<"    args: ";
            for(int j=0;j<commands[i].args.size();j++){
                 cout<<commands[i].args[j]<< " ";
            }
            cout<<endl;
        }

        cout<<"You Typed:"<<input<<endl;
     }
     return 0;
}