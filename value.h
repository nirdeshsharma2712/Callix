#pragma once
#include <variant>
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <functional>

struct ASTNode; // forward

struct Function;

using Number   = double;
using String   = std::string;
using FuncPtr  = std::shared_ptr<Function>;
using ASTPtr   = std::shared_ptr<ASTNode>;

struct Value {
    std::variant<std::monostate, Number, String, FuncPtr> data;

    Value() : data(std::monostate{}) {}
    Value(Number n) : data(n) {}
    Value(const String& s) : data(s) {}
    Value(const char* s) : data(String(s)) {}
    Value(FuncPtr f) : data(std::move(f)) {}

    bool isNil() const { return std::holds_alternative<std::monostate>(data); }
    bool isNum() const { return std::holds_alternative<Number>(data); }
    bool isStr() const { return std::holds_alternative<String>(data); }
    bool isFunc()const { return std::holds_alternative<FuncPtr>(data); }

    Number asNum() const;
    const String& asStr() const;
    FuncPtr asFunc() const;


    bool truthy() const;
};

inline Value num(double x){ return Value(x); }
inline Value str(const std::string& s){ return Value(s); }
