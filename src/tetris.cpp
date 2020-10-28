//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-10-26                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#include "tetris.h"
#include <iostream>
#include "defines.h"

Tetris::Tetris():
	_multiBlock(nullptr), _gameOver(false), _score(0)
{
	_window = new Window("Tetris - by Brenocq", MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT);
	_window->onKey = [this](const int key, const int scancode, const int action, const int mods) { onKey(key, scancode, action, mods); };
	_window->onDraw = [this]() { onDraw(); };
	_window->init();

	_updateSpeedMS = std::chrono::milliseconds(GAME_STEP_MS);

	createShaders();
}

Tetris::~Tetris()
{

}

void Tetris::run()
{
	printHelp();
	_window->loop();
}

void Tetris::printHelp()
{
std::cout << BOLDGREEN << "┌──────────────────── TETRIS ────────────────────┐\n" <<
			 BOLDGREEN << "│ - " << YELLOW << "AD " << WHITE << "or" << YELLOW<< " <Right-Arrow><Left-Arrow>" << WHITE << " to translate"<<BOLDGREEN<<" │\n" <<
			 BOLDGREEN << "│ - " << YELLOW << "SW " << WHITE << "or" << YELLOW<< " <Down-Arrow><Up-Arrow>" << WHITE << " to rotate "<<BOLDGREEN<<"      │\n" <<
			 BOLDGREEN << "│ - " << YELLOW << "<Space>" << WHITE << " to push down"<<BOLDGREEN<<"                         │\n" <<
			 BOLDGREEN << "│ - " << RED << "<Esq>" << WHITE << " to close"<<BOLDGREEN<<"                               │\n" <<
			 BOLDGREEN << "└────────────────────────────────────────────────┘\n" << RESET;
}

void Tetris::createShaders()
{
	Shader* shader = new Shader();
	shader->createFromFiles("main.vert.glsl", "main.frag.glsl");
	shader->useShader();
	_shaders.push_back(shader);
}

void Tetris::onKey(int key, int scancode, int action, int mods)
{
	if(key==GLFW_KEY_SPACE && action==GLFW_RELEASE)
		_updateSpeedMS = std::chrono::milliseconds(GAME_STEP_MS);

	if(action == GLFW_RELEASE)
		return;
	switch(key)
	{
		case GLFW_KEY_W:
			if(_multiBlock!=nullptr)
				_multiBlock->rotate(false, getBlocksMatrix());
			break;
		case GLFW_KEY_A:
			if(_multiBlock!=nullptr)
				_multiBlock->move({-1,0}, getBlocksMatrix());
			break;
		case GLFW_KEY_S:
			if(_multiBlock!=nullptr)
				_multiBlock->rotate(true, getBlocksMatrix());
			break;
		case GLFW_KEY_D:
			if(_multiBlock!=nullptr)
				_multiBlock->move({1,0}, getBlocksMatrix());
			break;
			break;
		case GLFW_KEY_UP:
			if(_multiBlock!=nullptr)
				_multiBlock->rotate(false, getBlocksMatrix());
			break;
		case GLFW_KEY_RIGHT:
			if(_multiBlock!=nullptr)
				_multiBlock->move({1,0}, getBlocksMatrix());
			break;
		case GLFW_KEY_DOWN:
			if(_multiBlock!=nullptr)
				_multiBlock->rotate(true, getBlocksMatrix());
			break;
		case GLFW_KEY_LEFT:
			if(_multiBlock!=nullptr)
				_multiBlock->move({-1,0}, getBlocksMatrix());
			break;
		case GLFW_KEY_SPACE:
			_updateSpeedMS = std::chrono::milliseconds(GAME_STEP_MS/6);
			break;
		case GLFW_KEY_ESCAPE:
			_window->close();
			break;
	}
}

std::vector<std::vector<bool>> Tetris::getBlocksMatrix()
{
	std::vector<std::vector<bool>> matrix;
	for(int i=0;i<GAME_WIDTH;i++)
		matrix.push_back(std::vector<bool>(GAME_HEIGHT));

	// Add blocks to matrix
	for(auto block : _blocks)
	{
		glm::ivec2 pos = block.getPosition();
		if(pos.y<GAME_HEIGHT)
			matrix[pos.x][pos.y]=true;
	}

	// Check game over
	for(int i=0;i<GAME_WIDTH;i++)
		if(matrix[i][GAME_HEIGHT-1])
			_gameOver=true;

	// Check delete row
	for(int i=0;i<GAME_HEIGHT;i++)
	{
		bool full = true;
		for(int j=0;j<GAME_WIDTH;j++)
			if(!matrix[j][i])
				full=false;
		if(full)
		{
			_score+=100;
			std::cout << BOLDGREEN << "+100 points!" << RESET << std::endl;

			// Delete blocks
			std::vector<Block> newBlocks;
			for(auto block : _blocks)
				if(block.getPosition().y!=i)
					newBlocks.push_back(block);
			_blocks = newBlocks;

			// Move other blocks down
			for(auto& block : _blocks)
				if(block.getPosition().y>=i)
					block.move({0,-1});

			// Clean matrix
			for(int i=0;i<GAME_WIDTH;i++)
				for(int j=0;j<GAME_HEIGHT;j++)
					matrix[i][j] = false;
			// Update matrix
			for(auto block : _blocks)
			{
				glm::ivec2 pos = block.getPosition();
				if(pos.y<GAME_HEIGHT)
					matrix[pos.x][pos.y]=true;
			}
			
			// Check again same line
			i--;
		}
	}

	// Add multiBlock to matrix
	std::vector<Block> blocks;
	if(_multiBlock != nullptr)
		blocks = _multiBlock->getBlocks();
	for(auto block : blocks)
	{
		glm::ivec2 pos = block.getPosition();
		if(pos.y<GAME_HEIGHT)
			matrix[pos.x][pos.y]=true;
	}

	return matrix;
}

void Tetris::onDraw()
{
	using clock = std::chrono::high_resolution_clock;
	static auto lastTime = clock::now();
	if(_gameOver)
	{
		_window->close();
		std::cout << BOLDRED << "┌───────────────────────────┐\n";
		std::cout << BOLDRED << "│         GAME OVER         │\n";
		std::cout << BOLDRED << "└───────────────────────────┘\n";
		std::cout << BOLDGREEN << "Score: " << _score << "\nWell played!" << RESET << std::endl;
		return;
	}

	// Clear window
	glClearColor(0.2f,0.2f,0.2f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Draw blocks (fixed blocks)
	for(auto block : _blocks)
		block.draw(_shaders[0]->getShaderId());

	// Draw multiBlock (Blocks being controlled by the player)
	if(_multiBlock == nullptr)
		_multiBlock = new MultiBlock((MultiBlock::MultiBlockType)(rand()%7));
	else
	{
		auto deltaTime = clock::now() - lastTime;
		std::chrono::nanoseconds delta = std::chrono::duration_cast<std::chrono::nanoseconds>(deltaTime);
		if(delta.count()>=_updateSpeedMS.count())
		{
			_multiBlock->move({0,-1}, getBlocksMatrix());
			lastTime = clock::now(); 
		}

		_multiBlock->draw(_shaders[0]->getShaderId());

		// Check collision
		if(_multiBlock->colliding(getBlocksMatrix()))
		{
			std::vector<Block> blocks = _multiBlock->getBlocks();
			for(auto block : blocks)
				_blocks.push_back(block);
			delete _multiBlock;
			_multiBlock=nullptr;
		}
	}

	_window->swapBuffers();
}
