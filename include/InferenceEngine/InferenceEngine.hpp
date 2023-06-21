#pragma once

#include "fact.hpp"
#include "rule.hpp"
#include <vector>
#include <span>

class InferenceEngine
{
public:
	InferenceEngine() = default;
	InferenceEngine(std::span<Fact> facts, std::span<Rule> rules);
	auto AddFact(const Fact& fact) -> void;
	auto AddRule(const Rule& rule) -> void;
	auto Deduce(const Fact& fact) -> bool;

private:
	std::vector<Fact> fact_base_;
	std::vector<Rule> rule_base_;
};
