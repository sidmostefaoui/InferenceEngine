#include <iostream>

#include "Fact.hpp"
#include "InferenceEngine.hpp"
#include "Rule.hpp"
#include "scanner.hpp"
#include "Parser.hpp"

int main()
{

    std::vector<Fact> facts = {
                               // {"A", true},
                                {"B", true},
                                {"C", -4}
                              };

    std::vector<Rule> rules = {
                                !Expr::Equals("A", true) >> Fact("D", true),
                                (!Expr::Equals("A", true) & Expr::Equals("C", -4)) >> Fact("D", false)
    };

    auto tokens = Fact::Scanner::Scan("A(B) = -1.0");

    if (tokens)
    {
        std::cout << "scanned successfully. \n";
    }

    auto fact = Fact::Parser::Parse(*tokens);

    if (fact)
    {
        std::cout << "parsed successfully. \n";
    }
    

    auto inference_engine = InferenceEngine(facts, rules);
    std::cout << "inference:" << (inference_engine.Deduce({ "D", true }) ? "vrai" : "faux") << "\n";

    //std::cout << "Hello World!\n";
}

