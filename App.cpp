#include "App.hpp"
#include <cassert>

#include <glad/glad.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <iostream>

#include <stdexcept>
#include <chrono>

void glfw_error_callback(int error, const char* description)
{
	std::cout << "GLFW ERROR " << error << ": " << description << "\n";
	assert(false);
}

void GLAPIENTRY gl_message_callback(GLenum, GLenum type, GLuint, GLenum severity,
									GLsizei, const GLchar* message, const void*)
{
	printf("GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
	assert(false);
}

GLFWwindow* App::wnd_handle_ = nullptr;

void App::Create(const std::string& title, int w, int h)
{
	assert(w > 0);
	assert(h > 0);

	/* Initialize GLFW */
	if (!glfwInit())
	{
		throw std::runtime_error("Failed to initialize GLFW");
	}

	glfwSetErrorCallback(glfw_error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	wnd_handle_ = glfwCreateWindow(w, h, title.c_str(), NULL, NULL);

	if (!wnd_handle_)
	{
		throw std::runtime_error("Failed to create window");
	}

	glfwMakeContextCurrent(wnd_handle_);
	glfwSwapInterval(0); // V-Sync

	/* initialize OpenGL */
	gladLoadGL();
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(gl_message_callback, 0);

	/* initialize ImGui */
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(wnd_handle_, true);
	ImGui_ImplOpenGL3_Init("#version 130");
}

auto App::Shutdown() -> void
{
	assert(wnd_handle_);
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(wnd_handle_);
	glfwTerminate();
}


auto App::StartFrame() -> void
{
	glfwPollEvents();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

auto App::EndFrame() -> void
{
	ImGui::Render();

	int display_w = 0;
	int display_h = 0;
	glfwGetFramebufferSize(wnd_handle_, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);

	static auto clear_color = ImVec4(51/255.0f, 51/255.0f, 51/255.0f, 0.5f);
	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
		clear_color.z * clear_color.w, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(wnd_handle_);
}

auto App::IsRunning() -> bool
{
	assert(wnd_handle_);
	return !glfwWindowShouldClose(wnd_handle_);
}