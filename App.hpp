#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string>

class App
{
public:
	static auto Create(const std::string& title, int w, int h) -> void;
	static auto Shutdown() -> void;
	static auto StartFrame() -> void;
	static auto EndFrame() -> void;
	static auto IsRunning() -> bool;
private:
	static GLFWwindow* wnd_handle_;
};

