/*
 * ---------------------------------------------------------------------------
 *
 * Podz: A Pod Racing Game
 * Copyright (C) 2006-2018 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 *        File: src/Keyboard.cpp
 * Description: Keyboard Handling Functions
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

// OpenGL
#define PODZ_USE_GLUT
#include "OpenGL.h"

// This module
#include "Vehicle.h"
#include "Display.h"
#include "Texture.h"
#include "Timer.h"
#include "Application.h"
#include "Keyboard.h"


namespace Podz
{

Keyboard *Keyboard::instance = 0;

Keyboard::Keyboard(Display &disp, Vehicle &vehi)
    : display(disp), vehicle(vehi), timer(0)
{
    if (glutDeviceGet(GLUT_HAS_KEYBOARD) != 1)
	return;

    for (int i = 0; i < KEY_NUM; ++i)
	pressed[i] = false;

    instance = this;
    RegisterCallbacks();
}

void Keyboard::RegisterCallbacks()
{
    glutKeyboardFunc(KeyboardFunc);
    glutSpecialFunc(SpecialFunc);
    glutSpecialUpFunc(SpecialUpFunc);
    glutIgnoreKeyRepeat(2);
}

void Keyboard::CheckKeys() const
{
    if (pressed[KEY_UP])
	vehicle.Accelerate();
    if (pressed[KEY_DOWN])
	vehicle.Brake();
    if (pressed[KEY_LEFT])
	vehicle.TurnLeft();
    if (pressed[KEY_RIGHT])
	vehicle.TurnRight();

    vehicle.Move();
}

void Keyboard::UpdateKey(int key, bool state)
{
    switch (key) {
    case GLUT_KEY_UP:
	pressed[KEY_UP] = state;
	break;

    case GLUT_KEY_DOWN:
	pressed[KEY_DOWN] = state;
	break;

    case GLUT_KEY_LEFT:
	pressed[KEY_LEFT] = state;
	break;

    case GLUT_KEY_RIGHT:
	pressed[KEY_RIGHT] = state;
    }
}

void Keyboard::KeyPressed(unsigned char key, int, int)
{
    switch (key) {
    case 27: // Escape
    case 'Q':
    case 'q':
	Application::Exit();

    case ' ':
    case 'P':
    case 'p':
	if (timer->HasStarted() && !timer->HasFinished()) {
	    if (timer->IsPaused())
		timer->Start();
	    else
		timer->Stop();
	}
	break;

    case 'R':
    case 'r':
	vehicle.Init();
	timer->Reset();
	break;

    case 'L':
    case 'l':
	display.ToogleLighting();
	glutPostRedisplay();
	break;

    case 'T':
    case 't':
	Texture::ToogleTexturing();
	display.RebuildLists();
	glutPostRedisplay();
	break;

    case 'F':
    case 'f':
	Application::ToogleFullScreen();
    }
}

void Keyboard::SpecialKeyPressed(int key, int, int)
{
    switch (key) {
    case GLUT_KEY_UP:
    case GLUT_KEY_DOWN:
    case GLUT_KEY_LEFT:
    case GLUT_KEY_RIGHT:
	UpdateKey(key, true);
	if (!timer->IsPaused())
	    timer->Start();
    }
}

void Keyboard::SpecialKeyReleased(int key, int, int)
{
    UpdateKey(key, false);
}

void Keyboard::KeyboardFunc(unsigned char key, int x, int y)
{
    instance->KeyPressed(key, x, y);
}

void Keyboard::SpecialFunc(int key, int x, int y)
{
    instance->SpecialKeyPressed(key, x, y);
}

void Keyboard::SpecialUpFunc(int key, int x, int y)
{
    instance->SpecialKeyReleased(key, x, y);
}

} // namespace Podz

// End of File
