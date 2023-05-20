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
                                {"C", "non"}
                              };

    std::vector<Rule> rules = {
                                Cond("A", true) >> Fact("D", "MALADE")
                              };


    auto scanner = LexicalScanner();
    auto tokens = scanner.ScanRule("A or (B and C) -> D = \"Malade\"");
    
    if (tokens)
    {
        std::cout << "scanned successfully. \n";
    }

    auto inference_engine = InferenceEngine(facts, rules);
    std::cout << "inference:" << inference_engine.Deduce({ "D", "MALADE" }) << "\n";

    std::cout << "Hello World!\n";
}

