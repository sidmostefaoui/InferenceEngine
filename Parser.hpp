#pragma once

#include"scanner.hpp"
#include "fact.hpp"
#include "rule.hpp"
#include "scanner.hpp"

class Parser
{
public:
	static auto ParseFact(const std::vector<Token>& tokens) -> std::optional<Fact>;
	static auto ParseRule(const LexicalScanner::ScanRuleResult& res) -> std::optional<Rule>;
};

