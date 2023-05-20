#include "Parser.hpp"
#include <cassert>


// fact grammar: S -> id A
//          A -> epsilon / (id) / = B
//          B -> str / num / - num


auto S(const std::vector<Token>& tokens, int i, Fact& fact) -> void;
auto A(const std::vector<Token>& tokens, int i, Fact& fact) -> void;
auto B(const std::vector<Token>& tokens, int i, Fact& fact) -> void;

auto Parser::ParseFact(const std::vector<Token>& tokens) -> std::optional<Fact>
{
    assert(tokens.size() != 0);
    auto fact = Fact();
    S(tokens, 0, fact);

    if (fact.name_ == "ERROR")
        return std::nullopt;
    return fact;
}

auto S(const std::vector<Token>& tokens, int i, Fact& fact) -> void
{
    if (tokens[i].type == TokenType::IDENTIFIER)
    {
        fact.name_ = tokens[i].value;
        A(tokens, i + 1, fact);
        return;
    }

    fact.name_ = "ERROR";
}

auto A(const std::vector<Token>& tokens, int i, Fact& fact) -> void
{
    if (i == tokens.size()) // epsilon
    {
        fact.value_ = true;
        return;
    }

    if ((tokens.size() - i == 3) && tokens[i].type == TokenType::LEFT_PAREN
        && tokens[i+1].type == TokenType::IDENTIFIER && tokens[i+2].type == TokenType::RIGH_PAREN) // (id)
    {

        fact.name_.append("(");
        fact.name_.append(tokens[i + 1].value);
        fact.name_.append(")");
        fact.value_ = true;
        return;
    }

    if (tokens[i].type == TokenType::EQUALS)
    {
        B(tokens, i + 1, fact);
        return;
    }

    fact.name_ = "ERROR";
}

auto IsFloat(const std::string& s) -> bool
{
    return s.find('.') != std::string::npos;
}

auto B(const std::vector<Token>& tokens, int i, Fact& fact) -> void
{
    size_t rest = tokens.size() - i - 1;
    if (rest == 0 && tokens[i].type == TokenType::STRING) // str
    {
        fact.value_ = tokens[i].value;
        return;
    }

    if (rest == 0 && tokens[i].type == TokenType::NUMBER) // num
    {
        if (IsFloat(tokens[i].value))
            fact.value_ = std::stof(tokens[i].value);
        else
            fact.value_ = std::stoi(tokens[i].value);
        return;
    }

    if (rest == 1 && tokens[i].type == TokenType::MINUS && tokens[i + 1].type == TokenType::NUMBER) // - num
    {
        if (IsFloat(tokens[i+1].value))
            fact.value_ = -std::stof(tokens[i+1].value);
        else
            fact.value_ = -std::stoi(tokens[i+1].value);
        return;
    }

    fact.name_ = "ERROR";
}


// rule grammar:

auto Parser::ParseRule(const LexicalScanner::ScanRuleResult& res) -> std::optional<Rule>
{
    auto fact = ParseFact(res.fact_tokens);

    return std::optional<Rule>();
}