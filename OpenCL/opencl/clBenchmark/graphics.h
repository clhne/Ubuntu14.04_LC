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

#ifndef _GRAPHICS_H__
#define _GRAPHICS_H__

#include <iostream>
#include <GL/glew.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

// SDL
#include <SDL.h>

bool glOK();

// shader stuff
bool loadShaders(const char * vert, const char * frag, GLuint * prog, GLuint * pVert = NULL, GLuint * pFrag = NULL);
bool loadShadersWithGeometry(
	const char * vert, const char * frag, const char * geom, 
	GLuint * prog, GLuint * pVert = NULL, GLuint * pFrag = NULL, GLuint * pGeom = NULL,
	GLenum input_mode = GL_TRIANGLES,
	GLenum output_mode = GL_TRIANGLE_STRIP
);

bool checkCompileStatus(GLuint obj);
bool checkLinkStatus(GLuint obj);
bool printShaderLog(GLuint obj);
bool printProgramLog(GLuint obj);

// normalizes a vector
void normalize( float x[3] );

#endif


