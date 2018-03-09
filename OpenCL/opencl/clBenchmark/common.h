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

#ifndef _GUI_COMMON_H__
#define _GUI_COMMON_H__

#include "graphics.h"
#include "vector_math.h"

void drawBox(int lowerX, int lowerY, int w, int h);
void drawGradientBox(int lowerX, int lowerY, int w, int h, const Color & left, const Color & right);
void drawTexturedBox(int lowerX, int lowerY, int w, int h);
void drawTexturedBoxF(float lowerX, float lowerY, float w, float h);


void rect(float, float, float, float);
void stroke( Color c );

static void strokeWeight( float f )
{
	glLineWidth( f );
}

void noFill();
void fill( Color c );
void line( float x1, float y1, float x2, float y2 );
void line( float x1, float y1, float z1, float x2, float y2, float z2 );

#endif


