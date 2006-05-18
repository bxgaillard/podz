/*
 * ---------------------------------------------------------------------------
 *
 * Podz: A Pod Racing Game
 * Copyright (C) 2006 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 *        File: src/Vehicle.h
 * Description: Vahicle Handling Functions (Header)
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


#ifndef PODZ_VEHICLE_H
#define PODZ_VEHICLE_H

#include "Object.h"
#include "Vector.h"
#include "Basis.h"

namespace Podz
{

class Circuit;
class Texture;
class Timer;

class Vehicle : public Object
{
public:
    Vehicle(Circuit &circ);

    virtual void SetupModelview();
    virtual void SetupLightsConst();
    virtual void DisplayConst();
    virtual void DisplayVar();

    void Init();
    void Move();
    void Accelerate();
    void Brake();
    void TurnLeft();
    void TurnRight();

    void SetTimer(Timer *const tmr) { timer = tmr; }

private:
    Circuit &circuit;
    Timer *timer;

    enum { TEX_NUM = 7 };
    Texture *textures[TEX_NUM];

    Basis basis;
    Vector position, direction;
    Vector speed;
    float circPosition, lapPosition;
    float circOffset;
    float acceleration;
    float angle;

    bool wrongWay;
    int lap;

    void Decelerate(const float amount);

    // No assignment
    void operator =(const Vehicle &) const;
};

} // namespace Podz

#endif // !PODZ_VEHICLE_H

// End of File
