#pragma once

//
// Created by Sasha on 15.07.2022.
//

#include <algorithm>
#include "ast.h"
using std::unique_ptr;

namespace ast {
    class Parser {
    public:
        explicit Parser(std::vector<Token>* tokens) {
            this->tokens = *tokens;
            keywords.emplace_back("public");
            keywords.emplace_back("static");
            keywords.emplace_back("void");
            keywords.emplace_back("null");
            keywords.emplace_back("u8");
            keywords.emplace_back("i16");
            keywords.emplace_back("u16");
            keywords.emplace_back("i32");
            keywords.emplace_back("u32");
            keywords.emplace_back("class");
            keywords.emplace_back("interface");
            keywords.emplace_back("impl");
            keywords.emplace_back("with");
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
                    parse_identifier();
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
            std::vector<std::unique_ptr<MarkerExprAST>> markers;
            while(get_current_token().is(Token::Kind::Identifier) && get_current_token().lexeme().at(0) == '@') {
                markers.push_back(std::make_unique<MarkerExprAST>(get_current_token().lexeme().substr(1)));
                eat();
            }

            if(token_value == "class") {
                return parse_class(markers);
            } else if(token_value == "interface") {
            } else if(token_value == "impl") {

            } else {
                // define variable
            }

            return nullptr;
        }

        std::unique_ptr<ExprAST> parse_class(std::vector<std::unique_ptr<MarkerExprAST>>& markers) {
            Token name_token = get_next_token();
            if(name_token.is(Token::Kind::Identifier)) {
                std::string name = name_token.lexeme();
                std::unique_ptr<ClassExprAST> result = std::make_unique<ClassExprAST>(name_token.lexeme(), std::vector<std::unique_ptr<MemberExprAST>>(), object);
                result->markers = std::move(markers);
                Token brace = get_next_token();
                if(brace.is(Token::Kind::LeftCurly)) {
                    eat();
                    while(get_current_token().is(Token::Kind::Identifier)) {
                        std::unique_ptr<MemberExprAST> member = parse_member();
                        if(member != nullptr) {
                            result->members.push_back(std::move(member));
                        }
                    }
                    Token right_brace = get_current_token();
                    if(right_brace.is(Token::Kind::RightCurly)) {
                        eat();
                        return std::move(result);
                    } else {
                        error(right_brace, Token::Kind::RightCurly);
                        return nullptr;
                    }
                } else {
                    error(brace, Token::Kind::LeftCurly);
                    return nullptr;
                }
            } else if(contains(keywords, name_token.lexeme())) {
                error(name_token, Token::Kind::Identifier);
                return nullptr;
            }
            else {
                error(name_token, Token::Kind::Identifier);
                return std::make_unique<NullExprAST>();
            }
        }

        std::unique_ptr<MemberExprAST> parse_member() {
            Token current = get_current_token();
            Visibility visibility = parse_visibility(current.lexeme());
            if(visibility == Visibility::None) {
                Visibility default_visibility = Visibility::Protected;
            } else {
                eat();
            }

            current = get_current_token();
            if(current.is(Token::Kind::Identifier)) {
                // parse all parsed_types
                std::unique_ptr<TypenameAST> type = nullptr;
                for(std::unique_ptr<TypenameAST>& got : parsed_types) {
                    if(got->name == current.lexeme()) {
                        type = std::move(got);
                        break;
                    }
                }
                required_types.push_back(current.lexeme());
                type = type == nullptr ? std::make_unique<TypenameAST>(current.lexeme()) : std::move(type);
                current = get_next_token();
                if(current.is_not(Token::Kind::Identifier)) {
                    error(current, Token::Kind::Identifier);
                    return nullptr;
                }

                std::string name = current.lexeme();
                if(contains(keywords, name)) {
                    error(current, "Unexpected keyword");
                    return nullptr;
                }

                current = get_next_token();
                if(current.is(Token::Kind::LeftParen)) { // method
                    std::unique_ptr<MethodExprAST> result = std::make_unique<MethodExprAST>(visibility, std::move(type), name);
                } else { // field
                    return std::make_unique<FieldExprAST>(visibility, std::move(type), name);
                }

            } else {
                error(current, Token::Kind::Identifier);
                return nullptr;
            }
            return nullptr;
        }

        void eat() {
            current_index++;
        }

        void error(Token token, Token::Kind expected) {
            std::cout << "Unexpected token (" << token.line << ":" << token.column << ")" << token.lexeme() << " ( " << token.kind_str() << "). Expected " << Token::kind_to_string(expected) << std::endl;
            std::exit(-1);
        }

        void error(Token token, Token::Kind expected, Token::Kind or_expected) {
            std::cout << "Unexpected token (" << token.line << ":" << token.column << ")" << token.lexeme() << " ( " << token.kind_str() << "). Expected " << Token::kind_to_string(expected) << " or " << Token::kind_to_string(or_expected) << std::endl;
            std::exit(-1);
        }

        void error(Token token, std::string error) {
            std::cout << error << " (" << token.line << ":" << token.column << ")" << token.lexeme() << " ( " << token.kind_str() << ")" << std::endl;
            std::exit(-1);
        }

        void error(std::string error) {
            std::cout << error << std::endl;
            std::exit(-1);
        }
    private:
        int current_index = -1;
        std::vector<Token> tokens;
        std::vector<std::string> keywords;
        std::vector<std::unique_ptr<TypenameAST>> parsed_types;
        std::vector<std::string> required_types;

        Visibility parse_visibility(const std::string& vis) {
            if(contains(keywords, vis)) {
                error(get_current_token(), Token::Kind::Identifier, Token::Kind::LeftCurly);
                return Visibility::None;
            } else {
                if(vis == "public") {
                    return Visibility::Public;
                } else if(vis == "private") {
                    return Visibility::Private;
                } else if(vis == "protected") {
                    return Visibility::Protected;
                } else {
                    return Visibility::None;
                }
            }
        }

        static bool contains(std::vector<std::string>& vec, std::string value) {
            return std::binary_search(vec.begin(), vec.end(), value);
        }
    };

}