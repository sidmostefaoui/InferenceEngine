#include <iostream>

#include "fact.hpp"
#include "inference.hpp"
#include "rule.hpp"
#include "scanner.hpp"
#include "Parser.hpp"

int main()
{

    std::vector<Fact> facts = {
                                {"A", true},
                                {"B", true},
                                {"C", -4}
                              };

    std::vector<Rule> rules = {
                                //(Cond("A", true) && Cond("B", true) && Cond("C", 4)) >> Fact("D", "MALADE")
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
    std::cout << "inference:" << inference_engine.Deduce({ "D", "MALADE" }) << "\n";

    std::cout << "Hello World!\n";
}

