#include "App.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <stdexcept>
#include <cassert>

void gl_message_callback(GLenum, GLenum type, GLuint, GLenum severity,
	GLsizei, const GLchar* message, const void*)
{
	std::printf("GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
	assert(false);
}

App::App(const std::string& title, int width, int height)
	: window_(title, width, height)
{
	/* initialize OpenGL */
	gladLoadGL();
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(gl_message_callback, 0);

	/* initialize ImGui */
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window_.handle_, true);
	ImGui_ImplOpenGL3_Init("#version 130");
}

App::~App()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

auto App::loop(std::function<void()> f) -> void
{
	while (!glfwWindowShouldClose(window_.handle_))
	{
		/* start frame */
		glfwPollEvents();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		f();
		
		/* end frame */
		ImGui::Render();
		int display_w = 0;
		int display_h = 0;
		glfwGetFramebufferSize(window_.handle_, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);

		static auto clear_color = ImVec4(51 / 255.0f, 51 / 255.0f, 51 / 255.0f, 0.5f);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
			clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window_.handle_);
	}
}


void glfw_error_callback(int error, const char* description)
{
	std::printf("GLFW ERROR %i: %s\n", error, description);
	assert(false);
}

Window::Window(const std::string& title, int width, int height)
{
	assert(width > 0);
	assert(height > 0);

	/* Initialize GLFW */
	if (!glfwInit())
		throw std::runtime_error("Failed to create Window.");

	glfwSetErrorCallback(glfw_error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	handle_ = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (!handle_)
		throw std::runtime_error("Failed to create Window.");

	glfwMakeContextCurrent(handle_);
	glfwSwapInterval(0); // V-Sync
}

Window::~Window()
{
	glfwDestroyWindow(handle_);
	glfwTerminate();
}
