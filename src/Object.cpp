/*
 * ---------------------------------------------------------------------------
 *
 * Podz: A Pod Racing Game
 * Copyright (C) 2006 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 *        File: src/Object.cpp
 * Description: Base Class for Graphics Objects
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
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
#endif // _WIN32

// OpenGL
#ifdef __APPLE__
# include <OpenGL/gl.h>
#else // !__APPLE__
# include <GL/gl.h>
#endif // !__APPLE__

// This module
#include "Vector.h"
#include "Texture.h"
#include "Object.h"


namespace Podz {

int Object::lastList = 0;


Object::Object()
{
    for (int i = 0; i < LIST_NUM; ++i)
	lists[i] = ++lastList;
}

Object::~Object()
{}

void Object::BuildLists()
{
    glNewList(lists[LIST_LIGHTS], GL_COMPILE);
    SetupLightsConst();
    glEndList();

    glNewList(lists[LIST_DISPLAY], GL_COMPILE);
    DisplayConst();
    glEndList();
}

void Object::SetupLights()
{
    glCallList(lists[LIST_LIGHTS]);
    SetupLightsVar();
}

void Object::Display()
{
    glCallList(lists[LIST_DISPLAY]);
    DisplayVar();
}

void Object::SetupModelview() {}
void Object::SetupLightsConst() {}
void Object::SetupLightsVar() {}
void Object::DisplayConst() {}
void Object::DisplayVar() {}

void Object::DrawTriangle(const Vector &point1, const Vector &point2,
			  const Vector &point3, const Texture *texture,
			  const float coord[6])
{
    Vector normal = ((point2 - point1) * (point3 - point1)) % 1;
    glNormal3f(normal.x, normal.y, normal.z);

    if (texture)
	texture->Select();

    glBegin(GL_TRIANGLES);

    if (texture) {
	if (coord) glTexCoord2f(coord[0], coord[1]);
	else       glTexCoord2f(0.f, 0.f);
    }
    glVertex3f(point1.x, point1.y, point1.z);

    if (texture) {
	if (coord) glTexCoord2f(coord[2], coord[3]);
	else       glTexCoord2f(1.f, 1.f);
    }
    glVertex3f(point2.x, point2.y, point2.z);

    if (texture) {
	if (coord) glTexCoord2f(coord[4], coord[5]);
	else       glTexCoord2f(0.f, 1.f);
    }
    glVertex3f(point3.x, point3.y, point3.z);

    glEnd();

    if (texture)
	glDisable(GL_TEXTURE_2D);
}

void Object::DrawQuad(const Vector &point1, const Vector &point2,
		      const Vector &point3, const Vector &point4,
		      const Texture *texture, const float coord[8])
{
    Vector normal = ((point2 - point1) * (point4 - point1)) % 1;
    glNormal3f(normal.x, normal.y, normal.z);

    if (texture)
	texture->Select();

    glBegin(GL_QUADS);

    if (texture) {
	if (coord) glTexCoord2f(coord[0], coord[1]);
	else       glTexCoord2f(0.f, 0.f);
    }
    glVertex3f(point1.x, point1.y, point1.z);

    if (texture) {
	if (coord) glTexCoord2f(coord[2], coord[3]);
	else       glTexCoord2f(1.f, 0.f);
    }
    glVertex3f(point2.x, point2.y, point2.z);

    if (texture) {
	if (coord) glTexCoord2f(coord[4], coord[5]);
	else       glTexCoord2f(1.f, 1.f);
    }
    glVertex3f(point3.x, point3.y, point3.z);

    if (texture) {
	if (coord) glTexCoord2f(coord[6], coord[7]);
	else       glTexCoord2f(0.f, 1.f);
    }
    glVertex3f(point4.x, point4.y, point4.z);

    glEnd();

    if (texture)
	glDisable(GL_TEXTURE_2D);
}


} // namespace Podz

// End of File
