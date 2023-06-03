#pragma once
#include "InferenceEngine.hpp"
#include <vector>
#include <string>

class RulesWindow
{
public:
    RulesWindow(InferenceEngine& ie, const std::vector<std::string>& rules);
    auto Draw() -> void;

private:
    InferenceEngine& ie_;
    char input_[1024];
    std::vector<std::string> rules_;
};
