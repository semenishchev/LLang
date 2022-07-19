#include "main.h"

char* get_cmd_option(char** begin, char** end, const std::string & option) {
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return nullptr;
}

bool is_cmd_option_exists(char** begin, char** end, const std::string& option) {
    return std::find(begin, end, option) != end;
}

int main(int argc, char* argv[]) {
    char* filename = get_cmd_option(argv, argv + argc, "-f");

    if (filename) {
        std::ifstream stream(filename);
        string buff;
        auto* tokens = new std::vector<Token>;
        long line = 1;
        while (std::getline (stream, buff)) {
            Lexer lexer(buff.c_str());
            unsigned long position = 0;
            for (auto token = lexer.next(); !token.is_one_of(Token::Kind::End, Token::Kind::Unexpected); token = lexer.next()) {
                if(token.is(Token::Kind::Unexpected)) {
                    std::cout << "Unexpected token: " << token.lexeme() << " at line: " << token.line << " column: " << token.column << std::endl;
                    break;
                }
                token.line = line;
                position += token.size;
                token.column = position;
                tokens->push_back(token);
            }
            line++;
        }
        stream.close();

        // TODO:
        //  generate LLVM IR
        //  compile generated LLVM IR
        //  parse it
        for (auto token : *tokens) {
            std::cout << token.to_string() << std::endl;
        }
//        ast::Parser parser(tokens);
//        std::for_each(parser.parse().begin(), parser.parse().end(), [](auto& expr) {
//            std::cout << expr->to_string() << std::endl;
//        });
        delete tokens;
    } else {
        cout << "No file specified. Use -f flag to specify a file to compile" << endl;
    }

    return 0;
}