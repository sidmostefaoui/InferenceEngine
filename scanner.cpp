#include "Scanner.hpp"

std::string Scanner::line_;
std::vector<Fact::Scanner::Token> Fact::Scanner::tokens_;
int Fact::Scanner::start_ = 0;
int Fact::Scanner::current_ = 0;