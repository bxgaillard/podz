/*
 * ---------------------------------------------------------------------------
 *
 * Podz: A Pod Racing Game
 * Copyright (C) 2006 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 *        File: src/PostProcess.cpp
 * Description: Post-Processing Effets Base Class
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

// System
#include <cstring>

// OpenGL
#define PODZ_USE_GL
#include "OpenGL.h"

// This module
#include "PostProcess.h"

namespace Podz {

PostProcess::~PostProcess()
{
    Free();
}

void PostProcess::Init()  {}
void PostProcess::Free()  {}
void PostProcess::Apply() {}

bool PostProcess::IsExtensionSupported(const char *extension)
{
    const GLubyte *extensions = NULL;
    const GLubyte *start;
    GLubyte *where, *terminator;

    // Extension names should not have spaces
    where = reinterpret_cast<GLubyte *>(std::strchr(extension, ' '));
    if (where != 0 || *extension == '\0')
	return false;

    extensions = glGetString(GL_EXTENSIONS);

    // It takes a bit of care to be fool-proof about parsing the OpenGL
    // extensions string. Don't be fooled by sub-strings, etc.
    start = extensions;
    for (;;) {
	where = reinterpret_cast<GLubyte *>(std::strstr(reinterpret_cast<const char *>(start), extension));
	if (where == 0)
	    break;
	terminator = where + std::strlen(extension);
	if ((where == start || *(where - 1) == ' ') &&
	    (*terminator == ' ' || *terminator == '\0'))
	    return true;
	start = terminator;
    }
    return false;
}

} // namespace Podz

// End of File
