/* ------------------------------------
 *
 * OpenCL image convolution benchmark
 * vis.cpp
 * ------------------------------------
 */
 
#include "graphics.h"
#include "vis.h"
#include "benchmark.h"
#include "batch_test.h"
#include "common.h"
#include "misc.h"
#include "vector_math.h"

const char * getInputFile();

Vis * Vis::initialize(int _w, int _h)
{
	assert(!instance);
	instance = new Vis(_w, _h);
	
	return instance;
}


Vis::Vis(int _w, int _h): w(_w), h(_h)
{
	zoom.setCurrent(1.0);
	translationX.setCurrent(0.0);
	translationY.setCurrent(0.0);
	mouseButton = false;
	
	inputImage.loadTexture(getInputFile(), 4, false, false);
	
	// initialize font
	font.init("data/GarrisonLightSansBold.ttf", 30);	
}


void Vis::render()
{
	BatchTest * batchTest = BatchTest::getInstance();
	static int s = min( w, h );
	
	glPushMatrix();
	{
		static float winW = (float) w / 2.0f;
		static float winH = (float) h / 2.0f;
		
		glTranslatef(
			winW, 
			winH, 
		0.0 );
		glScalef( zoom.getCurrent(), zoom.getCurrent(), 1.0 );
		glTranslatef( 
			-winW + translationX.getCurrent(), 
			-winH + translationY.getCurrent(), 
		0.0 );


		
		// loop through all benchmarks (filter sizes)
		for (int b = 0; b < batchTest->getTestCount(); b++)
		{
		
			const Benchmarker * benchmarker = batchTest->getTest(b);
			
			// loop through all kernels
			for (int i = 0; i < benchmarker->getKernelCount(); i++)
			{

				glPushAttrib(GL_ALL_ATTRIB_BITS);
				glDisable(GL_TEXTURE_2D);
				glColor3f(0.8, 0.0, 0.0);
				glLineWidth(3.0);
				rect(b*(s+10), i*(s+10), s, s);		

				glPopAttrib();
		
				benchmarker->bindTexture(i);
				drawTexturedBox(b*(s+10), i*(s+10), s, s);
				
				glPushAttrib(GL_ENABLE_BIT);
				//glEnable(GL_BLEND);
				{
					freetype::print(font, b*(s+10) + 10, i*(s+10) + 10, "%d ms", (int) (benchmarker->getKernel(i)->getAvgRunTime()));
				}
				glPopAttrib();
				
				if (i == 0)
				freetype::print(font,  b*(s+10) + 10, - 40, "%d x %d", benchmarker->getFilterSize(), benchmarker->getFilterSize());


			}
			
			if (b == 0)
			{
				inputImage.bindTexture();
				drawTexturedBox(-1*(s+10), 0, s, s);
			}
			
		}
		
		
	}
	glPopMatrix();
}

void Vis::processMouseButton(SDL_MouseButtonEvent event)
{
	bool newState = event.type == SDL_MOUSEBUTTONDOWN;

	switch (event.button)
	{
	case SDL_BUTTON_LEFT:
		mouseButton = newState;
		break;
		
	case SDL_BUTTON_WHEELUP:
		if (newState)
		{
			zoom *= 1.1;
		}
		break;
		
	case SDL_BUTTON_WHEELDOWN:
		if (newState)
		{
			zoom *= 1.0 / 1.1;
		}
		break;
	}
}

void Vis::processMouseMotion(SDL_MouseMotionEvent event)
{
	int mouseX = event.x;
	int mouseY = h - event.y;
	
	if (mouseButton)
	{
		translationX += ((float) event.xrel) / zoom.getCurrent();
		translationY += ((float) -event.yrel) / zoom.getCurrent();
	}
}
		

Vis * Vis::instance = NULL;


