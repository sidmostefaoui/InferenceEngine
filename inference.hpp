#pragma once

#include "fact.hpp"
#include "rule.hpp"
#include <vector>
#include <span>
#include <unordered_set>

struct InferenceEngine
{
    InferenceEngine(std::span<Fact> facts, std::span<Rule> rules);

    auto GetApplicableRules(std::vector<Rule>& rules) -> std::vector<Rule*>;
    bool Deduce(const Fact& fact);

private:
    std::vector<Fact> fact_base_;
    std::vector<Rule> rule_base_;
};
