//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-10-26                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#ifndef WINDOW_H
#define WINDOW_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <functional>

class Window{
	public:
		Window(std::string name, int width, int height);
		~Window();

		void init();
		void loop();
		void close();
		float getRatio();
		void swapBuffers();

		//---------- Callbacks ----------//
		std::function<void(int key, int scancode, int action, int mods)> onKey;
		std::function<void()> onDraw;

		static float ratio;
	private:
		static void keyCallback(GLFWwindow* window, const int key, const int scancode, const int action, const int mods);

		GLFWwindow* _window;
		std::string _name;
		int _width;
		int _height;
};

#endif// WINDOW_H
