#include <GLAD/glad.h>
#include "Graphics_OGL.h"
#include <iostream>
#include <fstream>
#include <sstream>

uint32_t Graphics_OGL::Shader_OGL::s_activeID;

Graphics_OGL::Shader_OGL::Shader_OGL(const char* compute) : ID(glCreateProgram())
{
	int computeShader = createShader(compute, GL_COMPUTE_SHADER);
	glAttachShader(ID, computeShader);

	linkProgram(ID);

	glDeleteShader(computeShader);
}

Graphics_OGL::Shader_OGL::Shader_OGL(const char* vertex, const char* fragment) : ID(glCreateProgram())
{
	int vertexShader = createShader(vertex, GL_VERTEX_SHADER);
	int fragmentShader = createShader(fragment, GL_FRAGMENT_SHADER);

	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);

	linkProgram(ID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Graphics_OGL::Shader_OGL::Shader_OGL(const char* vertex, const char* geometry, const char* fragment) : ID(glCreateProgram())
{
	int vertexShader = createShader(vertex, GL_VERTEX_SHADER);
	int geometryShader = createShader(geometry, GL_GEOMETRY_SHADER);
	int fragmentShader = createShader(fragment, GL_FRAGMENT_SHADER);

	glAttachShader(ID, vertexShader);
	glAttachShader(ID, geometryShader);
	glAttachShader(ID, fragmentShader);

	linkProgram(ID);

	glDeleteShader(vertexShader);
	glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);
}

void Graphics_OGL::Shader_OGL::use() const
{
	if (s_activeID != ID)
	{
		glUseProgram(ID);
		s_activeID = ID;
	}
}

void Graphics_OGL::Shader_OGL::Delete() const
{
	if (ID)
		glDeleteProgram(ID);
	s_activeID = 0;
}

void Graphics_OGL::Shader_OGL::bindUniformBlock(uint32_t bufferIndex, const char* const blockName)
{
	unsigned int uniform_index = glGetUniformBlockIndex(ID, blockName);
	glUniformBlockBinding(ID, uniform_index, bufferIndex);
}

void Graphics_OGL::Shader_OGL::bindStorageBlock(uint32_t bufferIndex, const char* const blockName)
{
	unsigned int uniform_index = glGetProgramResourceIndex(ID, GL_SHADER_STORAGE_BLOCK, blockName);
	glShaderStorageBlockBinding(ID, uniform_index, bufferIndex);
}

void Graphics_OGL::Shader_OGL::setInt(const std::string& name, int value)
{
	auto loc = glGetUniformLocation(s_activeID, name.c_str());
	glUniform1i(loc, value);
}

void Graphics_OGL::Shader_OGL::linkProgram(uint32_t id)
{
	int success;
	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512] = { 0 };
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		// Stuff
		std::cout << "ERROR::SHADER::PROGRAM::LINKAGE_FAILED\n" << infoLog << std::endl;
	}
}

int Graphics_OGL::Shader_OGL::createShader(const char* shaderPath, GLenum shaderType)
{
	unsigned int shader = glCreateShader(shaderType);
#ifndef _DEBUG
	glShaderSource(shader, 1, &shaderPath, NULL);
#else
	std::string shaderText;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		shaderFile.open(shaderPath);
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		shaderText = shaderStream.str();
		shaderText = shaderText.substr(3, shaderText.length() - 6);
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* shaderCode = shaderText.c_str();
	glShaderSource(shader, 1, &shaderCode, NULL);
#endif // !_DEBUG
	glCompileShader(shader);
	

	int success;
	char infoLog[512] = { 0 };
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		switch (shaderType)
		{
		case GL_VERTEX_SHADER:
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			break;
		case GL_GEOMETRY_SHADER:
			std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
			break;
		case GL_FRAGMENT_SHADER:
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
			break;
		default:
			std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
			break;
		}
	}
	return shader;
}

