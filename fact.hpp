#pragma once

#include<string_view>
#include<variant>

struct Fact
{
    template<typename T>
    Fact(std::string_view name, T value);
    Fact(const Fact& fact) : name_(fact.name_), value_(fact.value_) {}
    Fact() : name_(), value_() {}

    std::string name_;
    std::variant<int, float, bool, std::string> value_;
    friend bool operator==(const Fact& lhs, const Fact& rhs);

};

inline bool operator==(const Fact& lhs, const Fact& rhs)
{
    return (lhs.name_ == rhs.name_) && (lhs.value_ == rhs.value_);
}

template<typename T>
inline Fact::Fact(std::string_view name, T value) : name_(name), value_(value)
{
}

