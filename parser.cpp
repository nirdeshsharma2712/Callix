#include "parser.h"
#include <cctype>
#include <stdexcept>

static bool isIdentStart(char c){ return std::isalpha((unsigned char)c) || c=='_'; }
static bool isIdentChar (char c){ return std::isalnum((unsigned char)c) || c=='_'; }

void Parser::tokenize(){
    toks.clear();
    i=0;
    while(i<src.size()){
        char c = src[i];
        if (std::isspace((unsigned char)c)){ ++i; continue; }
        if (c=='('){ toks.push_back({Token::LParen,"("}); ++i; continue; }
        if (c==')'){ toks.push_back({Token::RParen,")"}); ++i; continue; }
        if (c==','){ toks.push_back({Token::Comma,","}); ++i; continue; }
        if (c==';'){ toks.push_back({Token::Semi,";"}); ++i; continue; }
        if (c=='"'){
            ++i; std::string s; 
            while(i<src.size() && src[i]!='"'){
                if (src[i]=='\\' && i+1<src.size()){
                    char n=src[i+1];
                    if (n=='n'){ s.push_back('\n'); i+=2; continue; }
                    if (n=='"'){ s.push_back('"'); i+=2; continue; }
                    s.push_back(n); i+=2; continue;
                }
                s.push_back(src[i++]);
            }
            if (i>=src.size()||src[i]!='"') throw std::runtime_error("Unterminated string");
            ++i;
            Token t; t.type=Token::String; t.lexeme=s; toks.push_back(t); continue;
        }
        if (std::isdigit((unsigned char)c) || (c=='.' && i+1<src.size() && std::isdigit((unsigned char)src[i+1]))){
            size_t j=i; 
            while(j<src.size() && (std::isdigit((unsigned char)src[j]) || src[j]=='.')) j++;
            double v = std::stod(src.substr(i, j-i));
            Token t; t.type=Token::Number; t.num=v; t.lexeme=src.substr(i,j-i);
            toks.push_back(t); i=j; continue;
        }
        if (isIdentStart(c)){
            size_t j=i+1; while(j<src.size() && isIdentChar(src[j])) j++;
            std::string name = src.substr(i, j-i);
            Token t; t.type=Token::Ident; t.lexeme=name; toks.push_back(t); i=j; continue;
        }
        throw std::runtime_error(std::string("Unexpected char: ")+c);
    }
    toks.push_back({Token::End,""});
}

const Token& Parser::consume(){ return toks[pos++]; }
bool Parser::match(Token::Type t){ if (at().type==t){ ++pos; return true; } return false; }

ASTPtr Parser::parsePrimary(){
    if (at().type==Token::Number){ double v=at().num; consume(); return ASTNode::Number(v); }
    if (at().type==Token::String){ std::string s=at().lexeme; consume(); return ASTNode::String(s); }
    if (at().type==Token::Ident){ 
        std::string name=at().lexeme; consume();
        // could be call
        if (match(Token::LParen)){
            std::vector<ASTPtr> args;
            if (at().type!=Token::RParen){
                do { args.push_back(parseExpression()); } while(match(Token::Comma));
            }
            if (!match(Token::RParen)) throw std::runtime_error("Expected ')'");
            return ASTNode::Call(name, std::move(args));
        }
        return ASTNode::Identifier(name);
    }
    throw std::runtime_error("Expected primary");
}

ASTPtr Parser::parseCallOrPrimary(){ return parsePrimary(); }
ASTPtr Parser::parseExpression(){ return parseCallOrPrimary(); }

ASTPtr Parser::parseStmt(){
    ASTPtr expr = parseExpression();
    if (!match(Token::Semi)) throw std::runtime_error("Missing ';' after statement");
    return expr;
}

std::vector<ASTPtr> Parser::parseProgram(){
    pos=0;
    std::vector<ASTPtr> stmts;
    while(at().type!=Token::End){
        stmts.push_back(parseStmt());
    }
    return stmts;
}
