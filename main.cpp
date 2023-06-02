#include <iostream>

#include "Fact.hpp"
#include "InferenceEngine.hpp"
#include "Rule.hpp"
#include "scanner.hpp"
#include "Parser.hpp"

int main()
{

    std::vector<Fact> facts = {
                                {"A", 2},
                                {"B", false},
                                {"C", -4}
                              };

    std::vector<Rule> rules = {
                                (Expr::GreaterThan("A", 3.0f) | Expr::Equals("B", true)) >> Fact("D", true)
    };

    auto fact_tokens = Fact::Scanner::Scan("A(B) = -1.0");

    if (fact_tokens)
    {
        std::cout << "scanned fact successfully. \n";
    }

    auto fact = Fact::Parser::Parse(*fact_tokens);

    if (fact)
    {
        std::cout << "parsed fact successfully. \n";
    }

    auto expr_tokens = Expr::Scanner::Scan("(A <= 4) and B = 3");

    if (expr_tokens)
    {
        std::cout << "scanned expression successfully. \n";
    }

    auto expr = Expr::Parser::Parse(*expr_tokens);

    if (expr)
    {
        std::cout << "parsed expression successfully. \n";
    }


    auto inference_engine = InferenceEngine(facts, rules);
    std::cout << "inference:" << (inference_engine.Deduce({ "D", true }) ? "vrai" : "faux") << "\n";

    //std::cout << "Hello World!\n";
}

