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


// expr grammar: expr -> not expr | expr and expr | expr or expr | fact
// this grammar is left recursive
// equivalent non left recursive grammar: expr -> not expr | fact expr2
//                                        expr2 -> and expr expr2 | or expr expr2 | epsilon


auto Aexpr(const std::vector<Token>& tokens, int& i, Fact fact) -> std::optional<Fact>;
auto Sexpr(const std::vector<Token>& tokens, int& i) -> std::optional<Fact>
{
    if (tokens[i].type == TokenType::IDENTIFIER)
    {
        auto fact = Fact();
        fact.name_ = tokens[i].value;
        i += 1;
        return Aexpr(tokens, i, fact);
    }

    return std::nullopt;
}

auto Bexpr(const std::vector<Token>& tokens, int& i, Fact fact) -> std::optional<Fact>;
auto Aexpr(const std::vector<Token>& tokens, int& i, Fact fact) -> std::optional<Fact>
{
    if (i == tokens.size()) // epsilon
    {
        fact.value_ = true;
        return fact;
    }

    /*if ((tokens.size() - i == 3) && tokens[i].type == TokenType::LEFT_PAREN
        && tokens[i + 1].type == TokenType::IDENTIFIER && tokens[i + 2].type == TokenType::RIGH_PAREN) // (id)
    {

        fact.name_.append("(");
        fact.name_.append(tokens[i + 1].value);
        fact.name_.append(")");
        fact.value_ = true;
        i += 2;
        return fact;
    }*/

    if (tokens[i].type == TokenType::EQUALS)
    {
        i += 1;
        return Bexpr(tokens, i, fact);
    }

    fact.value_ = true;
    return fact;
    //return std::nullopt;
}

auto Bexpr(const std::vector<Token>& tokens, int& i, Fact fact) -> std::optional<Fact>
{
    if (tokens[i].type == TokenType::STRING) // str
    {
        fact.value_ = tokens[i].value;
        return fact;
    }

    if (tokens[i].type == TokenType::NUMBER) // num
    {
        if (IsFloat(tokens[i].value))
            fact.value_ = std::stof(tokens[i].value);
        else
            fact.value_ = std::stoi(tokens[i].value);
        return fact;
    }

    if (tokens[i].type == TokenType::MINUS && tokens[i + 1].type == TokenType::NUMBER) // - num
    {
        if (IsFloat(tokens[i + 1].value))
            fact.value_ = -std::stof(tokens[i + 1].value);
        else
            fact.value_ = -std::stoi(tokens[i + 1].value);
        return fact;
    }
    
    return std::nullopt;
}

auto expr2(const std::vector<Token>& tokens, int& i, Expr expr) -> std::optional<Expr>;
auto expr(const std::vector<Token>& tokens, int& i) -> std::optional<Expr>
{
    if (tokens[i].type == TokenType::NOT)
    {
        i += 1;
        auto fact = Sexpr(tokens, i);
        if (!fact)
            return std::nullopt;

        auto left = NotCond(Cond(*fact));
        return expr2(tokens, i, left);

    }

    auto fact = Sexpr(tokens, i);
    if (!fact)
        return std::nullopt;

    auto left = Cond(*fact);
    return expr2(tokens, i, left);
}

auto expr2(const std::vector<Token>& tokens, int& i, Expr prev) -> std::optional<Expr>
{
    if (i == tokens.size()) // epsilon
        return prev;

    if (tokens[i].type == TokenType::AND)
    {
        i += 1;
        auto c = AndCond();

        { // convert lhs to Box
            if (std::holds_alternative<Cond>(prev))
            {
                c.lhs_ = Box(new Cond(*std::get_if<Cond>(&prev)));
            }
            else if (std::holds_alternative<NotCond>(prev))
            {
                c.lhs_ = Box(new NotCond(*std::get_if<NotCond>(&prev)));
            }
            else if (std::holds_alternative<AndCond>(prev))
            {
                c.lhs_ = Box(new AndCond(*std::get_if<AndCond>(&prev)));
            }
        }
      
        auto rhs_opt = expr(tokens, i);

        if (!rhs_opt)
            return std::nullopt;

        Expr rhs = *rhs_opt;

        { // convert rhs to box
            if (std::holds_alternative<Cond>(rhs))
            {
                c.rhs_ = Box(new Cond(*std::get_if<Cond>(&rhs)));
            }
            else if (std::holds_alternative<NotCond>(rhs))
            {
                c.rhs_ = Box(new NotCond(*std::get_if<NotCond>(&rhs)));
            }
            else if (std::holds_alternative<AndCond>(rhs))
            {
                c.rhs_ = Box(new AndCond(*std::get_if<AndCond>(&rhs)));
            }
        }
     
        return expr2(tokens, i, c);
    }

    return std::nullopt;
}


auto ParseExpr(const std::vector<Token>& tokens) -> std::optional<Expr>
{
    assert(tokens.size() != 0);
    int i = 0;
    return expr(tokens, i);
}

auto Parser::ParseRule(const LexicalScanner::ScanRuleResult& res) -> std::optional<Rule>
{
    auto fact = ParseFact(res.fact_tokens);
    if (!fact)
        return std::nullopt;

    auto expr = ParseExpr(res.expr_tokens);
    if (!expr)
        return std::nullopt;

    return Rule(*expr, *fact);
}