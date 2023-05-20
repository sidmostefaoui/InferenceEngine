#pragma once
#include<variant>
#include"fact.hpp"
#include "indirect_value.h"

struct AndCond;
struct NotCond;
struct Cond;
struct Rule;

template<typename T>
using Box = isocpp_p1950::indirect_value<T>;
using ExprBox = std::variant<Box<Cond>, Box<NotCond>, Box<AndCond>>;
using Expr = std::variant<AndCond, Cond, NotCond>;

struct Cond
{
	template<typename T>
	Cond(std::string_view name, T value) : fact_(name, value) {}
	Cond(const Fact& fact) : fact_(fact) {}

	AndCond operator&&(const Cond& other);
	AndCond operator&&(const NotCond& other);
	AndCond operator&&(const AndCond& other);
	Rule operator>>(const Fact& fact);
	NotCond operator!();
	friend bool operator==(const Cond& lhs, const Cond& rhs);

	Fact fact_;
};

struct NotCond
{
	NotCond(Cond cond) : fact_(cond.fact_) {}

	AndCond operator&&(const Cond& other);
	AndCond operator&&(const NotCond& other);
	AndCond operator&&(const AndCond& other);
	Rule operator>>(const Fact& fact);
	friend bool operator==(const NotCond& lhs, const NotCond& rhs);

	Fact fact_;
};

struct AndCond
{
	AndCond() : lhs_(), rhs_() {}
	AndCond(const Box<Cond>& lhs, const Box<Cond>& rhs) : lhs_(lhs), rhs_(rhs) {}
	AndCond(const Box<Cond>& lhs, const Box<NotCond>& rhs) : lhs_(lhs), rhs_(rhs) {}
	AndCond(const Box<Cond>& lhs, const Box<AndCond>& rhs) : lhs_(lhs), rhs_(rhs) {}

	AndCond(const Box<NotCond>& lhs, const Box<NotCond>& rhs) : lhs_(lhs), rhs_(rhs) {}
	AndCond(const Box<NotCond>& lhs, const Box<Cond>& rhs) : lhs_(lhs), rhs_(rhs) {}
	AndCond(const Box<NotCond>& lhs, const Box<AndCond>& rhs) : lhs_(lhs), rhs_(rhs) {}

	AndCond(const Box<AndCond>& lhs, const Box<AndCond>& rhs) : lhs_(lhs), rhs_(rhs) {}
	AndCond(const Box<AndCond>& lhs, const Box<Cond>& rhs) : lhs_(lhs), rhs_(rhs) {}
	AndCond(const Box<AndCond>& lhs, const Box<NotCond>& rhs) : lhs_(lhs), rhs_(rhs) {}

	AndCond operator&&(const AndCond& other);
	AndCond operator&&(const Cond& other);
	AndCond operator&&(const NotCond& other);
	Rule operator>>(const Fact& fact);
	friend bool operator==(const AndCond& lhs, const AndCond& rhs);

	ExprBox lhs_;
	ExprBox rhs_;
};


struct Rule
{
	Rule(const Rule& rule) : premise_(rule.premise_), conclusion_(rule.conclusion_) {}
	Rule(const Cond& premise, const Fact& conlusion) : premise_(premise), conclusion_(conlusion) {}
	Rule(const NotCond& premise, const Fact& conlusion) : premise_(premise), conclusion_(conlusion) {}
	Rule(const AndCond& premise, const Fact& conlusion) : premise_(premise), conclusion_(conlusion) {}
	Rule(const Expr& premise, const Fact& conclusion) : premise_(premise), conclusion_(conclusion) {}

	Rule operator=(const Rule& rule)
	{
		auto r = Rule(rule);
		return r;
	}

	friend bool operator==(const Rule& lhs, const Rule& rhs);

	Expr premise_;
	Fact conclusion_;
};
