#include "main.h"

long GetFileSize(std::string filename)
{
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

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
        auto* code = new std::string();
        string buff;
        while (std::getline (stream, buff)) {
            // Output the text from the file
            code->append(buff);
            code->append("\n");
        }
        stream.close();
        auto* tokens = new std::vector<Token>;
        Lexer lexer(reinterpret_cast<const char*>(code->c_str()));
        for (auto token = lexer.next(); !token.is_one_of(Token::Kind::End, Token::Kind::Unexpected); token = lexer.next()) {
            tokens->push_back(token);
        }
        // TODO:
        //  generate LLVM IR
        //  compile generated LLVM IR
        //  parse it
        for (auto token : *tokens) {
            std::cout << token.to_string() << std::endl;
        }
        ast::Parser parser(tokens);

        delete tokens;
    } else {
        cout << "No file specified. Use -f flag to specify a file to compile" << endl;
    }

    return 0;
}