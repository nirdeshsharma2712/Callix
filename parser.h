#pragma once
#include "ast.h"
#include <string>
#include <vector>

struct Token {
    enum Type { Ident, Number, String, LParen, RParen, Comma, Semi, End } type;
    std::string lexeme;
    double num=0.0;
};

struct Parser {
    std::string src;
    size_t i=0;
    std::vector<Token> toks;

    explicit Parser(std::string s): src(std::move(s)) {}

    void tokenize();
    std::vector<ASTPtr> parseProgram();

private:
    const Token& at(size_t k=0) const { return toks[pos+k]; }
    const Token& consume();
    bool match(Token::Type t);
    ASTPtr parseStmt(); // statement = call ';'
    ASTPtr parseCallOrPrimary();
    ASTPtr parsePrimary();
    ASTPtr parseExpression(); // expression = callOrPrimary
    size_t pos=0;
};
