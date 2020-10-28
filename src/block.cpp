//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-10-26                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#include "block.h"
#include "window.h"
#include "defines.h"

Block::Block(glm::ivec2 position, glm::vec3 color):
	_position(position), _color(color)
{

}

Block::~Block()
{
}

void Block::draw(GLuint program)
{
	float vertices[] = {
		// Center
		-0.7f,	-0.7f,
		-0.7f,	0.7f,
		0.7f,	-0.7f,
		0.7f,	0.7f,
		// Bottom
		-1.0f,	-1.0f,
		-0.7f,	-0.7f,
		1.0f,	-1.0f,
		0.7f,	-0.7f,
		// Top
		1.0f,	1.0f,
		0.7f,	0.7f,
		-1.0f,	1.0f,
		-0.7f,	0.7f,
		// Right
		1.0f,	1.0f,
		0.7f,	0.7f,
		1.0f,	-1.0f,
		0.7f,	-0.7f,
		// Left
		-1.0f,	-1.0f,
		-0.7f,	-0.7f,
		-1.0f,	1.0f,
		-0.7f,	0.7f,
	};

	//----- Send vertices to GPU -----//
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position location
    GLint locPos = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(locPos);
    glVertexAttribPointer(locPos, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Color location
	GLint locColor = glGetUniformLocation(program, "color");

	// Model location
	glm::mat4 model = calcModel();
	GLint locModel = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(locModel, 1, GL_TRUE, glm::value_ptr(model));

	// Draw center
	glUniform4f(locColor, _color.x,_color.y,_color.z,1);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// Draw bottom
	glUniform4f(locColor, _color.x*0.8,_color.y*0.8,_color.z*0.8,1);
	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

	// Draw top
	glUniform4f(locColor, _color.x*1.2,_color.y*1.2,_color.z*1.2,1);
	glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);

	// Draw right
	glUniform4f(locColor, _color.x*0.8,_color.y*0.8,_color.z*0.8,1);
	glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);

	// Draw left
	glUniform4f(locColor, _color.x*1.2,_color.y*1.2,_color.z*1.2,1);
	glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);

    glDeleteBuffers(1, &_vbo);
}

glm::mat4 Block::calcModel()
{
	float blockWidth = 2.f/(GAME_WIDTH);
	float blockHeight = 2.f/(GAME_HEIGHT);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(_position.x*blockWidth-1+blockWidth/2, _position.y*blockHeight-1+blockHeight/2, 0.0f));
	model = glm::scale(model, glm::vec3(blockWidth/2*0.95, blockHeight/2*0.95, 1.0f));
	model = glm::transpose(model);
	return model;
}

void Block::move(glm::ivec2 vec)
{
	if((_position.x+vec.x)>=0 && (_position.x+vec.x)<GAME_WIDTH)
		_position.x+=vec.x;
	if((_position.y+vec.y)>=0)
		_position.y+=vec.y;
}

void Block::rotate(glm::vec2 point, float angle)
{
	// Create rotation-translation matrix
	glm::mat4 mat = glm::mat4(1.0f);
	mat = glm::translate(mat, glm::vec3(point.x, point.y, 0.0f));
	mat = glm::rotate(mat, glm::radians(angle) ,glm::vec3(0, 0, 1));
	//mat = glm::scale(mat, glm::vec3(3, 0, 1.0f));
	mat = glm::translate(mat, glm::vec3(-point.x, -point.y, 0.0f));

	// Apply rotation to change position
	glm::vec4 transformed = glm::vec4(_position.x, _position.y, 0, 1);
	transformed = mat * transformed;		
	//printf("P(%f, %f) Bef(%d,%d) Af(%f,%f)\n", point.x, point.y, _position.x, _position.y, transformed.x, transformed.y);

	_position.x = round(transformed.x);
	_position.y = round(transformed.y);
}

bool Block::colliding(std::vector<std::vector<bool>> matrix)
{
	if(_position.y==0)
		return true;
	if(matrix[_position.x][_position.y-1])
		return true;
	return false;
}

bool Block::canMove(glm::ivec2 vec, std::vector<std::vector<bool>> matrix)
{
	glm::ivec2 temp = _position;
	_position.x+=vec.x;
	_position.y+=vec.y;

	bool possible = true;
	if(_position.x>=0 && _position.x<GAME_WIDTH && _position.y>=0)
		if(_position.y>=GAME_HEIGHT)
			possible = true;
		else	
			possible = !matrix[_position.x][_position.y];
	else
		possible = false;
	_position = temp;

	return possible;
}

bool Block::canRotate(glm::vec2 point, float angle, std::vector<std::vector<bool>> matrix)
{
	glm::ivec2 temp = _position;
	rotate(point, angle);

	bool possible = true;
	if(_position.x>=0 && _position.x<GAME_WIDTH && _position.y>=0 && _position.y<GAME_HEIGHT)
		possible = !matrix[_position.x][_position.y];
	else
		possible = false;
	_position = temp;

	return possible;
}
