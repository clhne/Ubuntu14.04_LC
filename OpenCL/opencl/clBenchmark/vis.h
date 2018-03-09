/* ------------------------------------
 *
 * OpenCL image convolution benchmark
 * vis.h
 * ------------------------------------
 */

#ifndef _VIS__H__
#define _VIS__H__

#include <assert.h>
#include "vector_math.h"
#include "texture_unit.h"
#include "FreeType.h"

class Vis
{
public:
	static Vis * getInstance() { assert(instance); return instance; }
	static Vis * initialize(int w, int h);
	void render();
	
	// events
	void processMouseButton(SDL_MouseButtonEvent event);
	void processMouseButtonDown(SDL_MouseButtonEvent event) {}
	void processMouseButtonUp(SDL_MouseButtonEvent event) {}
	
	void processMouseMotion(SDL_MouseMotionEvent event);
	void processKeyDown(SDL_KeyboardEvent event) {}
	void processKeyUp(SDL_KeyboardEvent event) {}
	
private:
	Interpolator translationX, translationY, zoom;
	bool mouseButton;
	TextureUnit inputImage;
	
	// dimensions
	int w;
	int h;
	
	
	freetype::font_data font;
	
	// private constructor
	Vis(int, int);
	
	// static instance
	static Vis * instance;
	
};

#endif

