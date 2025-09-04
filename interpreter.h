#pragma once
#include "ast.h"
#include "environment.h"
#include <unordered_map>
#include <iostream>

struct Interpreter {
    Environment globals;
    std::unordered_map<std::string, FuncPtr> functions; // name -> function

    Interpreter();
    Value eval(ASTPtr node, Environment& env);
    void run(const std::vector<ASTPtr>& program);

    // registration
    void registerBuiltin(const std::string& name, std::function<Value(std::vector<Value>&)> fn, int minArity=-1);
};
