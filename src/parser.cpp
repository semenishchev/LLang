//
// Created by Sasha on 15.07.2022.
//

#include "parser.h"

using enum Token::Kind;
using std::unique_ptr;

Token ast::Parser::get_next_token() {
    return tokens.at(++current_index);
}

Token ast::Parser::get_current_token() {
    return tokens.at(current_index);
}

std::vector<unique_ptr<ExprAST>> ast::Parser::parse() {
    std::vector<unique_ptr<ExprAST>>
    eat();
    
    return ast;
}

unique_ptr<ExprAST> parse_file() {
    while(!get_current_token().is(Token::Kind::End))) {
        eat();
        if(get_current_token().is(Token::Kind::Identifier)) {
            // parse identifier
        }
    }
}

unique_ptr<ExprAST> parse_number_expr() {
    auto value = std::make_unique<NumberExprAST>(std::stod(get_current_token().lexeme()));
    eat();
    return std::move(value);
}

unique_ptr<ExprAST> parse_paren_expr() {
    return unique_ptr<ExprAST>();
}

unique_ptr<ExprAST> parse_identifier_expr() {
    Token current = get_current_token();
    Token next = get_next_token();
    if(next.is_not(Token::Kind::LeftParen)) {
        return std::move(std::make_unique<VariableExprAST>(current.lexeme()));
    }
    std::vector<unique_ptr<ExprAST>> args;
    next = get_next_token();
    if(next.is_not(Token::Kind::RightParen)) { // method has arguments
        while(1) {
            auto arg = parse_expr();
            if(arg) {
                args.push_back(arg);
            } else {
                return nullptr;
            }

            if(get_current_token().is(Token::Kind::RightParen)) {
                break;
            } else if(get_current_token().is_not(Token::Kind::Comma)) {
                error(get_current_token(), Token::Kind::Comma, Token::Kind::RightParen);
            }
            eat();
        }
    }
    eat();
    return unique_ptr<ExprAST>();
}

unique_ptr<ExprAST> parse_primary_expr() {
    return unique_ptr<ExprAST>();
}

unique_ptr<ExprAST> parse_unary_expr() {
    return unique_ptr<ExprAST>();
}

unique_ptr<ExprAST> parse_class_def_expr() {
    return unique_ptr<ExprAST>();
}

unique_ptr<ExprAST> parse_outside_inheritance_expr() {
    return unique_ptr<ExprAST>();
}

unique_ptr<ExprAST> parse_expr() {
    return unique_ptr<ExprAST>();
}
