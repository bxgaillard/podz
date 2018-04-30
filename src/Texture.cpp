/*
 * ---------------------------------------------------------------------------
 *
 * Podz: A Pod Racing Game
 * Copyright (C) 2006-2018 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 *        File: src/Texture.cpp
 * Description: Texture Loader
 *
 * ---------------------------------------------------------------------------
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc., 59
 * Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * ---------------------------------------------------------------------------
 */


// Configuration
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif // HAVE_CONFIG_H

// Windows
#ifdef _WIN32
# define DIRSEP "\\"
# ifdef _MSC_VER
#  pragma warning(disable: 4702)
# endif // _MSC_VER
#else // !_WIN32
# define DIRSEP "/"
#endif // !_WIN32

// STL
#include <ios>
#include <istream>
#include <fstream>
#include <iostream>
#include <string>
#include <list>

// OpenGL
#define PODZ_USE_GL
#define PODZ_USE_GLEXT
#define PODZ_USE_GLU
#include "OpenGL.h"

// This module
#include "Texture.h"

/*
// Define this constant if not handled by <GL/gl.h>
#ifndef GL_CLAMP_TO_EDGE
# ifdef GL_CLAMP_TO_EDGE_EXT
#  define GL_CLAMP_TO_EDGE GL_CLAMP_TO_EDGE_EXT
# else // !GL_CLAMP_TO_EDGE_EXT
#  define GL_CLAMP_TO_EDGE 0x812F
# endif // !GL_CLAMP_TO_EDGE_EXT
#endif // !GL_CLAMP_TO_EDGE
*/


namespace Podz {

bool Texture::texturing = true;
std::list<Texture *> Texture::all;

Texture::Texture(const char *const fname)
    : filename(fname), id(0)
{
    if (!Load(filename))
	std::cerr << "WARNING: could not load texture '" << filename << "'."
		  << std::endl;

    all.push_back(this);
    iterator = --all.end();
}

Texture::~Texture()
{
    Free();
    all.erase(iterator);
}

void Texture::Free()
{
    glDeleteTextures(1, &id);
    id = 0;
}

void Texture::FreeAll()
{
    for (std::list<Texture *>::iterator i = all.begin(); i != all.end(); ++i)
	(*i)->Free();
}

void Texture::LoadAll()
{
    for (std::list<Texture *>::iterator i = all.begin(); i != all.end(); ++i)
	(*i)->Reload();
}

unsigned Texture::GetInt(std::istream &stream, const unsigned bytes)
{
    unsigned result = 0;
    for (unsigned i = 0; i < bytes; ++i)
	result |= (static_cast<unsigned>(stream.get()) & 0xFF) << (i * 8);
    return result;
}

bool Texture::Select() const
{
    // Switch to this texture
    if (texturing) {
	glBindTexture(GL_TEXTURE_2D, id);
	glEnable(GL_TEXTURE_2D);
	glColor3f(1.f, 1.f, 1.f);
    }

    return IsLoaded();
}

bool Texture::Load(const char *const filename)
{
    // Create input stream
    std::ifstream file((std::string("textures" DIRSEP) + filename + ".bmp")
		       .c_str(), std::ios::binary);
    if (!file.is_open())
	return false;

    // Read image size
    file.ignore(18);
    const unsigned width = GetInt(file, 4);
    const unsigned height = GetInt(file, 4);

    // Read image BPP
    if (GetInt(file, 2) != 1)
	return false; // 1 plane only
    const unsigned bpp = GetInt(file, 2);
    if (bpp != 24 && bpp != 32)
	return false; // 24 and 32 bpp only

    // Compute memory size
    const unsigned size = width * height * (bpp / 8);
    file.ignore(24);
    if (!file.good())
	return false;

    // Read data and reverse lines
    const unsigned lineSize = width * (bpp / 8);
    char *data = (new char[size]) + size;
    for (unsigned i = 0; i < height; ++i)
	file.read(data -= lineSize, lineSize);
    if (!file.good()) {
	delete data;
	return false;
    }
    file.close();

    // Invert components (BGR -> RGB)
    for (unsigned i = 0; i < size; i += bpp / 8) {
	char temp = data[i];
	data[i] = data[i + 2];
	data[i + 2] = temp;
    }

    // Generate and bind texture
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // Build texture
    gluBuild2DMipmaps(GL_TEXTURE_2D, bpp / 8, width, height,
		      bpp == 24 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);

    // Set texture parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		    GL_LINEAR_MIPMAP_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // Free memory and return
    delete[] data;
    return true;
}

} // namespace Podz

// End of File
