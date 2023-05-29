#include "Rule.hpp"

Rule::Rule(const Expr& premise, const Fact& conclusion)
	: premise_(premise), conclusion_(conclusion)
{
}

auto Rule::premise() const -> const Expr&
{
	return premise_;
}

auto Rule::conclusion() const -> const Fact&
{
	return conclusion_;
}


auto operator==(const Rule& lhs, const Rule& rhs) -> bool
{
	return lhs.premise_ == rhs.premise_ && lhs.conclusion_ == rhs.conclusion_;
}
