#pragma once
#include <InferenceEngine/InferenceEngine.hpp>

class InferenceWindow
{
public:
	InferenceWindow(InferenceEngine& ie);
	auto Draw() -> void;

private:
	InferenceEngine& ie_;
	char input_[1024];
};

