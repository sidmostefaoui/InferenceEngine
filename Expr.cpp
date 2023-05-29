#include "Expr.hpp"
#include "Rule.hpp"

#include <cassert>


Expr Expr::Equals(std::string_view name, Fact::Value value)
{
	Expr e = {};
	e.type_ = Type::EQUALS;
	e.name_ = name;
	e.lhs_ = value;
	e.rhs_ = std::monostate();
	return e;
}

Expr Expr::Not(const Expr& expr)
{
	Expr e = {};
	e.type_ = Type::NOT;
	e.lhs_ = Box<Expr>(new Expr(expr));
	e.rhs_ = std::monostate();
	return e;
}

Expr Expr::And(const Expr& lhs, const Expr& rhs)
{
	Expr e = {};
	e.type_ = Type::AND;
	e.lhs_ = Box<Expr>(new Expr(lhs));
	e.rhs_ = Box<Expr>(new Expr(rhs));
	return e;
}

Expr Expr::Or(const Expr& lhs, const Expr& rhs)
{
	Expr e = {};
	e.type_ = Type::OR;
	e.lhs_ = Box<Expr>(new Expr(lhs));
	e.rhs_ = Box<Expr>(new Expr(rhs));
	return e;
}

auto Expr::Evaluate(std::span<Fact> facts) const -> bool
{
	if (type_ == Type::AND)
	{
		Box<Expr> lhs = *std::get_if<Box<Expr>>(&lhs_);
		Box<Expr> rhs = *std::get_if<Box<Expr>>(&rhs_);
		return lhs->Evaluate(facts) && rhs->Evaluate(facts);
	}

	if (type_ == Type::OR)
	{
		Box<Expr> lhs = *std::get_if<Box<Expr>>(&lhs_);
		Box<Expr> rhs = *std::get_if<Box<Expr>>(&rhs_);
		return lhs->Evaluate(facts) || rhs->Evaluate(facts);
	}

	if (type_ == Type::NOT)
	{
		Box<Expr> val = *std::get_if<Box<Expr>>(&lhs_);
		return !val->Evaluate(facts);
	}

	if (type_ == Type::EQUALS)
	{
		Value val = *std::get_if<Value>(&lhs_);
		
		for (auto& f : facts)
		{
			if (f.name() == name_ && f.value() == val)
				return true;
		}
		return false;
	}

	assert(false);
	return false;
}

Expr Expr::operator!()
{
	return Expr::Not(*this);
}

Expr Expr::operator&(const Expr& other)
{
	return Expr::And(*this, other);
}

Expr Expr::operator|(const Expr& other)
{
	return Expr::Or(*this, other);
}

Rule Expr::operator>>(const Fact& fact)
{
	return Rule(*this, fact);
}

auto operator==(const Expr& lhs, const Expr& rhs) -> bool
{
	return lhs.type_ == rhs.type_ && lhs.name_ == rhs.name_ && lhs.lhs_ == rhs.lhs_ && lhs.rhs_ == rhs.rhs_;
}
