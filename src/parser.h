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

        std::vector<std::unique_ptr<ExprAST>> parse_start() {
            eat();
            std::vector<std::unique_ptr<ExprAST>> ast;

            while(get_current_token().is_not(Token::Kind::End)) {
                Token token = get_current_token();
                if(token.is(Token::Kind::Identifier)) {

                } else {
                    error(token, Token::Kind::Identifier);
                    break;
                }
                eat();
            }

            return ast;
        }

        std::unique_ptr<ExprAST> parse_identifier() {
            std::string token_value = get_current_token().lexeme();
            if(token_value == "class") {
                return parse_class_declaration();
            } else if(token_value == "interface") {

            } else if(token_value == "impl") {

            } else {
                // define variable
            }

            return nullptr;
        }

        std::unique_ptr<ExprAST> parse_class_declaration() {

        }


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