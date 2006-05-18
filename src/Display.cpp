/*
 * ---------------------------------------------------------------------------
 *
 * Podz: A Pod Racing Game
 * Copyright (C) 2006 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 *        File: src/Display.cpp
 * Description: Display/Rendering Functions
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
# include <cstdlib>
# ifdef _MSC_VER
#  pragma warning(disable: 4505 4702)
# endif // _MSC_VER
#endif // _WIN32

// STL
#include <list>

// OpenGL
#ifdef __APPLE__
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
# include <GLUT/glut.h>
#else // !__APPLE__
# include <GL/gl.h>
# include <GL/glu.h>
# include <GL/glut.h>
#endif // !__APPLE__

// This module
#include "Object.h"
#include "Texture.h"
#include "Display.h"

#ifndef PACKAGE_NAME
# define PACKAGE_NAME "Podz"
#endif // PACKAGE_NAME


namespace Podz {

Display *Display::instance = 0;
bool Display::lighting = true;


Display::Display(const int wwidth, const int wheight)
    : width(wwidth), height(wheight)
{
    instance = this;

    SetFullScreen(false, true);
}

Display::~Display()
{
    for (std::list<Object *>::iterator current = objects.begin();
	 current != objects.end();
	 ++current)
	delete (*current);
    glutDestroyWindow(window);
}

void Display::AddObject(Object *object)
{
    objects.push_back(object);
    object->BuildLists();
}

void Display::SetFullScreen(const bool fullScreen, const bool first)
{
    this->fullScreen = fullScreen;

    if (!first)
	Texture::FreeAll();

    // Initialize the GLUT window
    if (fullScreen) {
	if (!first)
	    glutDestroyWindow(window);

	glutGameModeString("");
	window = glutEnterGameMode();
    } else {
	if (!first)
	    glutLeaveGameMode();

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	window = glutCreateWindow(PACKAGE_NAME);
    }

    if (!first)
	Texture::LoadAll();

    // No cursor
    glutSetCursor(GLUT_CURSOR_NONE);

    // Set GLUT callback functions
    glutDisplayFunc(DisplayFunc);
    glutReshapeFunc(ReshapeFunc);

    // Set global OpenGL parameters
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_NORMALIZE);

    // Rebuild display lists
    RebuildLists();
}

void Display::RebuildLists()
{
    for (std::list<Object *>::iterator object = instance->objects.begin();
	 object != instance->objects.end();
	 ++object)
	(*object)->BuildLists();
}

void Display::DisplayText(const char *const text, const float x,
			  const float y, const float scale)
{
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glPushMatrix();

    glLoadIdentity();
    glTranslatef(x, y, -1.f);
    glScalef(scale, scale, scale);
    glColor3f(0.f, 1.f, 0.f);

    for (int i = 0; text[i] != '\0'; ++i)
	glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);

    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
}

void Display::OnDisplay()
{
    //efface l'écran
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    std::list<Object *>::iterator current;

    for (current = objects.begin(); current != objects.end(); ++current)
	(*current)->SetupModelview();

    for (current = objects.begin(); current != objects.end(); ++current)
	(*current)->SetupLights();

    for (current = objects.begin(); current != objects.end(); ++current) {
	if (IsLightingEnabled())
	    glEnable(GL_LIGHTING);
	else
	    glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	(*current)->Display();
    }

    glutSwapBuffers();
}

void Display::OnReshape(const int width, const int height)
{
    if (!fullScreen)
	this->width = width, this->height = height;

    const float ratio =
	static_cast<float>(width) / static_cast<float>(height);

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(VIEW_ANGLE, ratio, VIEW_NEAR, VIEW_FAR);
    glMatrixMode(GL_MODELVIEW);

    glutPostRedisplay();
}

void Display::DisplayFunc()
{
    instance->OnDisplay();
}

void Display::ReshapeFunc(int width, int height)
{
    instance->OnReshape(width, height);
}

} // namespace Podz

// End of File
