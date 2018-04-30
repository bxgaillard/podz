/*
 * ---------------------------------------------------------------------------
 *
 * Podz: A Pod Racing Game
 * Copyright (C) 2006-2018 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 *        File: src/OpenGL.h
 * Description: Cross-Platform OpenGL Headers Inclusion
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


// Windows
#if defined(_WIN32) && !defined(APIENTRY)
# define WIN32_LEAN_AND_MEAN 1
# include <windows.h> // For APIENTRY definition
#endif // _WIN32 && !APIENTRY

// OpenGL
#ifdef PODZ_USE_GL
# undef PODZ_USE_GL
# ifdef __APPLE__
#  include <OpenGL/gl.h>
# else // !__APPLE__
#  include <GL/gl.h>
# endif // !__APPLE__
#endif // PODZ_USE_GL

// OpenGL extensions
#ifdef PODZ_USE_GLEXT
# undef PODZ_USE_GLEXT
# ifdef __APPLE__
#  include <OpenGL/glext.h>
# elif defined(_WIN32)
#  define glutGetProcAddress wglGetProcAddress
#  include "OpenGLExt.h" // Use bundled SGI's glext.h
# else // !__APPLE__ && !_WIN32
#  include <GL/gl.h>
# endif // !__APPLE__
#endif // PODZ_USE_GLEXT

// GLU
#ifdef PODZ_USE_GLU
# undef PODZ_USE_GLU
# ifdef __APPLE__
#  include <OpenGL/glu.h>
# else // !__APPLE__
#  include <GL/glu.h>
# endif // !__APPLE__
#endif // PODZ_USE_GLU

// GLUT
#ifdef PODZ_USE_GLUT
# undef PODZ_USE_GLUT
# ifdef __APPLE__
#  include <GLUT/glut.h>
# else // !__APPLE__
#  ifdef _WIN32
#   include <cstdlib> // For exit() definition, required by GLUT
#   ifdef _MSC_VER
#    pragma warning(disable: 4505)
#   endif // _MSC_VER
#  else
#    include <GL/glx.h>
#    define glutGetProcAddress glXGetProcAddress
#  endif
#  include <GL/glut.h>
# endif // !__APPLE__
#endif // PODZ_USE_GLUT

// End of File
