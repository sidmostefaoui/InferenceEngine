#pragma once

#include "Fact.hpp"
#include "Expr.hpp"

class Rule
{
public:
	class Parser;

	Rule(const Expr& premise, const Fact& conclusion);
	Rule(const Rule& other) = default;

	auto operator=(const Rule& other) -> Rule& = default;
	friend auto operator==(const Rule& lhs, const Rule& rhs) -> bool;

	auto premise() const -> const Expr&;
	auto conclusion() const -> const Fact&;

private:
	Expr premise_;
	Fact conclusion_;
};

class Rule::Parser
{
public:
	struct Result
	{
		Expr expr;
		Fact fact;
	};

	static auto Parse(std::string_view line) -> std::optional<Result>;
};
