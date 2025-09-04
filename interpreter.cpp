#include "interpreter.h"
#include "builtins.h"
#include <stdexcept>
#include <sstream>
#include <iomanip>

static std::string typeName(const Value& v){
    if (v.isNum()) return "number";
    if (v.isStr()) return "string";
    if (v.isFunc()) return "function";
    return "nil";
}

Value::Number Value::asNum() const {
    if (!isNum()) throw std::runtime_error("Expected number, got "+typeName(*this));
    return std::get<Number>(data);
}
const String& Value::asStr() const {
    if (!isStr()) throw std::runtime_error("Expected string, got "+typeName(*this));
    return std::get<String>(data);
}
FuncPtr Value::asFunc() const {
    if (!isFunc()) throw std::runtime_error("Expected function, got "+typeName(*this));
    return std::get<FuncPtr>(data);
}
bool Value::truthy() const {
    if (isNum()) return asNum()!=0.0;
    if (isStr()) return !asStr().empty();
    if (isFunc()) return true;
    return false;
}

Interpreter::Interpreter(): globals(nullptr) {
    installBuiltins(*this);
}

void Interpreter::registerBuiltin(const std::string& name, std::function<Value(std::vector<Value>&)> fn, int){
    auto F = std::make_shared<Function>();
    F->isBuiltin = true;
    F->builtin = std::move(fn);
    functions[name]=F;
}

Value Interpreter::eval(ASTPtr node, Environment& env){
    switch(node->kind){
        case ASTKind::Number: return Value(node->number);
        case ASTKind::String: return Value(node->text);
        case ASTKind::Identifier: {
            Value v;
            if (!env.get(node->text, v)) throw std::runtime_error("Undefined identifier: "+node->text);
            return v;
        }
        case ASTKind::Call: {
            auto itF = functions.find(node->text);
            if (itF==functions.end()){
                // maybe variable holds a function?
                Value callable;
                if (env.get(node->text, callable) && callable.isFunc()){
                    itF = functions.end();
                    // Temporarily register under a synthetic name
                    auto F = callable.asFunc();
                    std::vector<Value> argVals;
                    argVals.reserve(node->args.size());
                    for (auto& a: node->args) argVals.push_back(eval(a, env));
                    if (F->isBuiltin) return F->builtin(argVals);
                    // user func
                    if (argVals.size()!=F->params.size())
                        throw std::runtime_error("Arity mismatch calling function variable");
                    Environment local(&env);
                    for (size_t i=0;i<F->params.size();++i) local.declare(F->params[i], argVals[i]);
                    return eval(F->body, local);
                }
                // else maybe it’s a user-defined function by name
                auto itUF = functions.find(node->text);
                if (itUF==functions.end()) throw std::runtime_error("Unknown function: "+node->text);
            }
            auto F = functions[node->text];
            std::vector<Value> argVals;
            argVals.reserve(node->args.size());
            for (auto& a: node->args) argVals.push_back(eval(a, env));
            if (F->isBuiltin) return F->builtin(argVals);
            // user-defined by name
            if (argVals.size()!=F->params.size())
                throw std::runtime_error("Arity mismatch for "+node->text);
            Environment local(&env);
            for (size_t i=0;i<F->params.size();++i) local.declare(F->params[i], argVals[i]);
            return eval(F->body, local);
        }
    }
    throw std::runtime_error("Invalid AST node");
}

void Interpreter::run(const std::vector<ASTPtr>& program){
    Environment env(&globals);
    for (auto& stmt: program){
        eval(stmt, env);
    }
}
