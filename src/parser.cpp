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
    std::vector<std::unique_ptr<ExprAST>> result =  parse_start();
    bool error = false;
    for(const std::string& required_type : required_types) {
        bool matched = false;
        for(std::unique_ptr<TypenameAST>& typename_ast : parsed_types) {
            if(typename_ast->name == required_type) {
                matched = true;
                break;
            }
        }

        if(!matched) {
            error = true;
            std::cerr << "Undefined type " << required_type << std::endl;
        }
    }
    if(error) {
        exit(1);
    }
    return result;
}
