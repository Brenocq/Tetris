//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-10-27                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#ifndef MULTI_BLOCK_H
#define MULTI_BLOCK_H
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "block.h"

class MultiBlock{
	public:
		enum MultiBlockType
		{
			IBLOCK=0,
			JBLOCK,
			LBLOCK,
			OBLOCK,
			SBLOCK,
			TBLOCK,
			ZBLOCK
		};

		MultiBlock(MultiBlockType type);
		~MultiBlock();

		void draw(GLuint program);

		void move(glm::ivec2 vec, std::vector<std::vector<bool>> matrix);
		void rotate(bool clockwise, std::vector<std::vector<bool>> matrix);
		bool colliding(std::vector<std::vector<bool>> matrix);

		//---------- Getters and Setters ----------//
		std::vector<Block> getBlocks() const { return _blocks; }

	private:
		MultiBlockType _type;
		glm::vec3 _color;
		std::vector<Block> _blocks;
};

#endif// MULTI_BLOCK_H
