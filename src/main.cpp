//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-10-26                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <math.h>
#include "defines.h"
#include "window.h"
#include "tetris.h"

int main(int argc, char** argv)
{
	srand(time(NULL));
	Tetris tetris;
	tetris.run();
}

