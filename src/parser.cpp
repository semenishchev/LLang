//
// Created by Sasha on 15.07.2022.
//

#include "parser.h"

using enum Token::Kind;

Token ast::Parser::get_next_token() {
    return tokens.at(++current_index);
}

void ast::Parser::parse() {
    std::vector<ExprAST> ast;
    std::unique_ptr<ExprAST> expr;
    eat();
    Token::Kind expected;
    switch (get_current_token().kind()) {
        case Token::Kind::Identifier:
            expr = parse_identifier_expr();
            break;
        default:
            expr = nullptr;
            expected = Token::Kind::Identifier;
            break;
    }

    if(expr.get() == nullptr) {
        error(token, expected)
        break;
    }
}

Token ast::Parser::get_current_token() {
    return tokens.at(current_index);
}

std::unique_ptr<ExprAST> ast::Parser::parse_number_expr() {
    auto value = std::make_unique<NumberExprAST>(std::stod(get_current_token().lexeme()));
    eat();
    return std::move(value);
}

std::unique_ptr<ExprAST> ast::Parser::parse_paren_expr() {
    return std::unique_ptr<ExprAST>();
}

std::unique_ptr<ExprAST> ast::Parser::parse_identifier_expr() {
    Token current = get_current_token();
    Token next = get_next_token();
    if(next.is_not(Token::Kind::LeftParen)) {
        return std::move(std::make_unique<VariableExprAST>(current.lexeme()));
    }
    std::vector<std::unique_ptr<ExprAST>> args;
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
    return std::unique_ptr<ExprAST>();
}

std::unique_ptr<ExprAST> ast::Parser::parse_primary_expr() {
    return std::unique_ptr<ExprAST>();
}

std::unique_ptr<ExprAST> ast::Parser::parse_unary_expr() {
    return std::unique_ptr<ExprAST>();
}

std::unique_ptr<ExprAST> ast::Parser::parse_class_def_expr() {
    return std::unique_ptr<ExprAST>();
}

std::unique_ptr<ExprAST> ast::Parser::parse_outside_inheritance_expr() {
    return std::unique_ptr<ExprAST>();
}

std::unique_ptr<ExprAST> ast::Parser::parse_expr() {
    return std::unique_ptr<ExprAST>();
}
