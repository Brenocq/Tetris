//------------------------------------//
// Author: Breno Cunha Queiroz        //
// Date: 2020-10-26                   //
// SCC0650 - Computação Gráfica (2020)//
//------------------------------------//
#include "window.h"
#include "defines.h"

float Window::ratio = 1;

Window::Window(std::string name, int width, int height):
	_name(name), _width(width), _height(height)
{
	ratio = (float)_width/_height;
}

Window::~Window()
{

}

void Window::init()
{
    // Check if glfw was initialized
    if(!glfwInit())
    {
        std::cout << BOLDRED << "[Window] GLFW initialization failed!" << RESET << std::endl;
        glfwTerminate();
		exit(1);
    }

    //---------- GLFW config ----------//
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    //---------- Create window ----------//
    _window = glfwCreateWindow(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, _name.c_str(), nullptr, nullptr);
    glfwSetWindowPos(_window, 
			mode->width/2-(_width)/2, 
			mode->height/2-(_height)/2);

	// Check if window was created
    if(_window == nullptr)
    {
        std::cout << BOLDRED << "[Window] Failed to create main window!" << RESET << std::endl;
        glfwTerminate();
        exit(1);
    }

    //---------- Config context ----------//
    // Set to draw to this window
    glfwMakeContextCurrent(_window);

    //---------- Callbacks ----------//
	glfwSetWindowUserPointer(_window, this);
	glfwSetKeyCallback(_window, keyCallback);

	//---------- GLEW ----------//
	if(glewInit())
	{
        std::cout << BOLDRED << "[Window] Glew initialization failed!" << RESET << std::endl;
		glfwDestroyWindow(_window);
		glfwTerminate();
	}
}

void Window::loop()
{
	// Run in loop until the window is closed
	while(!glfwWindowShouldClose(_window))
	{
        glfwPollEvents();

		if(onDraw)
			onDraw();
    }
}

void Window::close()
{
	if(_window != nullptr)
	{
		glfwSetWindowShouldClose(_window, GL_TRUE);
	}
}

float Window::getRatio()
{
	return (float)_width/_height;
}

void Window::swapBuffers()
{
	glfwSwapBuffers(_window);
}

//---------- Callbacks ----------//
void Window::keyCallback(GLFWwindow* window, const int key, const int scancode, const int action, const int mods)
{
	Window* const _this = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (_this->onKey)
	{
		_this->onKey(key, scancode, action, mods);
	}
}

