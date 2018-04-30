/*
 * ---------------------------------------------------------------------------
 *
 * Podz: A Pod Racing Game
 * Copyright (C) 2006-2018 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 *        File: src/Timer.h
 * Description: Timer Functions (Header)
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


#ifndef PODZ_TIMER_H
#define PODZ_TIMER_H

#include "Object.h"


namespace Podz
{

class Display;
class Keyboard;

class Timer : public Object
{
public:
    Timer(int intervl, Keyboard &kbd);

    virtual void DisplayOSD();

    void RegisterCallbacks();

    void Start();
    void Stop();
    void Finish();
    void Reset();

    bool IsPaused() const { return state == PAUSE; }
    bool HasStarted() const { return state != BEGIN; }
    bool HasFinished() const { return state == END; }

private:
    int interval, time;
    enum { BEGIN, PAUSE, PLAY, END } state;
    Keyboard &keyboard;

    void OnTimer(int value);

    // GLUT callback
    static Timer *instance;
    static void TimerFunc(int value);

    // No assignment
    void operator =(const Timer &) const;
};

} // namespace Podz

#endif // !PODZ_TIMER_H

// End of File
