#include "App.h"
#include "Transform.h"
#include "utils/Debug.h"
#include "UI/Settings.h"
#include "stb_image/stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

// TODO: Refactor App since it is currently too dense in here...

App::App() : imgui(nullptr), initialized(false)
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

	if (!textureShader)
	{
		std::cerr << "Error: Shader is null, cannot render\n";
		return;
	}

	setupShaderUniforms();

	float lastFrameTime = 0.0f;
	float deltaTime = 0.0f;

	Transform transform;
	transform.setPosition(glm::vec3(0.5f, -0.5f, 0.0f));
	transform.setAutoRotate(true);
	transform.setSpeed(glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate on Z

	while (!window->shouldClose())
	{
		auto currentTime = static_cast<float>(glfwGetTime());
		deltaTime = currentTime - lastFrameTime;
		lastFrameTime = currentTime;

		processInput();

		render();
		renderUI();

		transform.update(deltaTime);
		glm::mat4 trans = transform.getMatrix();

		unsigned int transformLoc = glGetUniformLocation(textureShader->ID, "transform");
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
		textureShader = std::make_unique<Shader>("shaders/textured.vert", "shaders/textured.frag");
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
	textureShader->Activate();
	textureShader->setInt("texture1", 0);
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
	glClearColor(settings.windowColor[0], settings.windowColor[1], settings.windowColor[2], settings.windowColor[3]);
	glClear(GL_COLOR_BUFFER_BIT);
}

void App::processInput()
{
	if (glfwGetKey(window->getGLFWWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window->getGLFWWindow(), true);
}
