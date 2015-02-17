#include "Shader.h"


Shader::Shader(const std::string& fileName)
{
	shaderProgram = glCreateProgram();
	shaders[0] = create_shader(load_shader(fileName + ".vs"), GL_VERTEX_SHADER);
	shaders[1] = create_shader(load_shader(fileName + ".fs"), GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < NUM_SHADERS; ++i){
		glAttachShader(shaderProgram, shaders[i]);
	}

	glBindAttribLocation(shaderProgram, 0, "position");
	glBindAttribLocation(shaderProgram, 1, "texcoord");
	glBindAttribLocation(shaderProgram, 2, "color");

	glLinkProgram(shaderProgram);
	check_shader_error(shaderProgram, GL_LINK_STATUS, true, "Error: Program link failed.");

	glValidateProgram(shaderProgram);
	check_shader_error(shaderProgram, GL_VALIDATE_STATUS, true, "Error: Program invalid.");
}



Shader::~Shader()
{
	for (unsigned int i = 0; i < NUM_SHADERS; ++i){
		glDetachShader(shaderProgram, shaders[i]);
		glDeleteShader(shaders[i]);
	}

	glDeleteProgram(shaderProgram);
}

GLuint Shader::create_shader(const std::string& text, GLenum shaderType){
	GLuint shader = glCreateShader(shaderType);

	if (shader == 0)
		std::cerr << "Error: Shader creation failed." << std::endl;

	const GLchar* shaderSourceStrings[1];
	GLint shaderSourceStringLengths[1];

	shaderSourceStrings[0] = text.c_str();
	shaderSourceStringLengths[0] = text.length();

	glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLengths);
	glCompileShader(shader);

	check_shader_error(shader, GL_COMPILE_STATUS, false, "Error: Shader compile failed");

	return shader;
}

void Shader::Bind(){
	glUseProgram(shaderProgram);
}

std::string Shader::load_shader(const std::string& fileName)
{
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	return output;
}

void Shader::check_shader_error(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}