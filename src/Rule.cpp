#include <InferenceEngine/Rule.hpp>

Rule::Rule(const Expr& premise, const Fact& conclusion)
	: premise_(premise), conclusion_(conclusion)
{
}

auto Rule::premise() const -> const Expr&
{
	return premise_;
}

auto Rule::conclusion() const -> const Fact&
{
	return conclusion_;
}


auto operator==(const Rule& lhs, const Rule& rhs) -> bool
{
	return lhs.premise_ == rhs.premise_ && lhs.conclusion_ == rhs.conclusion_;
}

auto Rule::Parser::Parse(std::string_view line) -> std::optional<Result>
{
    std::string rule_str = std::string(line);
    std::string target = "->";
    int count = 0;
    size_t pos = 0;

    while ((pos = rule_str.find(target, pos)) != std::string::npos)
    {
        count += 1;
        pos += target.length();
    }

    if (count != 1)
    {
        return std::nullopt;
    }

    std::string expr_str = rule_str.substr(0, rule_str.find(target));
    std::string fact_str = rule_str.substr(rule_str.find(target) + target.length());

    auto fact_tokens = Fact::Scanner::Scan(fact_str);

    if (!fact_tokens)
    {
        return std::nullopt;
    }

    auto fact = Fact::Parser::Parse(*fact_tokens);

    if (!fact)
    {
        return std::nullopt;
    }

    auto expr_tokens = Expr::Scanner::Scan(expr_str);

    if (!expr_tokens)
    {
        return std::nullopt;
    }

    auto expr = Expr::Parser::Parse(*expr_tokens);

    if (!expr)
    {
        return std::nullopt;
    }

    return Result(*expr, *fact);
}
