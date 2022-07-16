#pragma once

//
// Created by Sasha on 15.07.2022.
//

#include "ast.h"

namespace ast {
    class Parser {
    public:
        explicit Parser(std::vector<Token>* tokens) {
            this->tokens = *tokens;
        }

        std::vector<std::unique_ptr<ExprAST>> parse();
        Token get_next_token();
        Token get_current_token();

        void eat() {
            current_index++;
        }

        void error(Token token, Token::Kind expected) {
            std::cout << "Unexpected token " << token.lexeme() << " ( " << token.kind_str() << "). Expected " << Token::kind_to_string(expected);
            std::exit(-1);
        }

        void error(Token token, Token::Kind expected, Token::Kind or_expected) {
            std::cout << "Unexpected token " << token.lexeme() << " ( " << token.kind_str() << "). Expected " << Token::kind_to_string(expected) << " or " << Token::kind_to_string(or_expected);
            std::exit(-1);
        }
    private:
        int current_index = -1;
        std::vector<Token> tokens;
    };

}