#include "Window.h"

#include <iostream>

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

Window::Window() : window(nullptr)
{
	if (!glfwInit())
	{
		std::cerr << "Failed to init GLFW" << '\n';
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	window = glfwCreateWindow(640, 480, "Hello World!", NULL, NULL);
	if (window == nullptr)
	{
		std::cerr << "Failed to create GLFW window" << '\n';
		glfwTerminate();
		return;
	}

	makeContextCurrent();
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

Window::~Window()
{
	if (window != nullptr)
	{
		glfwDestroyWindow(window);
	}
	glfwTerminate();
}

bool Window::shouldClose() const
{
	return window != nullptr && glfwWindowShouldClose(window);
}

void Window::swapBuffers() const
{
	if (window != nullptr)
	{
		glfwSwapBuffers(window);
	}
}

void Window::pollEvents() const
{
	glfwPollEvents();
}

void Window::makeContextCurrent() const
{
	if (window != nullptr)
	{
		glfwMakeContextCurrent(window);
	}
}
