/*
 * ---------------------------------------------------------------------------
 *
 * Podz: A Pod Racing Game
 * Copyright (C) 2006-2018 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 *        File: src/Basis.h
 * Description: Vector Space Basis Manipulation Functions (Header)
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


#ifndef PODZ_BASIS_H
#define PODZ_BASIS_H

#define PODZ_USE_GL
#include "OpenGL.h"

#include "Vector.h"


namespace Podz {

class Basis
{
public:
    Vector origin, right, up, backward;

    Basis();
    Basis(const Vector &vorigin, const Vector &direction, const Vector &vup);
    Basis(const Vector &vorigin, const Vector &vright,
	  const Vector &vup, const Vector &vbackward);

    Basis Merge(const Basis &other, const float coef = .5f) const;

    Vector TransformVector(const Vector &v) const
	{ return Mult(transform, v); }
    Vector RevertVector(const Vector &v) const
	{ return Mult(invert, v); }

    Vector TransformPoint(const Vector &v) const
	{ return Mult(transform, v) + origin; }
    Vector RevertPoint(const Vector &v) const
	{ return Mult(invert, v - origin); }

    void Rotate(const float rx, const float ry, const float rz)
    {
	right.Rotate(rx, ry, rz);
	up.Rotate(rx, ry, rz);
	backward.Rotate(rx, ry, rz);
    }

    void Move() const;
    void SetView() const;

private:
    GLfloat transform[4][4];
    GLfloat invert[4][4];

    void Setup();

    static Vector Mult(const GLfloat matrix[4][4], const Vector &v);
    public:
    static bool RevertMatrix(const GLfloat m[4][4], GLfloat result[4][4]);
};

} // namespace Podz

#endif // !PODZ_BASIS_H

// End of File
