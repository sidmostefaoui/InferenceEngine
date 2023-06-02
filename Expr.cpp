#include "Expr.hpp"
#include "Rule.hpp"

#include <cassert>
#include <cctype>
#include <string>


Expr::Expr(const Expr& other)
	: type_(other.type_), name_(other.name_), lhs_(other.lhs_), rhs_(other.rhs_)
{
}

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
		case '!': AddToken(Token::Type::NOT); break;
		case '&': AddToken(Token::Type::AND); break;
		case '|': AddToken(Token::Type::OR); break;
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

// Parser Class

std::vector<Expr::Parser::Token> Expr::Parser::tokens_;
int Expr::Parser::i_ = 0;

auto Expr::Parser::Parse(const std::vector<Token>& tokens) -> std::optional<Expr>
{
	tokens_ = tokens;
	i_ = 0;
	assert(tokens_.size() != 0);
	return expr();
}

auto Expr::Parser::Consume() -> Token
{
	assert(i_ < tokens_.size());
	return tokens_[i_++];
}

auto Expr::Parser::Peek(int i) -> Token
{
	assert(i > 0 && ((i_ - 1 + i) < tokens_.size()));
	return tokens_[i_ - 1 + i];
}

auto Expr::Parser::IsFloat(std::string_view s) -> bool
{
	return s.find('.') != std::string::npos;
}

// expr grammar: expr -> not expr | (expr) | expr and expr | expr or expr | cond
// this grammar is left recursive
// equivalent non left recursive grammar: expr -> not expr expr2 | (expr) expr2 | cond expr2
//                                        expr2 -> and expr expr2 | or expr expr2 | epsilon
//										  cond -> id comp | id(id) comp
//										  comp -> epsilon | = val | < val | <= val | > val | >= val
//									      val -> str | num | - num

auto Expr::Parser::expr() -> std::optional<Expr>
{
	Token t = Peek(1);
	
	if (t.type == Token::Type::NOT) // not expr expr2
	{
		Consume();
		auto e = expr();
		if (!e)
			return std::nullopt;
		return expr2(Expr::Not(*e));
	}

	if (t.type == Token::Type::L_PAREN) // (expr) expr2
	{
		Consume();
		auto e = expr();
		if (!e) 
			return std::nullopt;

		if (Peek(1).type == Token::Type::R_PAREN)
		{
			Consume();
			return expr2(*e);
		}

		return std::nullopt;
	}

	// cond expr2
	auto c = cond();
	if (!c)
		return std::nullopt;
	return expr2(*c);
}

auto Expr::Parser::expr2(Expr lhs) -> std::optional<Expr>
{
	Token t = Peek(1);

	if (t.type == Token::Type::EOL || t.type == Token::Type::R_PAREN) // epsilon
		return lhs;

	t = Consume();
	if (t.type == Token::Type::AND) // and expr expr2
	{
		auto e = expr();
		if (!e)
			return std::nullopt;
		return expr2(Expr::And(lhs, *e));
	}

	if (t.type == Token::Type::OR) // and expr expr2
	{
		auto e = expr();
		if (!e)
			return std::nullopt;
		return expr2(Expr::Or(lhs, *e));
	}

	return std::nullopt;
}

auto Expr::Parser::cond() -> std::optional<Expr>
{
	Token t = Consume();

	if (t.type == Token::Type::IDENTIFIER && Peek(1).type == Token::Type::L_PAREN)
	{
		Consume();
		Token t2 = Consume();

		if (t2.type == Token::Type::IDENTIFIER && Peek(1).type == Token::Type::R_PAREN)
		{
			Consume();
			return comp(Expr::Equals(t.value + "(" + t2.value + ")", true));
		}
		
		return std::nullopt;
	}

	if (t.type == Token::Type::IDENTIFIER)
	{
		return comp(Expr::Equals(t.value, true));
	}

	return std::nullopt;
}

auto Expr::Parser::comp(Expr lhs) -> std::optional<Expr>
{
	Token t = Peek(1);
	if (t.type == Token::Type::EOL || t.type == Token::Type::AND || t.type == Token::Type::OR || t.type == Token::Type::R_PAREN) // epsilon
		return lhs;

	t = Consume();

	if (t.type == Token::Type::EQUALS)
		return val(lhs);

	if (t.type == Token::Type::LESS_THAN)
	{
		lhs.type_ = Expr::Type::LESS_THAN;
		return val(lhs);
	}

	if (t.type == Token::Type::LESS_THAN_OR_EQUALS)
	{
		lhs.type_ = Expr::Type::LESS_THAN_OR_EQUALS;
		return val(lhs);
	}

	if (t.type == Token::Type::GREATER_THAN)
	{
		lhs.type_ = Expr::Type::GREATER_THAN;
		return val(lhs);
	}

	if (t.type == Token::Type::GREATER_THAN_OR_EQUALS)
	{
		lhs.type_ = Expr::Type::GREATER_THAN_OR_EQUALS;
		return val(lhs);
	}

	return std::nullopt;
}

auto Expr::Parser::val(Expr lhs) -> std::optional<Expr>
{
	Token t = Consume();

	if (t.type == Token::Type::STRING)
	{
		lhs.lhs_ = t.value;
		return lhs;
	}

	if (t.type == Token::Type::NUMBER)
	{
		if (IsFloat(t.value))
			lhs.lhs_ = std::stof(t.value);
		else
			lhs.lhs_ = std::stoi(t.value);
		return lhs;
	}

	if (t.type == Token::Type::MINUS && Peek(1).type == Token::Type::NUMBER)
	{
		Token t2 = Consume();

		if (IsFloat(t2.value))
			lhs.lhs_ = std::stof(t2.value);
		else
			lhs.lhs_ = std::stoi(t2.value);
		return lhs;
	}

	return std::nullopt;
}