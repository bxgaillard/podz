/*
 * ---------------------------------------------------------------------------
 *
 * Podz: A Pod Racing Game
 * Copyright (C) 2006 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 *        File: src/Keyboard.h
 * Description: Keyboard Handling Functions (Header)
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


#ifndef PODZ_KEYBOARD_H
#define PODZ_KEYBOARD_H

namespace Podz
{

class Vehicle;
class Timer;

class Keyboard
{
public:
    Keyboard(Vehicle &vehi);

    static void RegisterCallbacks();
    void CheckKeys() const;
    void SetTimer(Timer *const tmr) { timer = tmr; }

private:
    enum { KEY_UP = 0, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_NUM };

    static Keyboard *instance;
    Vehicle &vehicle;
    Timer *timer;

    bool pressed[KEY_NUM];

    void UpdateKey(int key, bool state);

    void KeyPressed(unsigned char key, int x, int y);
    void SpecialKeyPressed(int key, int x, int y);
    void SpecialKeyReleased(int key, int x, int y);

    // GLUT callbacks
    static void KeyboardFunc(unsigned char key, int x, int y);
    static void SpecialFunc(int key, int x, int y);
    static void SpecialUpFunc(int key, int x, int y);

    // No assignment
    void operator =(const Keyboard &) const;
};

} // namespace Podz

#endif // !PODZ_KEYBOARD_H

// End of File
