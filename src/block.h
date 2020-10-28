//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-10-26                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#ifndef BLOCK_H
#define BLOCK_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <vector>

class Block{
	public:
		Block(glm::ivec2 position, glm::vec3 color);
		~Block();

		void draw(GLuint program);
		void move(glm::ivec2 vec);
		void rotate(glm::vec2 point, float angle);
		bool colliding(std::vector<std::vector<bool>> matrix);
		bool canRotate(glm::vec2 point, float angle, std::vector<std::vector<bool>> matrix);

		//---------- Getters and Setters ----------//
		glm::ivec2 getPosition() const { return _position; };
		void setPosition(glm::ivec2 position) { _position = position; };

	private:
		glm::mat4 calcModel();

		glm::ivec2 _position;
		glm::vec3 _color;

		GLuint _vbo;// Vertex buffer object
};

#endif// BLOCK_H
