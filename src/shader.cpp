//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-10-26                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#include "shader.h"
#include <fstream>
#include "defines.h"

Shader::Shader()
{

}
Shader::~Shader()
{

}

void Shader::createFromFiles(std::string vertexFileName, std::string fragmentFileName)
{
	std::string vertexString = readFile(vertexFileName);
	std::string fragmentString = readFile(fragmentFileName);
	std::string vertexCode = vertexString.c_str();
	std::string fragmentCode = fragmentString.c_str();

	compileShader(vertexCode, fragmentCode);
}

std::string Shader::readFile(std::string fileName)
{
	std::string content;
	std::ifstream fileStream(("src/shaders/"+fileName).c_str(), std::ios::in);

	if(!fileStream.is_open())
	{
        std::cout << BOLDRED << "[Shader] Failed to read " + fileName + "! File doesn't exist." << RESET << std::endl;
		return "";
	}

	std::string line = "";
	while(!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line+"\n");
	}

	fileStream.close();
	return content;
}

void Shader::compileShader(std::string vertexCode, std::string fragmentCode)
{
	_shaderId = glCreateProgram();

	if(!_shaderId)
	{
		printf("Error creating shader program!");
        std::cout << BOLDRED << "[Shader] Error creating shader program!" << RESET << std::endl;
		return;
	}

	addShader(_shaderId, vertexCode, GL_VERTEX_SHADER);
	addShader(_shaderId, fragmentCode, GL_FRAGMENT_SHADER); 

	GLint result = 0;
	GLchar eLog[1024] = {0};

	glLinkProgram(_shaderId);
	glGetProgramiv(_shaderId, GL_LINK_STATUS, &result);
	if(!result)
	{
		glGetProgramInfoLog(_shaderId, sizeof(eLog), NULL, eLog);
        std::cout << BOLDRED << "[Shader] Error linking program: '" + std::string(eLog)+ "'" << RESET << std::endl;
		return;
	}

	glValidateProgram(_shaderId);
	glGetProgramiv(_shaderId, GL_VALIDATE_STATUS, &result);
	if(!result)
	{
		glGetProgramInfoLog(_shaderId, sizeof(eLog), NULL, eLog);
        std::cout << BOLDRED << "[Shader] Error validating program: '" + std::string(eLog)+ "'" << RESET << std::endl;
		return;
	}

	//---------- Get uniforms ----------//
	_uniformModel = glGetUniformLocation(_shaderId, "model");
}

void Shader::useShader()
{
	glUseProgram(_shaderId);
}

void Shader::addShader(GLuint program, std::string shaderCode, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	const GLchar* code[1];
	code[0] = shaderCode.c_str();

	GLint codeLength[1];
	codeLength[0] = shaderCode.size();

	glShaderSource(shader, 1, code, codeLength);
	glCompileShader(shader);

	GLint result = 0;
	GLchar eLog[1024] = {0};

	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if(!result)
	{
		glGetShaderInfoLog(shader, sizeof(eLog), NULL, eLog);
		//printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
        std::cout << BOLDRED << "[Shader] Error compiling the "<<shaderType<<" shader: '" + std::string(eLog)+ "'" << RESET << std::endl;
		return;
	}

	glAttachShader(program, shader);
}
