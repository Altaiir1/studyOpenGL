#include "App.h"
#include "utils/Debug.h"
#include "stb_image/stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

App::App() : initialized(false), imgui(nullptr)
{
	std::cout << "Initializing App...\n\n";

	if (!initWindow())
		return;
	if (!initOpenGL())
		return;
	if (!initImGui())
		return;
	if (!initShaders())
		return;
	if (!initTextures())
		return;
	if (!initGeometry())
		return;

	initialized = true;
	std::cout << "Initialization complete!\n\n";
}

App::~App()
{
	std::cout << "Terminating App" << '\n';
}

void App::run()
{
	std::cout << "Starting render loop...\n\n";

	if (!shader)
	{
		std::cerr << "Error: Shader is null, cannot render\n";
		return;
	}

	setupShaderUniforms();

	while (!window->shouldClose())
	{
		processInput();

		render();
		renderUI();

		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

		unsigned int transformLoc = glGetUniformLocation(shader->ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		window->swapBuffers();
		window->pollEvents();
	}
}

bool App::initWindow()
{
	std::cout << "Initializing Window..." << '\n';
	window = std::make_unique<Window>();
	return true;
}

bool App::initOpenGL()
{
	std::cout << "Initializing OpenGL..." << '\n';

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << '\n';
		return false;
	}

	// Enable debug output (not available on macOS)
#ifndef __APPLE__
	GLEnableDebugOutput();
#endif

	return true;
}

bool App::initImGui()
{
	std::cout << "Initializing ImGui..." << '\n';
	imgui = std::make_unique<ImGuiHandler>(window->getGLFWWindow());
	return true;
}

bool App::initShaders()
{
	std::cout << "Initializing Shaders..." << '\n';

	try
	{
		shader = std::make_unique<Shader>("shaders/default.vert", "shaders/default.frag");
		std::cout << "Shader loaded successfully\n\n";
		return true;
	}
	catch (const std::exception &e)
	{
		std::cerr << "Failed to load shader: " << e.what() << '\n';
		return false;
	}
}

bool App::initTextures()
{
	std::cout << "Initializing Textures..." << '\n';

	try
	{
		texture1 = std::make_unique<Texture>("resources/textures/awesomeface.png");
		std::cout << "Texture loaded successfully\n\n";
		return true;
	}
	catch (const std::exception &e)
	{
		std::cerr << "Failed to load texture: " << e.what() << '\n';
		return false;
	}
}

bool App::initGeometry()
{
	std::cout << "Initializing Geometry..." << '\n';

	// Vertex data with positions, colors and textures coordinates
	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,	  // top right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f	  // top left
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3	 // second triangle
	};

	// Create and configure VAO
	VAO1 = std::make_unique<VAO>();
	VAO1->Bind();

	// Create buffers
	VBO1 = std::make_unique<VBO>(vertices, sizeof(vertices));
	EBO1 = std::make_unique<EBO>(indices, sizeof(indices));

	// Configure vertex attributes
	VAO1->LinkVBO(*VBO1, 0, 3, 8 * sizeof(float), 0 * sizeof(float));
	VAO1->LinkVBO(*VBO1, 1, 2, 8 * sizeof(float), 6 * sizeof(float));

	// Cleanup
	VBO1->Unbind();
	VAO1->Unbind();

	std::cout << "Geometry setup complete\n";
	return true;
}

void App::setupShaderUniforms()
{
	shader->Activate();
	shader->setInt("texture1", 0);
}

void App::render()
{
	Clear();

	// Bind textures to texture units
	texture1->Activate(GL_TEXTURE0);

	// Draw geometry
	VAO1->Bind();
	GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
}

void App::renderUI()
{
	imgui->beginFrame();
	ui.render(settings);
	imgui->endFrame();
}

bool App::isValid() const
{
	// Returns whether the initialized returns too early (false if any error)
	// or not (true)
	return initialized;
}

void App::Clear()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void App::processInput()
{
	if (glfwGetKey(window->getGLFWWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window->getGLFWWindow(), true);
}
