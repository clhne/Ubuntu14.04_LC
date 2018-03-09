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

#include "graphics.h"
#include "common.h"

bool fillEnabled = false;
Color fillColor(0, 0, 0, 0);
Color strokeColor(0);

void stroke( Color c )
{
	strokeColor = c;
	glColor4fv( c.v() );
}

void noFill()
{
	fillEnabled = false;
}

void fill( Color c )
{
	fillEnabled = true;
	fillColor = c;
}

void line( float x1, float y1, float x2, float y2 )
{
	glBegin(GL_LINES);
	{
		glVertex2f( x1, y1 );
		glVertex2f( x2, y2 );
	}
	glEnd();
}

void line( float x1, float y1, float z1, float x2, float y2, float z2 )
{
	glBegin(GL_LINES);
	{
		glVertex3f( x1, y1, z1 );
		glVertex3f( x2, y2, z2 );
	}
	glEnd();
}

	

void rect(float x, float y, float w, float h)
{
	/*
	if (fillEnabled)
	{
		glColor4fv( fillColor.v() );
		drawBox(x, y, w, h);
	}
	*/
	
	//glColor4fv( strokeColor.v() );
	
	glBegin(GL_LINES);
	{
		glVertex2f( x, y );
		glVertex2f( x + w, y );
		
		glVertex2f( x+w, y );
		glVertex2f( x+w, y + h );
		
		glVertex2f( x, y+h);
		glVertex2f( x+w, y+h );
		
		glVertex2f( x, y );
		glVertex2f( x, y + h );
	}
	glEnd();
	
}


void drawBox(int lowerX, int lowerY, int w, int h)
{
	glBegin(GL_QUADS);
	{
		glVertex2i( lowerX , lowerY );
		glVertex2i( lowerX + w, lowerY );
		glVertex2i( lowerX + w, lowerY + h );
		glVertex2i( lowerX, lowerY + h );
	}
	glEnd();
}

void drawBox(float lowerX, float lowerY, float w, float h)
{
	glBegin(GL_QUADS);
	{
		glVertex2f( lowerX , lowerY );
		glVertex2f( lowerX + w, lowerY );
		glVertex2f( lowerX + w, lowerY + h );
		glVertex2f( lowerX, lowerY + h );
	}
	glEnd();
}

void drawGradientBox(int lowerX, int lowerY, int w, int h, const Color & left, const Color & right)
{
	glBegin(GL_QUADS);
	{
		glColor4f( left.r(), left.g(), left.b(), left.a() );
		glVertex2i( lowerX , lowerY );
		
		glColor4f( right.r(), right.g(), right.b(), right.a() );
		glVertex2i( lowerX + w, lowerY );
		
		glColor4f( right.r(), right.g(), right.b(), right.a() );
		glVertex2i( lowerX + w, lowerY + h );
		
		glColor4f( left.r(), left.g(), left.b(), left.a() );
		glVertex2i( lowerX, lowerY + h );
	}
	glEnd();
}

void drawTexturedBox(int lowerX, int lowerY, int w, int h)
{
	glBegin(GL_QUADS);
	{
		glTexCoord2f( 0.0, 0.0 );
		glVertex2i( lowerX , lowerY );
		
		glTexCoord2f( 1.0, 0.0 );
		glVertex2i( lowerX + w, lowerY );
		
		glTexCoord2f( 1.0, 1.0 );
		glVertex2i( lowerX + w, lowerY + h );
		
		glTexCoord2f( 0.0, 1.0 );
		glVertex2i( lowerX, lowerY + h );
	}
	glEnd();
}

void drawTexturedBoxF(float lowerX, float lowerY, float w, float h)
{
	glBegin(GL_QUADS);
	{
		glTexCoord2f( 0.0, 0.0 );
		glVertex2f( lowerX , lowerY );
		
		glTexCoord2f( 1.0, 0.0 );
		glVertex2f( lowerX + w, lowerY );
		
		glTexCoord2f( 1.0, 1.0 );
		glVertex2f( lowerX + w, lowerY + h );
		
		glTexCoord2f( 0.0, 1.0 );
		glVertex2f( lowerX, lowerY + h );
	}
	glEnd();
}


