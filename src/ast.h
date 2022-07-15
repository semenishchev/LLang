#pragma once

#include <utility>
#include <memory>

#include "lexer.h"

namespace ast {
    class ExprAST {
    public:
        virtual ~ExprAST() = default;
    };

    class NumberExprAST : public ExprAST {
        double val;

    public:
        NumberExprAST(double val) : val(val) {}
    };

    class VariableExprAST : public ExprAST {
        std::string name;

    public:
        VariableExprAST(std::string name) : name(std::move(name)) {}
    };

    class BinaryExprAST : public ExprAST {
        char op;
        std::unique_ptr<ExprAST> LHS, RHS;

    public:
        BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS,
                      std::unique_ptr<ExprAST> RHS)
                : op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
    };

/// CallExprAST - Expression class for function calls.
    class CallExprAST : public ExprAST {
        std::string callee;
        std::vector<std::pair<std::string, ExprAST>> args;

    public:
        CallExprAST(std::string Callee,
                    std::vector<std::pair<std::string, ExprAST>> args)
                : callee(std::move(Callee)), args(std::move(args)) {}
    };

    class PrototypeAST {
        std::string name;
        std::vector<std::pair<std::string, std::string>> args;

    public:
        PrototypeAST(std::string name, std::vector<std::pair<std::string, std::string>> args)
                : name(std::move(name)), args(std::move(args)) {}

        const std::string &getName() const { return name; }
    };

/// FunctionAST - This class represents a function definition itself.
    class FunctionAST {
        std::unique_ptr<PrototypeAST> proto;
        std::unique_ptr<ExprAST> body;

    public:
        FunctionAST(std::unique_ptr<PrototypeAST> proto,
                    std::unique_ptr<ExprAST> body)
                : proto(std::move(proto)), body(std::move(body)) {}
    };
}
