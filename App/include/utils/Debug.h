#pragma once
#include <glad/glad.h>
#include <iostream>

inline void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

inline bool GLCheckError(const char* function, const char* file, int line) {
	bool hasError = false;
	while (GLenum error = glGetError()) {
		std::cerr << "[OpenGL Error] (" << error << "): "
			<< function << " in " << file << ":" << line << '\n';
		hasError = true;
	}
	return !hasError;
}

#ifdef NDEBUG
#define GLCall(x) x
#else
#define GLCall(x) GLClearError(); x; GLCheckError(#x, __FILE__, __LINE__)
#endif

#ifndef __APPLE__  // OpenGL 4.3+ debug callbacks are not available on macOS (4.1 max)
inline void APIENTRY glDebugOutput(GLenum source,
	GLenum type,
	unsigned int id,
	GLenum severity,
	GLsizei length,
	const char* message,
	const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << '\n';
	std::cout << "Debug message (" << id << "): " << message << '\n';

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << '\n';

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << '\n';

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << '\n';
	std::cout << '\n';
}

// Initialize OpenGL debug output callback
inline void GLEnableDebugOutput()
{
	int flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		// Check if glDebugMessageCallback is available (requires OpenGL 4.3+ or GL_KHR_debug extension)
		if (glDebugMessageCallback != nullptr)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(glDebugOutput, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
			std::cout << "Debug output callback enabled\n\n";
		}
		else
		{
			std::cout << "Debug context available but glDebugMessageCallback not supported\n";
			std::cout << "Requires OpenGL 4.3+ or GL_KHR_debug extension\n";
			std::cout << "Using manual error checking only (GLCall macro)\n\n";
		}
	}
	else {
		std::cout << "Debug context not available, skipping debug output\n";
		std::cout << "Using manual error checking only (GLCall macro)\n\n";
	}
}
#endif