/*
 * ---------------------------------------------------------------------------
 *
 * Podz: A Pod Racing Game
 * Copyright (C) 2006-2018 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 *        File: src/Object.h
 * Description: Base Class for Graphics Objects (Header)
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


#ifndef PODZ_OBJECT_H
#define PODZ_OBJECT_H

namespace Podz {

class Vector;
class Texture;

class Object
{
public:
    virtual ~Object() = 0;

    enum List { LIST_LIGHTS = 0, LIST_DISPLAY, LIST_NUM };

    void BuildLists();
    void SetupLights();
    void Display();

    virtual void SetupModelview();
    virtual void SetupLightsConst();
    virtual void SetupLightsVar();
    virtual void DisplayConst();
    virtual void DisplayVar();
    virtual void DisplayOSD();

protected:
    Object();

    static void DrawTriangle(const Vector &point1, const Vector &point2,
			     const Vector &point3, const Texture *texture,
			     const float coord[6]);
    static void DrawQuad(const Vector &point1, const Vector &point2,
			 const Vector &point3, const Vector &point4,
			 const Texture *texture, const float coord[8]);

private:
    int lists;
};

} // namespace Podz

#endif // !PODZ_OBJECT_H

// End of File
