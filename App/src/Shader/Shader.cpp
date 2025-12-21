#include <stdexcept>

#include "Shader.h"
#include "utils/Debug.h"

std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return contents;
	}
	throw std::runtime_error("Failed to open file: " + std::string(filename));
}

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// Vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	if (vertexShader == 0) {
		throw std::runtime_error("Failed to create vertex shader");
	}

	GLCall(glShaderSource(vertexShader, 1, &vertexSource, NULL));
	GLCall(glCompileShader(vertexShader));

	// Check for shader compile errors
	int success;
	char infoLog[512];
	GLCall(glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success));
	if (!success)
	{
		GLCall(glGetShaderInfoLog(vertexShader, 512, NULL, infoLog));
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		GLCall(glDeleteShader(vertexShader));
		throw std::runtime_error("Vertex shader compilation failed");
	}

	// Fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	if (fragmentShader == 0) {
		GLCall(glDeleteShader(vertexShader));
		throw std::runtime_error("Failed to create fragment shader");
	}

	GLCall(glShaderSource(fragmentShader, 1, &fragmentSource, NULL));
	GLCall(glCompileShader(fragmentShader));

	// Check for shader compile errors
	GLCall(glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success));
	if (!success)
	{
		GLCall(glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog));
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		GLCall(glDeleteShader(vertexShader));
		GLCall(glDeleteShader(fragmentShader));
		throw std::runtime_error("Fragment shader compilation failed");
	}

	// Link shaders
	ID = glCreateProgram();
	if (ID == 0) {
		GLCall(glDeleteShader(vertexShader));
		GLCall(glDeleteShader(fragmentShader));
		throw std::runtime_error("Failed to create shader program");
	}

	GLCall(glAttachShader(ID, vertexShader));
	GLCall(glAttachShader(ID, fragmentShader));
	GLCall(glLinkProgram(ID));

	// Check for linking errors
	GLCall(glGetProgramiv(ID, GL_LINK_STATUS, &success));
	if (!success)
	{
		GLCall(glGetProgramInfoLog(ID, 512, NULL, infoLog));
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		GLCall(glDeleteProgram(ID));
		ID = 0;
		throw std::runtime_error("Shader program linking failed");
	}

	GLCall(glDeleteShader(vertexShader));
	GLCall(glDeleteShader(fragmentShader));
}

void Shader::Activate() const
{
	if (ID == 0) {
		std::cerr << "Warning: Attempting to activate invalid shader program" << '\n';
		return;
	}
	GLCall(glUseProgram(ID));
}

void Shader::Delete() const
{
	if (ID != 0) {
		GLCall(glDeleteProgram(ID));
	}
}

// Helper functions to send uniforms with specific data type (CPU -> GPU)
void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
