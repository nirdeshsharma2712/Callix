#pragma once
#include "value.h"
#include <string>
#include <vector>

enum class ASTKind { Number, String, Identifier, Call };

struct ASTNode {
    ASTKind kind;
    // payloads
    double number = 0.0;
    std::string text;                // for String/Identifier/Call name
    std::vector<ASTPtr> args;        // for Call args

    static ASTPtr Number(double v){
        auto n = std::make_shared<ASTNode>();
        n->kind = ASTKind::Number; n->number = v; return n;
    }
    static ASTPtr String(std::string s){
        auto n = std::make_shared<ASTNode>();
        n->kind = ASTKind::String; n->text = std::move(s); return n;
    }
    static ASTPtr Identifier(std::string s){
        auto n = std::make_shared<ASTNode>();
        n->kind = ASTKind::Identifier; n->text = std::move(s); return n;
    }
    static ASTPtr Call(std::string name, std::vector<ASTPtr> a){
        auto n = std::make_shared<ASTNode>();
        n->kind = ASTKind::Call; n->text = std::move(name); n->args = std::move(a); return n;
    }
};

struct Function {
    // User-defined or builtin
    std::vector<std::string> params;
    ASTPtr body; // for user-defined
    bool isBuiltin = false;
    std::function<Value(std::vector<Value>&)> builtin; // if builtin
};
