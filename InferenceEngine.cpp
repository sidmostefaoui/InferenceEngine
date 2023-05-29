#include <iostream>
#include "InferenceEngine.hpp"

InferenceEngine::InferenceEngine(std::span<Fact> facts, std::span<Rule> rules)
    : fact_base_(), rule_base_()
{
    for (auto& f : facts)
        fact_base_.push_back(f);

    for (auto& r : rules)
        rule_base_.push_back(r);
}

auto ApplicableRules(std::span<Rule> rules, std::span<Fact> facts) -> std::vector<Rule*>
{
    std::vector<Rule*> applicable_rules;

    for (auto& r : rules)
    {
        if (r.premise().Evaluate(facts))
            applicable_rules.push_back(&r);
    }

    return applicable_rules;
}

bool InferenceEngine::Deduce(const Fact& fact)
{
    for (auto& f : fact_base_)
        if (f == fact)
            return true;

    std::vector<Rule> unused_rules = rule_base_;
    std::vector<Rule*> applicable_rules = ApplicableRules(unused_rules, fact_base_);

    while (!applicable_rules.empty())
    {
        Rule r = *applicable_rules[0];

        fact_base_.push_back(r.conclusion());
        std::erase(unused_rules, r);

        if (r.conclusion() == fact)
            return true;

        applicable_rules = ApplicableRules(unused_rules, fact_base_);
    }

    return false;
}


