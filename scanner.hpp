#pragma once

#include <vector>
#include <optional>
#include <string_view>

template<typename Token>
class Scanner
{
public:
	
	static auto Scan(std::string_view line) -> std::optional<std::vector<Token>> = 0;

private:
	static auto AddToken(Token::Type type, std::string_view value) -> void;
	static auto AddToken(Token::Type type) -> void;
	static auto EndOfLine() -> bool;
	static auto Consume() -> char;
	static auto Peek(int i) -> char;
	static auto String() -> std::string;
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
