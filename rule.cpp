#include "rule.hpp"


AndCond Cond::operator&&(const Cond& other)
{
	return { Box(new Cond(*this)), Box(new Cond(other)) };
}

AndCond Cond::operator&&(const NotCond& other)
{
	return { Box(new Cond(*this)), Box(new NotCond(other)) };
}

AndCond Cond::operator&&(const AndCond& other)
{
	return { Box(new Cond(*this)), Box(new AndCond(other)) };
}

Rule Cond::operator>>(const Fact& fact)
{
	return {*this, fact};
}

NotCond Cond::operator!()
{
	return NotCond(*this);
}

AndCond NotCond::operator&&(const Cond& other)
{
	return { Box(new NotCond(*this)), Box(new Cond(other)) };
}

AndCond NotCond::operator&&(const NotCond& other)
{
	return { Box(new NotCond(*this)), Box(new NotCond(other)) };
}

AndCond NotCond::operator&&(const AndCond& other)
{
	return { Box(new NotCond(*this)), Box(new AndCond(other)) };
}

Rule NotCond::operator>>(const Fact& fact)
{
	return { *this, fact };
}

AndCond AndCond::operator&&(const AndCond& other)
{
    return { Box(new AndCond(*this)), Box(new AndCond(other)) };

}

AndCond AndCond::operator&&(const Cond& other)
{
    return { Box(new AndCond(*this)), Box(new Cond(other)) };

}

AndCond AndCond::operator&&(const NotCond& other)
{
    return { Box(new AndCond(*this)), Box(new NotCond(other)) };
}

Rule AndCond::operator>>(const Fact& fact)
{
	return { *this, fact };
}

bool operator==(const Cond& lhs, const Cond& rhs)
{
	return lhs.fact_ == rhs.fact_;
}

bool operator==(const NotCond& lhs, const NotCond& rhs)
{
	return lhs.fact_ == rhs.fact_;
}

bool operator==(const AndCond& lhs, const AndCond& rhs)
{
	return (lhs.lhs_ == rhs.lhs_) && (lhs.rhs_ == rhs.rhs_);
}

bool operator==(const Rule& lhs, const Rule& rhs)
{
	return (lhs.premise_ == rhs.premise_) && (lhs.conclusion_ == rhs.conclusion_);
}
