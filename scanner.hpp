#pragma once

#include <vector>
#include <string>
#include <optional>

//struct Token
//{
//	enum class Type
//	{
//		IDENTIFIER,
//		NUMBER,
//		STRING,
//		EQUALS,
//		MINUS,
//		LEFT_PAREN,
//		RIGH_PAREN,
//		AND,
//		OR,
//		NOT,
//		EOL
//	};
//
//	Type type;
//	std::string value;
//};
//
//auto TokenTypeToStr(Token::Type t) -> std::string;
//
//struct LexicalScanner
//{
//	struct ScanRuleResult
//	{
//		std::vector<Token> expr_tokens;
//		std::vector<Token> fact_tokens;
//	};
//
//	auto ScanFact(const std::string& line) -> std::optional<std::vector<Token>>;
//	auto ScanRule(const std::string& line) -> std::optional<ScanRuleResult>;
//
//private:
//	auto ScanExpr(const std::string& line) -> std::optional<std::vector<Token>>;
//	auto AddToken(Token::Type type, const std::string& value) -> void;
//	auto AddToken(Token::Type type) -> void;
//	auto EndOfLine() -> bool;
//	auto Next() -> char;
//	auto LookAhead() -> char;
//	auto LookAhead(int count) -> char;
//	auto ParseString() -> std::string;
//	auto ParseNumber() -> std::string;
//	auto ParseIdentifier() -> std::string;
//	
//private:
//	std::string line_;
//	std::vector<Token> tokens_;
//	int start_;
//	int current_;
//};