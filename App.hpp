#pragma once
#include <string>
#include <functional>

struct GLFWwindow;

class Window
{
public:
	Window(const std::string& title, int width, int height);
	Window(const Window&) = delete;
	~Window();
	GLFWwindow* handle_;
};

class App
{
public:
	App(const std::string& title, int w, int h);
	~App();
	App(const App&) = delete;
	auto loop(std::function<void()> f) -> void;
private:
	Window window_;
};