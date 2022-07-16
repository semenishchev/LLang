//
// Created by Sasha on 15.07.2022.
//

#include "parser.h"
#include "ast.h"

using namespace ast;
using std::unique_ptr;


Token ast::Parser::get_next_token() {
    return tokens.at(++current_index);
}

Token ast::Parser::get_current_token() {
    return tokens.at(current_index);
}

std::vector<std::unique_ptr<ExprAST>> ast::Parser::parse() {
    eat();
    return parse_start();
}
