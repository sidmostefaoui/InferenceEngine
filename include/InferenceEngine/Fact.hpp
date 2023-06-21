#pragma once

#include <variant>
#include <string_view>
#include <optional>
#include <vector>

class Fact
{
public:
    using Value = std::variant<int, float, bool, std::string>;
	class Scanner;
	class Parser;

    Fact(std::string_view name, Value value);
    Fact(const Fact& fact) = default;
	friend bool operator==(const Fact& lhs, const Fact& rhs);

	auto name() const -> const std::string&;
	auto value() const -> Value;
		
private:
    std::string name_;
    Value value_;
};

class Fact::Scanner
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
			MINUS,
			L_PAREN,
			R_PAREN,
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

class Fact::Parser
{
public:
	using Token = Scanner::Token;
    static auto Parse(const std::vector<Token>& tokens) -> std::optional<Fact>;

private:
    static auto Consume() -> Token;
    static auto Peek(int i) -> Token;
    static auto S() -> std::optional<Fact>;
    static auto A() -> std::optional<Fact::Value>;
    static auto B() -> std::optional<Fact::Value>;
	static auto IsFloat(std::string_view s) -> bool;

private:
    static std::vector<Token> tokens_;
    static int i_;
};


