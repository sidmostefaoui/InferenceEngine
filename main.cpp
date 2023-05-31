#include <iostream>

#include "Fact.hpp"
#include "InferenceEngine.hpp"
#include "Rule.hpp"
#include "scanner.hpp"
#include "Parser.hpp"

int main()
{

    std::vector<Fact> facts = {
                                {"A", 4.0f},
                                {"B", false},
                                {"C", -4}
                              };

    std::vector<Rule> rules = {
                                (Expr::GreaterThan("A", 3.0f) | Expr::Equals("B", true)) >> Fact("D", true)
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

