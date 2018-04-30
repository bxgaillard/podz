/*
 * ---------------------------------------------------------------------------
 *
 * Podz: A Pod Racing Game
 * Copyright (C) 2006-2018 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 *        File: src/Application.h
 * Description: Main Function (Header)
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


#ifndef PODZ_APPLICATION_H
#define PODZ_APPLICATION_H

namespace Podz {

class Display;
class Keyboard;
class Timer;

class Application
{
public:
    Application();
    ~Application();

    void DoToogleFullScreen();

    static void ToogleFullScreen() { instance->DoToogleFullScreen(); };
    static void Exit(int code = 0);

private:
    Display *display;
    Keyboard *keyboard;
    Timer *timer;

    bool fullScreen;

    static Application *instance;
    static void OnExit();
};

} // namespace Podz

#endif // !PODZ_APPLICATION_H

// End of File
