#pragma once
#include "value.h"

struct Environment {
    Environment* parent = nullptr;
    std::unordered_map<std::string, Value> vars;

    explicit Environment(Environment* p=nullptr): parent(p) {}

    bool hasHere(const std::string& k) const { return vars.find(k)!=vars.end(); }

    bool get(const std::string& k, Value& out) const {
        auto it = vars.find(k);
        if (it!=vars.end()){ out = it->second; return true; }
        if (parent) return parent->get(k, out);
        return false;
    }

    void set(const std::string& k, const Value& v){
        // assign existing in chain, else create here
        Environment* cur = this;
        while(cur){
            if (cur->hasHere(k)){ cur->vars[k]=v; return; }
            cur = cur->parent;
        }
        vars[k]=v;
    }

    void declare(const std::string& k, const Value& v){
        vars[k]=v;
    }
};
