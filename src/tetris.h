//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-10-26                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#ifndef TETRIS_H
#define TETRIS_H
#include <vector>
#include <chrono>
#include "window.h"
#include "shader.h"
#include "block.h"
#include "multiBlock.h"

class Tetris{
	public:
		Tetris();
		~Tetris();

		void run();
	private:
		void printHelp();
		void createShaders();
		std::vector<std::vector<bool>> getBlocksMatrix();

		//---------- Callbacks ----------//
		void onKey(int key, int scancode, int action, int mods);
		void onDraw();

		//---------- Variables ----------//
		Window* _window;
		std::vector<Shader*> _shaders;
		std::vector<Block> _blocks;
		MultiBlock* _multiBlock;
		std::chrono::nanoseconds _updateSpeedMS;
		bool _gameOver;
		int _score;
};

#endif// TETRIS_H
