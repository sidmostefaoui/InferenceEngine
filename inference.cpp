#include "inference.hpp"
#include <iostream>


InferenceEngine::InferenceEngine(std::span<Fact> facts, std::span<Rule> rules)
    : fact_base_(), rule_base_()
{
    for (auto& f : facts)
        fact_base_.push_back(f);
    
    for (auto& r : rules)
        rule_base_.push_back(r);
}

bool PremiseInFactBase(const Cond& cond, std::vector<Fact>& fact_base)
{
    for (auto& f : fact_base)
        if (f == cond.fact_)
            return true;
    return false;
}

bool PremiseInFactBase(const NotCond& cond, std::vector<Fact>& fact_base)
{
    for (auto& f : fact_base)
        if (f == cond.fact_)
            return true;
    return false;
}

bool PremiseInFactBase(const Box<NotCond>& cond, std::vector<Fact>& fact_base)
{
    return PremiseInFactBase(*cond, fact_base);
}


bool PremiseInFactBase(const Box<Cond>& cond, std::vector<Fact>& fact_base)
{
    return PremiseInFactBase(*cond, fact_base);
}

bool PremiseInFactBase(const AndCond& cond, std::vector<Fact>& fact_base);
bool PremiseInFactBase(const Box<AndCond>& cond, std::vector<Fact>& fact_base)
{
    return PremiseInFactBase(*cond, fact_base);
}

bool PremiseInFactBase(const AndCond& cond, std::vector<Fact>& fact_base)
{
    bool lhs = std::visit([&](auto&& arg) { return PremiseInFactBase(arg, fact_base); }, cond.lhs_);
    bool rhs = std::visit([&](auto&& arg) { return PremiseInFactBase(arg, fact_base); }, cond.rhs_);
    return lhs && rhs;
}

auto InferenceEngine::GetApplicableRules(std::vector<Rule>& rules) -> std::vector<Rule*>
{
    std::vector<Rule*> applicable_rules;

    for (auto& r : rules)
    {
        bool premise_in_fact_base = std::visit([&](auto&& v) { return PremiseInFactBase(v, fact_base_); }, r.premise_);

        if (premise_in_fact_base)
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
    std::vector<Rule*> applicable_rules = GetApplicableRules(unused_rules);

    while (!applicable_rules.empty())
    {
        Rule r = *applicable_rules[0];

        fact_base_.push_back(r.conclusion_);
        std::erase(unused_rules, r);

        if (r.conclusion_ == fact)
            return true;

        applicable_rules = GetApplicableRules(unused_rules);
    }

    return false;
}
