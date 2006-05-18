/*
 * ---------------------------------------------------------------------------
 *
 * Podz: A Pod Racing Game
 * Copyright (C) 2006 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 *        File: src/Cube.h
 * Description: Surrounding Cube (Header)
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


#ifndef PODZ_CUBE_H
#define PODZ_CUBE_H

#include "Object.h"


namespace Podz {

class Texture;

class Cube : public Object
{
public:
    Cube(const float size);
    virtual ~Cube();

    virtual void SetupLightsConst();
    virtual void DisplayConst();

private:
    enum { FRONT = 0, LEFT, RIGHT, BACK, TOP, BOTTOM, TEX_NUM };
    float dim;
    Texture *textures[TEX_NUM];
};

} // namespace Podz

#endif // !PODZ_CUBE_H