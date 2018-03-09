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

#include <iostream>
#include <fstream>
#include <math.h>
#include "graphics.h"
#include "misc.h"

using namespace std;

/* -------------------------------------------
 * shader loading and initialization
 * -------------------------------------------
 */

bool loadShaders(const char * vert, const char * frag, GLuint * prog, GLuint * pVert, GLuint * pFrag)
{
	GLuint v = 0, f = 0, program;
	char *vText = NULL;
	char *fText = NULL;
	
	if (vert != NULL) {
		v = glCreateShader(GL_VERTEX_SHADER);
		vText = readTextFile(vert);
	}
	
	if (frag != NULL) {
		f = glCreateShader(GL_FRAGMENT_SHADER);
		fText = readTextFile(frag);
	}
	
	
	if (vert && vText == NULL) {
		cerr << "Could not load vertex shader: " << vert << endl;
		delete [] vText; delete [] fText;
		return false;
	}
	else if (frag && fText == NULL) {
		cerr << "Could not load fragment shader: " << frag << endl;
		delete [] vText; delete [] fText;
		return false;
	}
	
	const char * vv = vText;
	const char * ff = fText;
	
	if (vert) glShaderSource(v, 1, &vv, NULL);
	if (frag) glShaderSource(f, 1, &ff, NULL);
	
	delete [] vText; delete [] fText;
	vText = NULL; fText = NULL;
	
	if (vert)
	{
		glCompileShader(v);
		if ( !checkCompileStatus(v) )
		{
			cerr << "Problem with vertex shader: " << vert << endl;
			printShaderLog(v);
			return false;
		}
	}
	
	
	if (frag)
	{
		glCompileShader(f);
		if ( !checkCompileStatus(f) )
		{
			cerr << "Problem with fragment shader: " << frag << endl;
			printShaderLog(f);
			
			if (vert) {
				glDeleteShader(v);
			}
			return false;
		}
	}
	
	// link the program
	program = glCreateProgram();
	if (vert) glAttachShader(program, v);
	if (frag) glAttachShader(program, f);
	
	glLinkProgram(program);
	if (! checkLinkStatus(program) ) 
	{
		cerr << "Problem with shader program linkage." << endl;
		printProgramLog(program);
		return false;
	}
	
	// use it
	glUseProgram(program);
	
	// everything OK
	*prog = program;
	
	if (pVert && vert) { *pVert = v; }
	if (pFrag && frag) { *pFrag = f; }
	
	return true;
}

