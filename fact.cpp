
#include "fact.hpp"
#include <cassert>

Fact::Fact(std::string_view name, Value value)
    : name_(name), value_(value)
{
}

Fact::Fact(const Fact& fact) : name_(fact.name_), value_(fact.value_)
{
}

bool operator==(const Fact& lhs, const Fact& rhs)
{
    return (lhs.name_ == rhs.name_) && (lhs.value_ == rhs.value_);
}

// Scanner Class

std::string Fact::Scanner::line_;
std::vector<Fact::Scanner::Token> Fact::Scanner::tokens_;
int Fact::Scanner::start_ = 0;
int Fact::Scanner::current_ = 0;

auto Fact::Scanner::Scan(std::string_view line)->std::optional<std::vector<Token>>
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
			if (IsDigit(c))
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

auto Fact::Scanner::AddToken(Token::Type type, std::string_view value) -> void
{
	tokens_.emplace_back(type, value);
}

auto Fact::Scanner::AddToken(Token::Type type) -> void
{
	tokens_.emplace_back(type, "");
}

auto Fact::Scanner::EndOfLine() -> bool
{
	return line_[current_] == '\0';
}

auto Fact::Scanner::Consume() -> char
{
	return line_[current_++];
}

auto Fact::Scanner::Peek(int count) -> char
{
	assert(count > 0);
	int i = current_ - 1 + count;
	if (i >= line_.size())
		return '\0';
	return line_[i];
}

auto Fact::Scanner::String() -> std::string
{
	while (Peek(1) != '"' && !EndOfLine())
	{
		Consume();
	}
	Consume();

	return line_.substr(start_ + 1, current_ - start_ - 2);
}

auto Fact::Scanner::Number() -> std::string
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

auto Fact::Scanner::Identifier() -> std::string
{
	while (IsAlpha(Peek(1)))
		Consume();

	return line_.substr(start_, current_ - start_);
}

auto Fact::Scanner::IsDigit(char c) -> bool
{
	return std::isdigit(static_cast<unsigned char>(c));
}

auto Fact::Scanner::IsAlpha(char c) -> bool
{
	return std::isalpha(static_cast<unsigned char>(c));
}

// Parser Class

std::vector<Fact::Parser::Token> Fact::Parser::tokens_;
int Fact::Parser::i_ = 0;

auto Fact::Parser::Parse(const std::vector<Token>& tokens) -> std::optional<Fact>
{
    tokens_ = tokens;
    i_ = 0;
    assert(tokens_.size() != 0);
    return S();
}

auto Fact::Parser::Consume() -> Token
{
    assert(i_ < tokens_.size());
    return tokens_[i_++];
}

auto Fact::Parser::Peek(int i) -> Token
{
    assert(i > 0 && ((i_ - 1 + i) < tokens_.size()));
    return tokens_[i_ - 1 + i];
}

// fact grammar: S -> id A | id(id) A
//          A -> epsilon | = B
//          B -> str | num | - num
auto Fact::Parser::S() -> std::optional<Fact>
{
    Token t = Consume();

    if (t.type == Token::Type::IDENTIFIER)
    {
        std::string name = t.value;

        if (Peek(1).type == Token::Type::L_PAREN &&
            Peek(2).type == Token::Type::IDENTIFIER &&
            Peek(3).type == Token::Type::R_PAREN)
        {
            Consume();
            name.append("(");
            Token t2 = Consume();
            name.append(t2.value);
            Consume();
            name.append(")");
        }

        auto a = A();
        if (!a) return std::nullopt;

        return Fact(name, *a);
    }

    return std::nullopt;
}

auto Fact::Parser::A() -> std::optional<Fact::Value>
{
    Token t = Consume();

    if (t.type == Token::Type::EOL)
        return true;

    if (t.type == Token::Type::EQUALS)
        return B();

    return std::nullopt;
}

auto Fact::Parser::IsFloat(std::string_view s) -> bool
{
    return s.find('.') != std::string::npos;
}

auto Fact::Parser::B() -> std::optional<Fact::Value>
{
    Token t = Consume();

    if (t.type == Token::Type::STRING && Peek(1).type == Token::Type::EOL)
    {
        return t.value;
    }

    if (t.type == Token::Type::NUMBER && Peek(1).type == Token::Type::EOL)
    {
        if (IsFloat(t.value))
            return std::stof(t.value);
        else
            return std::stoi(t.value);
    }

    if (t.type == Token::Type::MINUS && Peek(1).type == Token::Type::NUMBER && Peek(2).type == Token::Type::EOL)
    {
        Token t2 = Consume();

        if (IsFloat(t2.value))
            return -std::stof(t2.value);
        else
            return -std::stoi(t2.value);
    }

    return std::nullopt;
}
