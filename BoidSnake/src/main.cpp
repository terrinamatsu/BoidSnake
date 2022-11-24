#ifdef WIN32
#define SDL_MAIN_HANDLED
#endif // WIN32


#include <cstdlib>
#include <iostream>
#include <ngl/NGLInit.h>
#include <ngl/NGLStream.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include <SDL.h>


#include "Arena.h"
#include "RenderGlobals.h"


/// \author Jacob Worgan
/// \version 1.0
/// \15/6/2020

/// \Class main
/// \setup for SDL and OpenGL for the project,
///  and handles the main gameplay loop and controls. 


SDL_GLContext createOpenGLContext(SDL_Window* _window);
void setupShaders();

static int WIDTH = 1024;
static int HEIGHT = 720;
static float ASPECT = static_cast<float>(WIDTH) / HEIGHT;


int main()
{
	// Initialise SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << "error init sdl\n";
		return EXIT_FAILURE;
	}

	auto window = SDL_CreateWindow("Game", 0, 0, WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	// Initialise OpenGL
	auto glContext = createOpenGLContext(window);
	if (!glContext)
	{
		std::cerr << "can't create GL context \n";
		exit(EXIT_FAILURE);
	}
	SDL_GL_MakeCurrent(window, glContext);
	ngl::NGLInit::instance();
	setupShaders();
	glClearColor(1, 1, 1, 1);
	glEnable(GL_DEPTH_TEST);

	// Set inital Render Settings
	RenderGlobals::setView({ 0.0f, 30.0f, -20.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,1.0f });
	RenderGlobals::setProjection(45.0f, ASPECT);
	RenderGlobals::setWindow(WIDTH, HEIGHT);

	//Arena arena(20, 20);
	Arena arena("maps/map1.png");

	SDL_Event event;
	bool quit = false;

	// Controller Variables
	// Key Movement
	int xDir = 0;
	int yDir = 0;
	// Mouse Position
	int mouseX, mouseY;

	// Main Gameplay Loop
	while (!quit)
	{
		// Key Events
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT: quit = true; break;
			case SDL_KEYDOWN : 
				switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE: quit = true; break;
					//case SDLK_LEFT:   xDir = -1;   break;
					//case SDLK_RIGHT:  xDir = 1;    break;
					//case SDLK_UP:     yDir = -1;    break;
					//case SDLK_DOWN:   yDir = 1;   break;
				}
			break; // end key down
			case SDL_KEYUP:
				switch (event.key.keysym.sym)
				{
					//case SDLK_LEFT:  xDir = 0; break;
					//case SDLK_RIGHT: xDir = 0; break;
					//case SDLK_UP:    yDir = 0; break;
					//case SDLK_DOWN:  yDir = 0; break;
				}
			break; // end key up
			}
		} // end poll event

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, WIDTH, HEIGHT);
		
		// Mouse Control Updates
		SDL_GetMouseState(&mouseX, &mouseY);
		std::cout << mouseX << ' ' << mouseY << '\n';

		// Arena Updates
		arena.keyEvent(xDir, yDir, mouseX, mouseY);
		arena.update(0.3f);
		arena.draw();

		SDL_GL_SwapWindow(window);

		SDL_Delay(10);
	} // end game loop

	return EXIT_SUCCESS;
}  

SDL_GLContext createOpenGLContext(SDL_Window* _window)
{
	// Open GL setup
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	return SDL_GL_CreateContext(_window);
}

void setupShaders()
{
	// Setting up shaders to be used throughout the project

	// Diffuse Shader
	auto shader = ngl::ShaderLib::instance();
	shader->use(ngl::nglDiffuseShader);
	shader->setUniform("Colour", 1.0f, 0.0f, 0.0f, 1.0f);
	shader->setUniform("lightPos", 0.0f, 10.0f, 0.0f);
	shader->setUniform("lightDiffuse", 1.0f, 1.0f, 1.0f, 1.0f);
	shader->setUniform("normalMatrix", ngl::Mat3());
	// Ground checked shader
	shader->use(ngl::nglCheckerShader);
	shader->setUniform("lightPos", 0.0f, 10.0f, 0.0f);
	shader->setUniform("lightDiffuse", 1.0f, 1.0f, 1.0f, 1.0f);
	shader->setUniform("checkOn", true);
	shader->setUniform("colour1", 0.9f, 0.9f, 0.9f, 1.0f);
	shader->setUniform("colour2", 0.6f, 0.6f, 0.6f, 1.0f);
	shader->setUniform("checkSize", 10.0f);
	shader->setUniform("normalMatrix", ngl::Mat3());
}