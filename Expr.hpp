#pragma once

#include "Fact.hpp"

#include <variant>
#include <span>
#include "indirect_value.h"

class Rule;

class Expr
{
public:
	using Value = Fact::Value;

	Expr(const Expr& other) = default;
	static Expr Equals(std::string_view name, Value value);
	static Expr Not(const Expr& expr);
	static Expr And(const Expr& lhs, const Expr& rhs);
	static Expr Or(const Expr& lhs, const Expr& rhs);

	auto Evaluate(std::span<Fact> facts) const -> bool;

	Expr operator!();
	Expr operator&(const Expr& other);
	Expr operator|(const Expr& other);
	Rule operator>>(const Fact& fact);
	friend auto operator==(const Expr& lhs, const Expr& rhs) -> bool;

private:
	enum class Type;
	template<typename T>
	using Box = isocpp_p1950::indirect_value<T>;

	Expr() = default;

	Type type_;
	std::string name_;
	std::variant<Box<Expr>, Value, std::monostate> lhs_;
	std::variant<Box<Expr>, Value, std::monostate> rhs_;
};

enum class Expr::Type
{
	EQUALS,
	NOT,
	AND,
	OR
};
