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
	class Scanner;
	class Parser;

	Expr(const Expr& other);
	static Expr Equals(std::string_view name, Value value);
	static Expr LessThan(std::string_view name, Value value);
	static Expr LessThanOrEquals(std::string_view name, Value value);
	static Expr GreaterThan(std::string_view name, Value value);
	static Expr GreaterThanOrEquals(std::string_view name, Value value);
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
	LESS_THAN,
	LESS_THAN_OR_EQUALS,
	GREATER_THAN,
	GREATER_THAN_OR_EQUALS,
	NOT,
	AND,
	OR
};

class Expr::Scanner
{
public:
	struct Token
	{
		enum class Type
		{
			IDENTIFIER,
			NUMBER,
			STRING,
			EQUALS,
			LESS_THAN,
			LESS_THAN_OR_EQUALS,
			GREATER_THAN,
			GREATER_THAN_OR_EQUALS,
			MINUS,
			L_PAREN,
			R_PAREN,
			NOT,
			AND,
			OR,
			EOL
		};

		Type type;
		std::string value;
	};

	static auto Scan(std::string_view line) -> std::optional<std::vector<Token>>;

private:
	static auto AddToken(Token::Type type, std::string_view value) -> void;
	static auto AddToken(Token::Type type) -> void;
	static auto EndOfLine() -> bool;
	static auto Consume() -> char;
	static auto Peek(int i) -> char;
	static auto String() -> std::optional<std::string>;
	static auto Number() -> std::string;
	static auto Identifier() -> std::string;
	static auto IsDigit(char c) -> bool;
	static auto IsAlpha(char c) -> bool;

private:
	static std::string line_;
	static std::vector<Token> tokens_;
	static int start_;
	static int current_;
};

class Expr::Parser
{
public:
	using Token = Scanner::Token;
	static auto Parse(const std::vector<Token>& tokens) -> std::optional<Expr>;

private:
	static auto Consume() -> Token;
	static auto Peek(int i) -> Token;
	static auto IsFloat(std::string_view s) -> bool;

	static auto expr() -> std::optional<Expr>;
	static auto expr2(Expr lhs) -> std::optional<Expr>;
	static auto cond() -> std::optional<Expr>;
	static auto comp(Expr lhs) -> std::optional<Expr>;
	static auto val(Expr lhs) -> std::optional<Expr>;


private:
	static std::vector<Token> tokens_;
	static int i_;
};