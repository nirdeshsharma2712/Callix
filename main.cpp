#include <fstream>
#include <iostream>
#include <sstream>
#include "parser.h"
#include "interpreter.h"

int main(int argc, char** argv){
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string source;
    if (argc>=2){
        std::ifstream in(argv[1]);
        if (!in){ std::cerr<<"Cannot open "<<argv[1]<<"\n"; return 1; }
        std::ostringstream ss; ss<<in.rdbuf(); source = ss.str();
    } else {
        std::ostringstream ss; ss<<std::cin.rdbuf(); source = ss.str();
    }

    try{
        Parser P(source);
        P.tokenize();
        auto program = P.parseProgram();

        Interpreter I;
        I.run(program);
    } catch (const std::exception& ex){
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }
    return 0;
}
