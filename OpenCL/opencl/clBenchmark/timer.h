/*   Rising Tide exhibit                                                    */
/*   Copyright (C) 2010 Khairi Reda                                         */
/*   Electronic Visualization Laboratory                                    */
/*   University of Illinois at Chicago                                      */
/*   http://www.evl.uic.edu/                                                */
/*                                                                          */
/*   This is free software; you can redistribute it and/or modify it        */
/*   under the terms of the  GNU General Public License  as published by    */
/*   the  Free Software Foundation;  either version 2 of the License, or    */
/*   (at your option) any later version.                                    */
/*                                                                          */
/*   This program is distributed in the hope that it will be useful, but    */
/*   WITHOUT  ANY  WARRANTY;  without   even  the  implied  warranty  of    */
/*   MERCHANTABILITY or  FITNESS FOR A PARTICULAR PURPOSE.   See the GNU    */
/*   General Public License for more details.                               */
/* ======================================================================== */

#ifndef _TIMING_H__
#define _TIMING_H__

#include <stdlib.h>
#include <SDL.h>

class Timer;
class Timer
{
private:
	
	double theTime;
	double theDelta;
	
	// SDL ticks
	int tempTicks, lastTicks;
	
	// static instance
	static Timer * instance;

	// constructor
	Timer()
	{
		theTime = 0.0;
		theDelta = 0.0;
		lastTicks = SDL_GetTicks();
	}

public:
		
	// accessors
	double getTime() const { return theTime; }
	double getDelta() const { return theDelta; }
	
	// should be called once per frame only!
	void advance() 
	{
		tempTicks = SDL_GetTicks();
		theDelta = ((double) (tempTicks - lastTicks)) / 1000.0;
		theTime += theDelta;
		lastTicks = tempTicks;
	}
	
	// static instance accessor
	static Timer * getInstance();
};

#endif

