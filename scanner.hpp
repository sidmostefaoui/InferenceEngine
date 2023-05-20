#pragma once

#include <vector>
#include <string>
#include <optional>

enum class TokenType
{
	IDENTIFIER,
	NUMBER,
	STRING,
	EQUALS,
	MINUS,
	LEFT_PAREN,
	RIGH_PAREN,
	AND,
	OR,
	NOT
};

auto TokenTypeToStr(TokenType t) -> std::string;

struct Token
{
	TokenType type;
	std::string value;
	friend std::ostream& operator<<(std::ostream& os, const Token& tc);
};

struct LexicalScanner
{
	struct ScanRuleResult
	{
		std::vector<Token> expr_tokens;
		std::vector<Token> fact_tokens;
	};

	auto ScanFact(const std::string& line) -> std::optional<std::vector<Token>>;
	auto ScanRule(const std::string& line) -> std::optional<ScanRuleResult>;

private:
	auto ScanExpr(const std::string& line) -> std::optional<std::vector<Token>>;
	auto AddToken(TokenType type, const std::string& value) -> void;
	auto AddToken(TokenType type) -> void;
	auto EndOfLine() -> bool;
	auto Next() -> char;
	auto LookAhead() -> char;
	auto LookAhead(int count) -> char;
	auto ParseString() -> std::string;
	auto ParseNumber() -> std::string;
	auto ParseIdentifier() -> std::string;
	
private:
	std::string line_;
	std::vector<Token> tokens_;
	int start_;
	int current_;
};