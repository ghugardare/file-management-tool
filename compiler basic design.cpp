#include <iostream>
#include <string>
#include <cctype>

using namespace std;


enum TokenType {
    IDENTIFIER, NUMBER, OPERATOR, END
};

struct Token {
    TokenType type;
    string value;

class Lexer {
    string input;
    size_t pos;
public:
    Lexer(string text) : input(text), pos(0) {}
    
    Token getNextToken() {
        while (pos < input.size() && isspace(input[pos])) {
            pos++; // Skip whitespace
        }
        
        if (pos >= input.size()) {
            return {END, ""};
        }
        
        if (isalpha(input[pos])) { // Identifier
            string id;
            while (pos < input.size() && isalnum(input[pos])) {
                id += input[pos++];
            }
            return {IDENTIFIER, id};
        }
        
        if (isdigit(input[pos])) { // Number
            string num;
            while (pos < input.size() && isdigit(input[pos])) {
                num += input[pos++];
            }
            return {NUMBER, num};
        }
        
        if (input[pos] == '+' || input[pos] == '-' || input[pos] == '*' || input[pos] == '/') { // Operator
            return {OPERATOR, string(1, input[pos++])};
        }
        
        return {END, ""};
    }
};


class Parser {
    Lexer lexer;
    Token currentToken;
public:
    Parser(Lexer lex) : lexer(lex) {
        currentToken = lexer.getNextToken();
    }
    
    void parse() {
        while (currentToken.type != END) {
            cout << "Token: " << currentToken.value << endl;
            currentToken = lexer.getNextToken();
        }
    }
};

int main() {
    string input;
    cout << "Enter an expression: ";
    getline(cin, input);
    
    Lexer lexer(input);
    Parser parser(lexer);
    parser.parse();
    
    return 0;
}
