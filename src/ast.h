#pragma once

#include <utility>
#include <memory>

#include "lexer.h"
#include "system.h"

using std::move;

namespace ast {
    class ExprAST {
    public:
        virtual ~ExprAST() = default;
    };

    class MarkerExprAST : public ExprAST {
    public:
        std::string name;
        MarkerExprAST(std::string name) : name(move(name)) {}
    };

    class MemberExprAST;
    class TypenameAST : public ExprAST {
    public:
        TypenameAST(std::string name) : name(move(name)) {}
        TypenameAST(std::string name, std::vector<std::unique_ptr<MemberExprAST>> members) : name(move(name)), members(move(members)) {}
        std::string name;
        std::vector<std::unique_ptr<MemberExprAST>> members;
        std::vector<std::unique_ptr<MarkerExprAST>> markers;
    };

    class NullExprAST : public ExprAST {};

    class U8ExprAST : public TypenameAST {
    public:
        uint8_t val;
        U8ExprAST(uint8_t val) : TypenameAST("u8"), val(val) {}
    };

    class I16ExprAST : public TypenameAST {
    public:
        int val;
        I16ExprAST(int val) : TypenameAST("i16"), val(val) {}
    };

    class U16ExprAST : public TypenameAST {
    public:
        uint16_t val;
        U16ExprAST(uint16_t val) : TypenameAST("u16"), val(val) {}
    };

    class I32ExprAST : public TypenameAST {
    public:
        int32_t val;
        I32ExprAST(int32_t val) : TypenameAST("i32"), val(val) {}
    };

    class U32ExprAST : public TypenameAST {
    public:
        uint32_t val;
        U32ExprAST(uint32_t val) : TypenameAST("u32"), val(val) {}
    };

    class I64ExprAST : public TypenameAST {
    public:
        int64_t val;
        I64ExprAST(int64_t val) : TypenameAST("i64"), val(val) {}
    };

    class U64ExprAST : public TypenameAST {
    public:
        uint64_t val;
        U64ExprAST(uint64_t val) : TypenameAST("u64"), val(val) {}
    };

    class F32ExprAST : public TypenameAST {
    public:
        float val;
        F32ExprAST(float val) : TypenameAST("f32"), val(val) {}
    };

    class F64ExprAST : public TypenameAST {
    public:
        double val;
        F64ExprAST(double val) : TypenameAST("f64"), val(val) {}
    };

    class BooleanExprAST : public TypenameAST {
    public:
        bool val;
        BooleanExprAST(bool val) : TypenameAST("bool"), val(val) {}
    };

    class StringExprAST : public TypenameAST {
    public:
        types::String val;
        StringExprAST(types::String val) : TypenameAST("string"), val(move(val)) {}
    };

    enum Visibility {
        Public,
        Protected,
        Private,
        None
    };

    class MemberExprAST : public ExprAST {
    public:
        Visibility visibility;
        std::vector<std::unique_ptr<MarkerExprAST>> markers;
        MemberExprAST(Visibility visibility) : visibility(visibility) {}
    };

    class PrototypeAST : public MemberExprAST{
    public:
        std::string name;
        std::unique_ptr<TypenameAST> return_type;
        std::vector<std::pair<std::unique_ptr<TypenameAST>, std::string>> args;
        PrototypeAST(Visibility visibility, std::unique_ptr<TypenameAST> return_type, std::string name, std::vector<std::pair<std::unique_ptr<TypenameAST>, std::string>> args)
                : MemberExprAST(visibility), return_type(std::move(return_type)), name(move(name)), args(move(args)) {}

        const std::string &getName() const { return name; }
    };

/// FunctionAST - This class represents a function definition itself.

    class MethodExprAST : public MemberExprAST {
    public:
        std::unique_ptr<PrototypeAST> proto;
        std::unique_ptr<ExprAST> body;
    };

    class FieldExprAST : public MemberExprAST {
    public:
        TypenameAST type;
        std::string name;
        FieldExprAST(Visibility visibility, TypenameAST type, std::string name) : MemberExprAST(visibility), type(std::move(type)), name(move(name)) {}
    };

    class InterfaceExprAST : public TypenameAST {
    public:
        std::string name;
        std::vector<std::unique_ptr<InterfaceExprAST>> implements;
        std::vector<std::unique_ptr<MemberExprAST>> members;
        InterfaceExprAST(std::string name, std::vector<std::unique_ptr<MemberExprAST>> members) : name(move(name)), TypenameAST(move(name)), members(move(members)) {}
        InterfaceExprAST(std::string name, std::vector<std::unique_ptr<MemberExprAST>> members, std::vector<std::unique_ptr<InterfaceExprAST>> implements)
        : name(move(name)), TypenameAST(move(name)), members(move(members)), implements(move(implements)) {}
    };

    class ClassExprAST : public TypenameAST {
    public:
        ClassExprAST* superclass;
        std::vector<std::unique_ptr<InterfaceExprAST>> implements;
        std::vector<std::unique_ptr<MemberExprAST>> members;
        std::vector<std::unique_ptr<MarkerExprAST>> markers;
        ClassExprAST() : TypenameAST("object"), superclass(nullptr) {}
        ClassExprAST(std::string name, std::vector<std::unique_ptr<MemberExprAST>> members, ClassExprAST& superclass) : TypenameAST(move(name)), members(move(members)), superclass(&superclass) {}
        ClassExprAST(std::string name, std::vector<std::unique_ptr<MemberExprAST>> members, ClassExprAST* superclass) : TypenameAST(move(name)), members(move(members)), superclass(superclass) {}
        ClassExprAST(std::string name, std::vector<std::unique_ptr<MemberExprAST>> members, ClassExprAST& superclass, std::vector<std::unique_ptr<InterfaceExprAST>> implements)
        : TypenameAST(move(name), move(members)), superclass(&superclass), implements(move(implements)) {}
    };

    class OutsideImplementsAST : public ExprAST {
    public:
        std::unique_ptr<ClassExprAST> class_;
        std::unique_ptr<InterfaceExprAST> interface_;
        OutsideImplementsAST(std::unique_ptr<ClassExprAST> class_, std::unique_ptr<InterfaceExprAST> interface_) : class_(std::move(class_)), interface_(std::move(interface_)) {}
    };

    class VariableExprAST : public ExprAST {
    public:
        TypenameAST type;
        std::string name;
        VariableExprAST(TypenameAST type, std::string name) : name(move(name)), type(std::move(type)) {}
    };

    class BinaryExprAST : public ExprAST {
    public:
        char op;
        std::unique_ptr<ExprAST> LHS, RHS;

        BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS,
                      std::unique_ptr<ExprAST> RHS)
                : op(op), LHS(move(LHS)), RHS(move(RHS)) {}
    };

    class ConditionExprAST : public ExprAST {
    public:
        std::unique_ptr<ExprAST> condition;
        std::vector<std::unique_ptr<ExprAST>> initial_branch;
        std::vector<std::pair<std::unique_ptr<ExprAST>, std::vector<std::unique_ptr<ExprAST>>>> else_if_branches; // list of pairs (condition, branch)
        std::vector<std::unique_ptr<ExprAST>> else_branch;
    };

/// CallExprAST - Expression class for function calls.
    class CallExprAST : public ExprAST {
    public:
        std::string callee;
        std::vector<ExprAST> args;
        CallExprAST(std::string Callee,
                    std::vector<ExprAST> args)
                : callee(move(Callee)), args(move(args)) {}
    };

    static ClassExprAST* object = new ClassExprAST();
}

