#pragma once

#include <InferenceEngine/InferenceEngine.hpp>

class FactsWindow
{
public:
    FactsWindow(InferenceEngine& ie);
    auto Draw() -> void;

private:
    InferenceEngine& ie_;
    char input_[1024];
};

