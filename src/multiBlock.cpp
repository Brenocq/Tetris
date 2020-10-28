//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-10-27                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#include "multiBlock.h"
#include "defines.h"

MultiBlock::MultiBlock(MultiBlockType type):
	_type(type)
{
	_color = glm::vec3(rand()%100/200.f+0.5f, rand()%100/200.f+0.5f, rand()%100/200.f+0.5f);
	int spawnX = GAME_WIDTH/2;
	int spawnY = GAME_HEIGHT;

	_blocks.clear();
	switch(_type)
	{
		case IBLOCK:
			std::cout<<"I block\n";
			_blocks.push_back(Block({spawnX-1, spawnY}, _color));
			_blocks.push_back(Block({spawnX, spawnY}, _color));
			_blocks.push_back(Block({spawnX+1, spawnY}, _color));
			_blocks.push_back(Block({spawnX+2, spawnY}, _color));
			break;
		case JBLOCK:
			std::cout<<"J block\n";
			_blocks.push_back(Block({spawnX-1, spawnY}, _color));
			_blocks.push_back(Block({spawnX, spawnY}, _color));
			_blocks.push_back(Block({spawnX-1, spawnY+1}, _color));
			_blocks.push_back(Block({spawnX+1, spawnY}, _color));
			break;
		case LBLOCK:
			std::cout<<"L block\n";
			_blocks.push_back(Block({spawnX, spawnY}, _color));
			_blocks.push_back(Block({spawnX-1, spawnY}, _color));
			_blocks.push_back(Block({spawnX+1, spawnY}, _color));
			_blocks.push_back(Block({spawnX+1, spawnY+1}, _color));
			break;
		case OBLOCK:
			std::cout<<"O block\n";
			_blocks.push_back(Block({spawnX, spawnY}, _color));
			_blocks.push_back(Block({spawnX+1, spawnY}, _color));
			_blocks.push_back(Block({spawnX, spawnY+1}, _color));
			_blocks.push_back(Block({spawnX+1, spawnY+1}, _color));
			break;
		case SBLOCK:
			std::cout<<"S block\n";
			_blocks.push_back(Block({spawnX, spawnY}, _color));
			_blocks.push_back(Block({spawnX, spawnY+1}, _color));
			_blocks.push_back(Block({spawnX-1, spawnY}, _color));
			_blocks.push_back(Block({spawnX+1, spawnY+1}, _color));
			break;
		case TBLOCK:
			std::cout<<"T block\n";
			_blocks.push_back(Block({spawnX, spawnY}, _color));
			_blocks.push_back(Block({spawnX, spawnY+1}, _color));
			_blocks.push_back(Block({spawnX-1, spawnY}, _color));
			_blocks.push_back(Block({spawnX+1, spawnY}, _color));
			break;
		case ZBLOCK:
			std::cout<<"Z block\n";
			_blocks.push_back(Block({spawnX, spawnY}, _color));
			_blocks.push_back(Block({spawnX, spawnY+1}, _color));
			_blocks.push_back(Block({spawnX-1, spawnY+1}, _color));
			_blocks.push_back(Block({spawnX+1, spawnY}, _color));
			break;
	}
}

MultiBlock::~MultiBlock()
{

}

void MultiBlock::draw(GLuint program)
{
	for(auto block : _blocks)
	{
		block.draw(program);
	}
}

void MultiBlock::move(glm::ivec2 vec)
{
	// Check if can move
	bool canMove = true;
	for(auto block : _blocks)
	{
		glm::ivec2 pos = block.getPosition();
		if(pos.x+vec.x<0 || pos.x+vec.x>=GAME_WIDTH 
			|| pos.y+vec.y<0)
		{
			canMove=false;
			break;
		}
	}

	// Move blocks
	if(canMove)
		for(auto& block : _blocks)
			block.move(vec);
}

void MultiBlock::rotate(bool clockwise, std::vector<std::vector<bool>> matrix)
{
	std::vector<std::vector<bool>> matrixOtherBlocks = matrix;
	for(auto block : _blocks)
	{
		glm::ivec2 pos = block.getPosition();
		if(pos.y<GAME_HEIGHT)
			matrixOtherBlocks[pos.x][pos.y]=false;
	}

	switch(_type)
	{
		case IBLOCK:
			{
				// Rotate around center
				glm::ivec2 pos1 = _blocks[1].getPosition();
				glm::ivec2 pos2 = _blocks[2].getPosition();
				glm::vec2 point = (pos1+pos2)/2;

				// Check can rotate
				bool canRotate = true;
				for(auto block : _blocks)
					canRotate = canRotate && block.canRotate(point, clockwise?90:-90, matrixOtherBlocks);

				// Rotate
				if(canRotate)
					for(auto& block : _blocks)
						block.rotate(point, clockwise?90:-90);
			}
			break;
		case OBLOCK:
			// Dont rotate
			break;
		case JBLOCK:
		case LBLOCK:
		case SBLOCK:
		case TBLOCK:
		case ZBLOCK:
			{
				// Rotate around first block
				glm::ivec2 point = _blocks[0].getPosition();

				// Check can rotate
				bool canRotate = true;
				for(auto block : _blocks)
					canRotate = canRotate && block.canRotate(point, clockwise?90:-90, matrixOtherBlocks);

				// Rotate
				if(canRotate)
					for(auto& block : _blocks)
						block.rotate(point, clockwise?90:-90);
			}
			break;
	}
}

bool MultiBlock::colliding(std::vector<std::vector<bool>> matrix)
{
	std::vector<Block> lowerBlocks;

	// Find lower blocks
	for(int i=0;i<GAME_WIDTH;i++)
	{
		int lowest = -1;
		int lowestY = GAME_HEIGHT;
		int j=0;
		for(auto block : _blocks)
		{
			glm::ivec2 pos = block.getPosition();
			if(pos.x == i && pos.y<=lowestY)
			{
				lowest = j;
				lowestY = pos.y;
			}
			j++;
		}
		if(lowest!=-1)
			lowerBlocks.push_back(_blocks[lowest]);
	}

	// Check collisions
	for(auto block : lowerBlocks)
	{
		if(block.colliding(matrix))
			return true;
	}
	return false;
}
