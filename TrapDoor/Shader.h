#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>
#include <iostream>
#include <fstream>

class Shader
{
public:
	Shader(const std::string& fileName); //Take in the shader code
	virtual ~Shader();
	void Bind(); // Bind shader code and send to the GPU
private:
	static const enum {
		BG_SHADER,
		PLAYER_SHADER,

		NUM_SHADERS
	};
	Shader(const Shader& other){}
	void operator=(const Shader& other){}

	std::string Shader::load_shader(const std::string& fileName);
	void Shader::check_shader_error(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	GLuint Shader::create_shader(const std::string& text, GLenum shaderType);

	GLuint shaderProgram;
	GLuint shaders[NUM_SHADERS];
};

#endif