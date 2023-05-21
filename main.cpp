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


    auto scanner = LexicalScanner();
    auto tokens = scanner.ScanRule("A and B and C = -4 -> D = \"MALADE\"");

    if (tokens)
    {
        std::cout << "scanned successfully. \n";
    }

    auto rule = Parser::ParseRule(*tokens);

    if (rule)
    {
        std::cout << "parsed successfully. \n";
        rules.push_back(*rule);
    }
    

    auto inference_engine = InferenceEngine(facts, rules);
    std::cout << "inference:" << inference_engine.Deduce({ "D", "MALADE" }) << "\n";

    std::cout << "Hello World!\n";
}

