#include "scanner.hpp"
#include <cassert>
#include <iostream>

auto TokenTypeToStr(TokenType t) -> std::string
{
	switch (t)
	{
	case TokenType::IDENTIFIER:
		return "IDENTIFIER";
	case TokenType::NUMBER:
		return "NUMBER";
	case TokenType::STRING:
		return "STRING";
	case TokenType::EQUALS:
		return "=";
	case TokenType::MINUS:
		return "-";
	case TokenType::LEFT_PAREN:
		return "(";
	case TokenType::RIGH_PAREN:
		return ")";
	case TokenType::AND:
		return "and";
	case TokenType::OR:
		return "or";
	default:
	{
		assert(false);
		return "";
	}
	}
}

std::ostream& operator<<(std::ostream& os, const Token& t)
{
	return os << "[" << TokenTypeToStr(t.type) << ", " << t.value << "]";
}

auto IsDigit(char c) -> bool
{
	return c >= '0' && c <= '9';
}

auto IsAlpha(char c) -> bool
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

auto LexicalScanner::ScanFact(const std::string& line) -> std::optional<std::vector<Token>>
{
	line_ = line;
	current_ = 0;
	start_ = 0;
	tokens_.clear();

	while (!EndOfLine())
	{
		start_ = current_;
		char c = Next();

		switch (c)
		{
		case '(': AddToken(TokenType::LEFT_PAREN); break;
		case ')': AddToken(TokenType::RIGH_PAREN); break;
		case '=': AddToken(TokenType::EQUALS); break;
		case '-': AddToken(TokenType::MINUS); break;
		case '"': AddToken(TokenType::STRING, ParseString()); break;
		case ' ':
		case '\t':
		case '\r':
			break;
		default:
		{
			if (IsDigit(c))
			{
				AddToken(TokenType::NUMBER, ParseNumber());
			}
			else if (IsAlpha(c))
			{
				AddToken(TokenType::IDENTIFIER, ParseIdentifier());
			}
			else
			{
				using namespace std::string_literals;
				std::cout << "Lexical Error:\n" + "line:"s + line_ + "\n" + "char: " + c + "\n";
				return std::nullopt;
			}
		}
		break;
		}

	}

	return tokens_;
}


int SubStrCount(const std::string& str, const std::string& substr)
{
	int count = 0;
	size_t pos = 0;
	while ((pos = str.find(substr, pos)) != std::string::npos) {
		count += 1;
		pos += substr.length();
	}
	return count;
}

auto LexicalScanner::ScanRule(const std::string& line) -> std::optional<ScanRuleResult>
{
	if (SubStrCount(line, "->") != 1)
		return std::nullopt;

	std::string delimiter = "->";
	std::string expr_str = line.substr(0, line.find(delimiter));
	std::string fact_str = line.substr(line.find(delimiter) + delimiter.size());

	auto expr_tokens = ScanExpr(expr_str);
	if (!expr_tokens)
		return std::nullopt;

	auto fact_tokens = ScanFact(fact_str);
	if (!fact_tokens)
		return std::nullopt;

	return ScanRuleResult(*expr_tokens, *fact_tokens);
}

auto LexicalScanner::ScanExpr(const std::string& line) -> std::optional<std::vector<Token>>
{
	line_ = line;
	current_ = 0;
	start_ = 0;
	tokens_.clear();

	while (!EndOfLine())
	{
		start_ = current_;
		char c = Next();

		switch (c)
		{
		case '(': AddToken(TokenType::LEFT_PAREN); break;
		case ')': AddToken(TokenType::RIGH_PAREN); break;
		case '=': AddToken(TokenType::EQUALS); break;
		case '-': AddToken(TokenType::MINUS); break;
		case '"': AddToken(TokenType::STRING, ParseString()); break;
		case ' ':
		case '\t':
		case '\r':
			break;
		default:
		{
			if (IsDigit(c))
			{
				AddToken(TokenType::NUMBER, ParseNumber());
			}
			else if (IsAlpha(c))
			{
				std::string s = ParseIdentifier();
				if (s == "and")
					AddToken(TokenType::AND);
				else if (s == "or")
					AddToken(TokenType::OR);
				else if (s == "not")
					AddToken(TokenType::NOT);
				else
				AddToken(TokenType::IDENTIFIER, ParseIdentifier());
			}
			else
			{
				using namespace std::string_literals;
				std::cout << "Lexical Error:\n" + "line:"s + line_ + "\n" + "char: " + c + "\n";
				return std::nullopt;
			}
		}
		break;
		}

	}

	return tokens_;
}

auto LexicalScanner::AddToken(TokenType type, const std::string& value) -> void
{
	tokens_.emplace_back(type, value);
}

auto LexicalScanner::AddToken(TokenType type) -> void
{
	tokens_.emplace_back(type, "");
}

auto LexicalScanner::EndOfLine() -> bool
{
	return current_ >= line_.size();
}

auto LexicalScanner::Next() -> char
{
	return line_[current_++];
}

auto LexicalScanner::LookAhead() -> char
{
	if (EndOfLine())
		return '\0';
	return line_[current_];
}

auto LexicalScanner::LookAhead(int count) -> char
{
	assert(count != 0);
	int i = current_ + count - 1;
	if (i >= line_.size())
		return '\0';
	return line_[i];
}

auto LexicalScanner::ParseString() -> std::string
{
	while (LookAhead() != '"' && !EndOfLine())
	{
		Next();
	}
	Next();

	return line_.substr(start_ + 1, current_ - start_ - 2);
}

auto LexicalScanner::ParseNumber() -> std::string
{
	while (IsDigit(LookAhead()))
		Next();

	if (LookAhead() == '.' && IsDigit(LookAhead(2)))
	{
		Next();
		while (IsDigit(LookAhead()))
			Next();
	}

	return line_.substr(start_, current_ - start_);
}

auto LexicalScanner::ParseIdentifier() -> std::string
{
	while (IsAlpha(LookAhead()))
		Next();

	return line_.substr(start_, current_ - start_);
}

