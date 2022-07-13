#pragma once

#include "lexer.h"

namespace ast {
    class Parser {
    public:
        Parser(std::vector<Token>* tokens) {
            this->current_token = tokens->at(0);
        }
    private:
        Token current_token;
    };

    class ExprAST {
        virtual ~ExprAST();
    };
}
