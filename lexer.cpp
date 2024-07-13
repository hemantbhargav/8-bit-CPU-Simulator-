
#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>

#define MAX_TOKEN_LEN 100

enum TokenType {
    TOKEN_INT, TOKEN_IDENTIFIER, TOKEN_NUMBER, TOKEN_ASSIGN,
    TOKEN_PLUS, TOKEN_MINUS, TOKEN_IF, TOKEN_EQUAL, TOKEN_LBRACE, TOKEN_RBRACE,
    TOKEN_SEMICOLON, TOKEN_UNKNOWN, TOKEN_EOF
};

struct Token {
    TokenType type;
    char text[MAX_TOKEN_LEN];
};

class Lexer {
public:
    Lexer(const std::string &filename) : file(filename) {
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file");
        }
    }

    Token getNextToken() {
        int c;
        while ((c = file.get()) != EOF) {
            if (std::isspace(c)) continue;
            if (std::isalpha(c)) {
                int len = 0;
                token.text[len++] = c;
                while (std::isalnum(c = file.get())) {
                    if (len < MAX_TOKEN_LEN - 1) token.text[len++] = c;
                }
                file.unget();
                token.text[len] = '\0';
                if (strcmp(token.text, "int") == 0) token.type = TOKEN_INT;
                else if (strcmp(token.text, "if") == 0) token.type = TOKEN_IF;
                else token.type = TOKEN_IDENTIFIER;
                return token;
            }
            if (std::isdigit(c)) {
                int len = 0;
                token.text[len++] = c;
                while (std::isdigit(c = file.get())) {
                    if (len < MAX_TOKEN_LEN - 1) token.text[len++] = c;
                }
                file.unget();
                token.text[len] = '\0';
                token.type = TOKEN_NUMBER;
                return token;
            }
            switch (c) {
                case '=': token.type = TOKEN_ASSIGN; token.text[0] = '='; token.text[1] = '\0'; return token;
                case '+': token.type = TOKEN_PLUS; token.text[0] = '+'; token.text[1] = '\0'; return token;
                case '-': token.type = TOKEN_MINUS; token.text[0] = '-'; token.text[1] = '\0'; return token;
                case '{': token.type = TOKEN_LBRACE; token.text[0] = '{'; token.text[1] = '\0'; return token;
                case '}': token.type = TOKEN_RBRACE; token.text[0] = '}'; token.text[1] = '\0'; return token;
                case ';': token.type = TOKEN_SEMICOLON; token.text[0] = ';'; token.text[1] = '\0'; return token;
                default: token.type = TOKEN_UNKNOWN; token.text[0] = c; token.text[1] = '\0'; return token;
            }
        }
        token.type = TOKEN_EOF;
        token.text[0] = '\0';
        return token;
    }

private:
    std::ifstream file;
    Token token;
};

int main() {
    try {
        Lexer lexer("input.txt");
        Token token;
        do {
            token = lexer.getNextToken();
            std::cout << "Text: " << token.text <<","<< "     Token: " << token.type << std::endl;
        } while (token.type != TOKEN_EOF);
    } catch (const std::exception &ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