bool loadShadersWithGeometry(
	const char * vert, const char * frag, const char * geom, 
	GLuint * prog, GLuint * pVert, GLuint * pFrag, GLuint * pGeom,
	GLenum inputMode,
	GLenum outputMode
)
{
	GLuint v = 0, f = 0, g = 0, program;
	char *vText = NULL;
	char *fText = NULL;
	char *gText = NULL;
	
	if (vert != NULL) {
		v = glCreateShader(GL_VERTEX_SHADER);
		vText = readTextFile(vert);
	}
	
	if (frag != NULL) {
		f = glCreateShader(GL_FRAGMENT_SHADER);
		fText = readTextFile(frag);
	}
	
	if (geom != NULL) {
		g = glCreateShader(GL_GEOMETRY_SHADER_EXT);
		gText = readTextFile(geom);
	}
		
		
	if (vert && vText == NULL) {
		cerr << "Could not load vertex shader: " << vert << endl;
		delete [] vText; delete [] fText; delete [] gText;
		return false;
	}
	else if (frag && fText == NULL) {
		cerr << "Could not load fragment shader: " << frag << endl;
		delete [] vText; delete [] fText; delete [] gText;
		return false;
	}
	else if (geom && gText == NULL) 
	{
		cerr << "Could not load fragment shader: " << geom << endl;
		delete [] vText; delete [] fText; delete [] gText;
		return false;
	}
	
	const char * vv = vText;
	const char * ff = fText;
	const char * gg = gText;
	
	if (vert) glShaderSource(v, 1, &vv, NULL);
	if (frag) glShaderSource(f, 1, &ff, NULL);
	if (geom) glShaderSource(g, 1, &gg, NULL);
	
	delete [] vText; delete [] fText; delete [] gText;
	vText = NULL; fText = NULL; gText = NULL;
	
	if (vert)
	{
		glCompileShader(v);
		if ( !checkCompileStatus(v) )
		{
			cerr << "Problem with vertex shader: " << vert << endl;
			printShaderLog(v);
			return false;
		}
	}
	
	
	if (frag)
	{
		glCompileShader(f);
		if ( !checkCompileStatus(f) )
		{
			cerr << "Problem with fragment shader: " << frag << endl;
			printShaderLog(f);
			
			if (vert) {
				glDeleteShader(v);
			}
			return false;
		}
	}
	
	if (geom)
	{
		glCompileShader(g);
		if ( !checkCompileStatus(g) )
		{
			cerr << "Problem with geometry shader: " << geom << endl;
			printShaderLog(g);
			
			if (vert) {
				glDeleteShader(v);
			}
			if (frag) {
				glDeleteShader(f);
			}
			return false;
		}
	}
	
	// link the program
	program = glCreateProgram();
	if (vert) glAttachShader(program, v);
	if (frag) glAttachShader(program, f);
	if (geom) glAttachShader(program, g);

	// set max geometry out to maximum of graphics card
	glProgramParameteriEXT(program, GL_GEOMETRY_INPUT_TYPE_EXT, inputMode);
	glProgramParameteriEXT(program, GL_GEOMETRY_OUTPUT_TYPE_EXT, outputMode);

	GLint n;
	GLint setN = 40;

	glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT,&n);
	glProgramParameteriEXT(program,GL_GEOMETRY_VERTICES_OUT_EXT, n);
	cout << "Max vertices OUT from geometry shader: " << n << ", set to: " << n << endl;

	glLinkProgram(program);
	if (! checkLinkStatus(program) ) 
	{
		cerr << "Problem with shader program linkage." << endl;
		printProgramLog(program);
		return false;
	}
	
	// use it
	glUseProgram(program);
	
	// everything OK
	*prog = program;
	
	if (pVert && vert) { *pVert = v; }
	if (pFrag && frag) { *pFrag = f; }
	if (pGeom && geom) { *pGeom = g; }
	
	return true;
}

/* -------------------------------------------
 * shader error checking
 * -------------------------------------------
 */

bool checkCompileStatus(GLuint obj)
{
	GLint status;
	glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
	return status == GL_TRUE;	
}

bool checkLinkStatus(GLuint obj)
{
	GLint status;
	glGetProgramiv(obj, GL_LINK_STATUS, &status);
	return status == GL_TRUE;	
}


bool printShaderLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

	if (infologLength > 0)
	{
		// something bad happened
		
		infoLog = new char[ infologLength ];
		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		cerr << infoLog << endl;
		delete [] infoLog;
		
		return false;
	}
	else
	{
		return true;
	}
}

bool printProgramLog(GLuint obj)
{

	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

	if (infologLength > 0)
	{
		// something bad happened
		
		infoLog = new char[ infologLength ];
	        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
	        
	        cerr << infoLog << endl;
	        delete [] infoLog;
	        
	        return false;
	}
	else
	{
		return true;
	}
}


/* -------------------------------------------
 * OpenGL error checking
 * -------------------------------------------
 */
bool glOK()
{
	// check for OpenGL errors
	GLenum errCode = glGetError();
	if (errCode != GL_NO_ERROR)
	{
		std::cerr << "OpenGL error: " << gluErrorString(errCode) << endl;
		return false;
	} 
	else
	{
		return true;
	}
}

void normalize( float x[3] )
{
	float l = sqrt( x[0]*x[0] + x[1]*x[1] + x[2]*x[2] );
	
	if (l > 0.0f)
	{
		l = 1.0f / l;
		
		
		x[0] *= l;
		x[1] *= l;
		x[2] *= l;
	}
}

