/*
 * ---------------------------------------------------------------------------
 *
 * Podz: A Pod Racing Game
 * Copyright (C) 2006-2018 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 *        File: src/Application.cpp
 * Description: Main Function
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
# define WIN32_LEAN_AND_MEAN 1
# include <windows.h>
# define chdir !SetCurrentDirectoryA
# define DIRSEP "\\"
#else // !_WIN32
# define DIRSEP "/"
#endif // !_WIN32

// STL
#include <iostream>

// System
#include <cstdlib>
#ifdef DATA_DIR
# include <unistd.h>
#endif // DATA_DIR

// OpenGL
#define PODZ_USE_GLUT
#include "OpenGL.h"

// This module
#include "Display.h"
#include "Keyboard.h"
#include "Timer.h"
#include "Cube.h"
#include "Circuit.h"
#include "Vehicle.h"
#include "DepthOfField.h"
#include "Application.h"

#ifndef PACKAGE_TARNAME
#define PACKAGE_TARNAME "podz"
#endif // PACKAGE_TARNAME


namespace Podz {

Application *Application::instance = 0;

Application::Application()
    : fullScreen(false)
{
    if (
#ifdef DATA_DIR
	chdir(DATA_DIR) != 0 &&
#endif // DATA_DIR
	chdir("data") != 0 && chdir(".." DIRSEP "data") != 0) {
	std::cerr << "Error: cannot find data directory." << std::endl;
	std::exit(1);
    }

    display = new Display;

    Circuit *const circuit = new Circuit("level.txt");
    if (!circuit->IsLoaded()) {
	std::cerr << "Error: could not load level." << std::endl;
	std::exit(2);
    }

    Vehicle *const vehicle = new Vehicle(*circuit);
    Cube *const cube = new Cube(1000.f);

    keyboard = new Keyboard(*display, *vehicle);
    timer = new Timer(10, *keyboard);
    keyboard->SetTimer(timer);
    vehicle->SetTimer(timer);

    display->AddObject(cube);
    display->AddObject(circuit);
    display->AddObject(vehicle);
    display->AddObject(timer);

    display->AddPostProcess(new DepthOfField(*display, -2.f, 2.f, 5.f, 30.f));

    // Ensure resources get freed
    instance = this;
    std::atexit(OnExit);
}

Application::~Application()
{
    timer->Stop();
    delete display;
    delete keyboard;
    //delete timer; -- done by display
}

void Application::DoToogleFullScreen()
{
    if (fullScreen) {
	fullScreen = false;
	display->SetFullScreen(fullScreen);
	//timer->RegisterCallbacks();
	keyboard->RegisterCallbacks();
    } else {
	fullScreen = true;
	display->SetFullScreen(fullScreen);
	//timer->RegisterCallbacks();
	keyboard->RegisterCallbacks();
    }
}

void Application::Exit(int code)
{
    std::exit(code);
}

void Application::OnExit(void)
{
    delete instance;
}

} // namespace Podz


extern "C" int main(int argc, char **argv)
{
    // Initialization
    glutInit(&argc, argv);
    new Podz::Application;

    // Main loop
    glutMainLoop();

    // We should never get here
    return EXIT_SUCCESS;
}

#ifdef _WIN32
extern "C" int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    char *argv[] = { PACKAGE_TARNAME };
    main(sizeof(argv) / sizeof(*argv), argv);
}
#endif

// End of File
