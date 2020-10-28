//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-10-26                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#ifndef SHADER_H
#define SHADER_H
#include <iostream>
#include <string>
#include <GL/glew.h>

class Shader{
	public:
		Shader();
		~Shader();

		void createFromFiles(std::string vertexFileName, std::string fragmentFileName);
		void useShader();
		GLuint getShaderId() const { return _shaderId; }

		//---------- Getters and Setters ----------//
		GLuint getModelLocation() { return _uniformModel; }

	private:
		std::string readFile(std::string fileName);
		void compileShader(std::string vertexCode, std::string fragmentCode);
		void addShader(GLuint program, std::string shaderCode, GLenum shaderType);

		GLuint _shaderId;
		GLuint _uniformModel;
};

#endif// SHADER_H
