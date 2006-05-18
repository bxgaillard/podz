/*
 * ---------------------------------------------------------------------------
 *
 * Podz: A Pod Racing Game
 * Copyright (C) 2006 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 *        File: src/Timer.cpp
 * Description: Timer Functions
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
# define snprintf _snprintf
# ifdef _MSC_VER
#  pragma warning(disable: 4505)
# endif // _MSC_VER
#endif // _WIN32

// System
#include <cstdio>

// OpenGL
#ifdef __APPLE__
# include <GLUT/glut.h>
#else // !__APPLE__
# include <GL/glut.h>
#endif // !__APPLE__

// This module
#include "Keyboard.h"
#include "Display.h"
#include "Timer.h"

namespace Podz
{

Timer *Timer::instance = 0;

Timer::Timer(int intervl, Keyboard &kbd)
    : interval(intervl), time(0), state(BEGIN), keyboard(kbd)
{
    instance = this;
}

void Timer::Start()
{
    if (state == BEGIN || state == PAUSE) {
	state = PLAY;
	glutTimerFunc(0, TimerFunc, 0);
    }
}

void Timer::Stop()
{
    state = PAUSE;
    glutPostRedisplay();
}

void Timer::Finish()
{
    state = END;
    glutPostRedisplay();
}

void Timer::Reset()
{
    state = BEGIN;
    time = 0;
    glutPostRedisplay();
}

void Timer::OnTimer(int value)
{
    if (state != PLAY)
	return;

    time += interval;

    glutTimerFunc(interval, TimerFunc, value);
    keyboard.CheckKeys();
    glutPostRedisplay();
}

void Timer::DisplayVar()
{
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "Time: %d s", time / 1000);
    Display::DisplayText(buffer, -.7f, -.5f);

    switch (state) {
    case BEGIN:
	Display::DisplayText("Move to begin", -.5f, 0.f, .001);
	break;

    case PAUSE:
	Display::DisplayText("PAUSE", -.4f, 0.f, .002);
	break;

    case END:
	Display::DisplayText("Congratulations!", -.45f, 0.f, .001);
	break;

    default:
	break;
    }
}

void Timer::RegisterCallbacks()
{
    if (state == PLAY)
	glutTimerFunc(interval, TimerFunc, 0);
}

void Timer::TimerFunc(int value)
{
    instance->OnTimer(value);
}

} // namespace Podz

// End of File
