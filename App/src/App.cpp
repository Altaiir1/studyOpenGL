#include <glad/glad.h>
#include <iostream>

#include "imgui.h"
#include "App.h"

#include <cmath>

#include "UI/UI.h"
#include "Shader.h"
#include "BufferObjs/VBO.h"
#include "BufferObjs/VAO.h"
#include "BufferObjs/EBO.h"
#include "Window.h"
#include "utils/Debug.h"
#include "stb_image/stb_image.h"

App::App() : initialized(false), imgui(nullptr)
{
	std::cout << "Constructing App\n\n";

	window = std::make_unique<Window>();

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << '\n';
		return;
	}

	// Enable Debug Utility (not available on macOS - OpenGL 4.1 max)
#ifndef __APPLE__
	GLEnableDebugOutput();
#endif

	imgui = std::make_unique<ImGuiHandler>(window->getGLFWWindow());

	try {
		std::cout << "Loading Shader...\n";
		shader = std::make_unique<Shader>("shaders/default.vert", "shaders/default.frag");
		std::cout << "Shader loaded successfully\n\n";
	}
	catch (const std::exception& e) {
		std::cerr << "Failed to load shader: " << e.what() << '\n';
		return;
	}

	// TEXTURES
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("resources/textures/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture: " << stbi_failure_reason() << '\n';
	}

	stbi_image_free(data);

	// Vertices
	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
   };

	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	std::cout << "Setting up VAO, VBO, EBO..." << '\n';

	VAO1 = std::make_unique<VAO>();
	VAO1->Bind();

	VBO1 = std::make_unique<VBO>(vertices, sizeof(vertices));
	EBO1 = std::make_unique<EBO>(indices, sizeof(indices));

	// position attribute (layout = 0)
	VAO1->LinkVBO(*VBO1, 0, 3, 8 * sizeof(float), 0);
	// color attribute (layout = 1)
	VAO1->LinkVBO(*VBO1, 1, 3, 8 * sizeof(float), 3 * sizeof(float));
	// Texture attribute (layout = 2)
	VAO1->LinkVBO(*VBO1, 2, 2, 8 * sizeof(float), 6 * sizeof(float));

	VBO1->Unbind();
	VAO1->Unbind();

	std::cout << "VAO, VBO, EBO setup complete\n\n";

	// Returns true in the isValid() function so everything went fine
	initialized = true;
}

App::~App()
{
	std::cout << "Terminating App" << '\n';
}

void App::run()
{
	std::cout << "Starting render loop..." << '\n';

	if (!shader) {
		std::cerr << "Error: Shader is null, cannot render" << '\n';
		return;
	}

	shader->Activate();
	GLCall(glUniform1i(glGetUniformLocation(shader->ID, "texture1"), 0));
	shader->setInt("texture1", 0);

	std::cout << "All checks passed, entering render loop\n" << '\n';

	while (!window->shouldClose())
	{
		processInput();
		Clear();

		//GLuint colorLocation = glGetUniformLocation(shader->ID, "triangleColor");
		// GLCall(glUniform3f(colorLocation, settings.triangleColor[0], settings.triangleColor[1], settings.triangleColor[2]));

		VAO1->Bind();
		// GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO1->ID);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		imgui->beginFrame();

		ui.render(settings);
		// ImGui::ShowDemoWindow();

		imgui->endFrame();

		window->swapBuffers();
		window->pollEvents();
	}
}

// Returns whether the initialized returns too early (false if any error)
// or not (true)
bool App::isValid() const
{
	return initialized;
}

void App::Clear() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

// Here we don't pass window as a parameter since window pointer is a member
// therefore it's part of the Object Data
void App::processInput()
{
	if (glfwGetKey(window->getGLFWWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window->getGLFWWindow(), true);
}
