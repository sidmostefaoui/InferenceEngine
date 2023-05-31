#include "Expr.hpp"
#include "Rule.hpp"

#include <cassert>
#include <cctype>


Expr Expr::Equals(std::string_view name, Fact::Value value)
{
	Expr e = {};
	e.type_ = Type::EQUALS;
	e.name_ = name;
	e.lhs_ = value;
	e.rhs_ = std::monostate();
	return e;
}

Expr Expr::LessThan(std::string_view name, Value value)
{
	Expr e = {};
	e.type_ = Type::LESS_THAN;
	e.name_ = name;
	e.lhs_ = value;
	e.rhs_ = std::monostate();
	return e;
}

Expr Expr::LessThanOrEquals(std::string_view name, Value value)
{
	Expr e = {};
	e.type_ = Type::LESS_THAN_OR_EQUALS;
	e.name_ = name;
	e.lhs_ = value;
	e.rhs_ = std::monostate();
	return e;
}

Expr Expr::GreaterThan(std::string_view name, Value value)
{
	Expr e = {};
	e.type_ = Type::GREATER_THAN;
	e.name_ = name;
	e.lhs_ = value;
	e.rhs_ = std::monostate();
	return e;
}

Expr Expr::GreaterThanOrEquals(std::string_view name, Value value)
{
	Expr e = {};
	e.type_ = Type::GREATER_THAN_OR_EQUALS;
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
			if (f.name() == name_ && val == f.value())
				return true;
		}
		return false;
	}

	if (type_ == Type::LESS_THAN)
	{
		Value val = *std::get_if<Value>(&lhs_);

		for (auto& f : facts)
		{
			if (f.name() == name_ && f.value() < val)
				return true;
		}
		return false;
	}

	if (type_ == Type::LESS_THAN_OR_EQUALS)
	{
		Value val = *std::get_if<Value>(&lhs_);

		for (auto& f : facts)
		{
			if (f.name() == name_ && f.value() <= val)
				return true;
		}
		return false;
	}

	if (type_ == Type::GREATER_THAN)
	{
		Value val = *std::get_if<Value>(&lhs_);

		for (auto& f : facts)
		{
			if (f.name() == name_ && f.value() > val)
				return true;
		}
		return false;
	}

	if (type_ == Type::GREATER_THAN_OR_EQUALS)
	{
		Value val = *std::get_if<Value>(&lhs_);

		for (auto& f : facts)
		{
			if (f.name() == name_ && f.value() >= val)
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

// Scanner class

std::string Expr::Scanner::line_;
std::vector<Expr::Scanner::Token> Expr::Scanner::tokens_;
int Expr::Scanner::start_ = 0;
int Expr::Scanner::current_ = 0;

auto Expr::Scanner::Scan(std::string_view line)->std::optional<std::vector<Token>>
{
	line_ = line;
	current_ = 0;
	start_ = 0;
	tokens_.clear();

	while (!EndOfLine())
	{
		start_ = current_;
		char c = Consume();

		switch (c)
		{
		case '(': AddToken(Token::Type::L_PAREN); break;
		case ')': AddToken(Token::Type::R_PAREN); break;
		case '=': AddToken(Token::Type::EQUALS); break;
		case '-': AddToken(Token::Type::MINUS); break;
		case '"': AddToken(Token::Type::STRING, String()); break;
		case ' ':
		case '\t':
		case '\r':
			break;
		default:
		{
			if (c == '<' && Peek(1) == '=')
			{
				Consume();
				AddToken(Token::Type::LESS_THAN_OR_EQUALS);
			}
			else if (c == '<')
			{
				Consume();
				AddToken(Token::Type::LESS_THAN);
			}
			else if (c == '>' && Peek(1) == '=')
			{
				Consume();
				AddToken(Token::Type::GREATER_THAN_OR_EQUALS);
			}
			else if (c == '>')
			{
				Consume();
				AddToken(Token::Type::GREATER_THAN);
			}
			else if (IsDigit(c))
			{
				AddToken(Token::Type::NUMBER, Number());
			}
			else if (IsAlpha(c))
			{
				AddToken(Token::Type::IDENTIFIER, Identifier());
			}
			else
			{
				return std::nullopt;
			}
		}
		break;
		}
	}
	AddToken(Token::Type::EOL);

	return tokens_;
}

auto Expr::Scanner::AddToken(Token::Type type, std::string_view value) -> void
{
	tokens_.emplace_back(type, std::string(value));
}

auto Expr::Scanner::AddToken(Token::Type type) -> void
{
	tokens_.emplace_back(type, "");
}

auto Expr::Scanner::EndOfLine() -> bool
{
	return line_[current_] == '\0';
}

auto Expr::Scanner::Consume() -> char
{
	return line_[current_++];
}

auto Expr::Scanner::Peek(int count) -> char
{
	assert(count > 0);
	int i = current_ - 1 + count;
	if (i >= line_.size())
		return '\0';
	return line_[i];
}

auto Expr::Scanner::String() -> std::string
{
	while (Peek(1) != '"' && !EndOfLine())
	{
		Consume();
	}
	Consume();

	return line_.substr(start_ + 1, current_ - start_ - 2);
}

auto Expr::Scanner::Number() -> std::string
{

	while (IsDigit(Peek(1)))
	{
		Consume();
	}

	if (Peek(1) == '.' && IsDigit(Peek(2)))
	{
		Consume();
		while (IsDigit(Peek(1)))
			Consume();
	}

	return line_.substr(start_, current_ - start_);
}

auto Expr::Scanner::Identifier() -> std::string
{
	while (IsAlpha(Peek(1)))
		Consume();

	return line_.substr(start_, current_ - start_);
}

auto Expr::Scanner::IsDigit(char c) -> bool
{
	return std::isdigit(static_cast<unsigned char>(c));
}

auto Expr::Scanner::IsAlpha(char c) -> bool
{
	return std::isalpha(static_cast<unsigned char>(c));
}