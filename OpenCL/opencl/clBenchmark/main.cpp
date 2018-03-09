/* ------------------------------------
 *
 * OpenCL image convolution benchmark
 * main.cpp
 * ------------------------------------
 */

#include <iostream>
#include <GL/glew.h>
#include <SDL.h>
#include <time.h>
#include <math.h>

#include "graphics.h"
#include "timer.h"
#include "vector_math.h"
#include "vis.h"
#include "benchmark.h"
#include "batch_test.h"
#include "misc.h"

#define EVER	(;;)
using namespace std;

static int		WINDOW_WIDTH	= 1200;
static int		WINDOW_HEIGHT	= 800;

// pressed mouse button
int pressedMouseButton			= -1;
bool runKernels				= true;
int iterationCount			= 1;

// forward declaration
void loop();
void tick();

const char * inputFile = "data/test_screen.png";

const char * getInputFile()
{
	return inputFile;
}



bool initializeSDL()
{
	int error;
	error = SDL_Init(SDL_INIT_VIDEO);
	
	// initialize SDL parameters for OpenGL context
	/*
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
	
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	*/

	return error >= 0;
}

void initGL()
{	
	// initialize the GLEW library
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		std::cerr << "Error initializing GLEW library " << glewGetErrorString(err) << std::endl;	
	}
	else
	{
		std::cout << "OpenGL status: using GLEW " << glewGetString(GLEW_VERSION) << " with OpenGL version " << glGetString(GL_VERSION) << std::endl;
		if (GL_EXT_geometry_shader4) {
			std::cout << "Geometry shader supported!" << std::endl;
		}
		else
		{
			std::cout << "No Geometry shader :(" << std::endl;
		}
	}
	
	
	glClearColor( 0.0, 0.0, 0.0, 1.0);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	
	// setup projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho( 0.0, (double) WINDOW_WIDTH, 0.0, (double) WINDOW_HEIGHT, -1.0, 1.0 );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// blend function
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// setup viewport
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

void initCL(int argc, char ** argv)
{
	OCLContext::initContext(1, argv);
}

int main(int argc, char ** argv)
{

	
	
	// initialize SDL
	if (!initializeSDL()) {
		std::cerr << "Could not initialize SDL." << endl;
		exit(1);
	}

	bool fullScreen = false;
	int flags = SDL_OPENGL | SDL_DOUBLEBUF;
	if (fullScreen)
	{
		flags |= SDL_FULLSCREEN;
	}
	
	SDL_Surface * drawContext = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, flags);
	if (!drawContext) {
		cerr << "Bad: could not open SDL window :(" << endl;
		exit(1);
	}	
	
	if (argc > 1)
	{
		inputFile = argv[1];
	}
	
	int beginF = 3;
	int endF = 7;
	
	if (argc > 3)
	{
		beginF = atoi( argv[2] );
		endF = atoi( argv[3] );
	}
	
	if (argc > 4)
	{
		iterationCount = atoi( argv[4] );
	}
	
	// initialzie OpenGL
	initGL();
	
	// initialize OpenCL
	cout << "Initializing OpenCL..." << endl;
	initCL(argc, argv);
	cout << "OK" << endl;
	cout << "Testing filter sizes:\t\t" << beginF << "  to  " << endF << "   ..." << endl;
	BatchTest::initialize(beginF, endF);
		
	// seed random number generator
	srand(time(NULL));
		
	// render loop
	loop();
	
	return 0;
}


void tick()
{
	
	if (runKernels)
	{
		
		BatchTest::getInstance()->runTest(iterationCount);
		runKernels = false;
		
		
		cout << "Testing loop unroll...\n" << endl;
		char * fileContents = readTextFile("cl/4_unrolled.cl");
		string fileStr(fileContents);
		delete [] fileContents;
		
		string output;
		if (Benchmarker::unrollLoop(5, fileStr, output))
		{
			cout << output << endl;
		}
		else
		{
			cout << "Could not unroll!" << endl;
		}
	
	}
	
	
	// clear background
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	
	// tick all interpolators
	Interpolator::tickAll( Timer::getInstance()->getDelta() );
	
	// render windowing system
	Vis * vis = Vis::getInstance();
	vis->render();
	
	// flush and swap	
	SDL_GL_SwapBuffers();
}

bool handleSDLEvents()
{
	SDL_Event event;
	Vis * vis = Vis::getInstance();
	
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYUP:
			vis->processKeyUp(event.key);
			if (event.key.keysym.sym == ' ')
			{
				runKernels = true;
			}
			
			break;
		
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				return false;
			}
			else
			{
				vis->processKeyDown(event.key);
			}
			break;
			
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			
			vis->processMouseButton(event.button);
			break;
		
		case SDL_MOUSEMOTION:
			
			vis->processMouseMotion(event.motion);		
			break;
			
		case SDL_QUIT:
			return false;
		}
	}
	return true;
}

void loop()
{
	// for keeping track of FPS
	char fpsBuffer[50];
	int lastUpdate = 0, startTick = 0, endTick = 0, frameCount = 0;
	float accumTime = 0.0f;
	
	// create everything
	Vis::initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	
	for EVER
	{
		// account for time
		startTick = SDL_GetTicks();
		
		// tick
		tick();
		
		endTick = SDL_GetTicks();
		accumTime += (float) (endTick -  startTick) / 1000.0;
		frameCount++;
		
		// update window title
		if (endTick - lastUpdate > 1000)
		{
			// update caption
			accumTime /= (float) frameCount;
			int fps = floor(1.0 / accumTime + 0.5);
			sprintf(fpsBuffer, "CL Benchmark - %d FPS", fps);
			SDL_WM_SetCaption(fpsBuffer, fpsBuffer);
			
			lastUpdate = endTick;
			accumTime = 0.0f;
			frameCount = 0;
		}
		
		if (!handleSDLEvents()) {
			// exit loop
			break;
		}
		
		Timer::getInstance()->advance();
	}
	
}

