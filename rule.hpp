#pragma once

#include "Fact.hpp"
#include "Expr.hpp"

class Rule
{
public:
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
