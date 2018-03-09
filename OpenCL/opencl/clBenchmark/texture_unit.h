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

#ifndef _TEXTURE_UNIT_H__
#define _TEXTURE_UNIT_H__

#include <stdlib.h>
#include <SDL/SDL_image.h>

#include "graphics.h"

static const int TEXTURE_MONO	= 1;
static const int TEXTURE_RGB	= 3;
static const int TEXTURE_RGBA	= 4;

class TextureUnit;

class TextureUnit
{
private:
	bool loaded;
	GLuint texture_number;
	int width, height, channels;
	unsigned char * raw_texture;
	
	// private member function
	unsigned char * transform(SDL_Surface * surface, int channels);
	void flipVertically(unsigned char *, int w, int h, int channels, int bpc);
	GLenum getGLTextureFormat(int channels);
	
	
public:
	TextureUnit();
	~TextureUnit();

	// performs edge detection and resturns new texture unit
	TextureUnit * edge_detect(const char * filename = NULL) const;
	void merge(const TextureUnit * other, const char * filename = NULL);


	// texture loading
	bool loadTexture(const char * filename, int channels, bool flip = false, bool mipMap = false);
	bool loadRaw(const char * filename, int w, int h, int channels, int bpc, bool flip = false);
	
	// file reading
	bool readFile(const char * filename, int _channels);
	
	// pixel access
	const unsigned char * getPixel(int r, int c);
	
	// unloading function
	bool unloadTexture();
	
	int getWidth() const { return width; }
	int getHeight() const { return height; }
	bool isLoaded() const { return loaded; }
	
	// bind/activate
	void bindTexture();
	GLuint getNumber() const { return texture_number; }
	int  activateTexture(int & texture_id); 
};

bool writeTIFF(const char * filename, unsigned char * theImage, int w, int h, int bpp);

#endif

