/*
 * ---------------------------------------------------------------------------
 *
 * Podz: A Pod Racing Game
 * Copyright (C) 2006-2018 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 *        File: src/Basis.cpp
 * Description: Vector Space Basis Manipulation Functions
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
#define PODZ_USE_GL
#include "OpenGL.h"

// This module
#include "Vector.h"
#include "Basis.h"

namespace Podz {

Basis::Basis()
    : origin(0.f, 0.f, 0.f), right(1.f, 0.f, 0.f),
      up(0.f, 1.f, 0.f), backward(0.f, 0.f, 1.f)
{
    Setup();
}

Basis::Basis(const Vector &vorigin, const Vector &direction,
	     const Vector &vup)
    : origin(vorigin), up(vup), backward(-direction)
{
    up.Normalize();
    backward.Normalize();
    right = (direction * up) % 1;
    up = backward * right;

    Setup();
}

Basis::Basis(const Vector &vorigin, const Vector &vright,
	     const Vector &vup, const Vector &vbackward)
    : origin(vorigin), right(vright), up(vup), backward(vbackward)
{
    right.Normalize();
    up.Normalize();
    backward.Normalize();

    Setup();
}

void Basis::Setup()
{
    transform[0][0] = right.x;
    transform[0][1] = up.x;
    transform[0][2] = backward.x;
    transform[0][3] = 0.f;
    transform[1][0] = right.y;
    transform[1][1] = up.y;
    transform[1][2] = backward.y;
    transform[1][3] = 0.f;
    transform[2][0] = right.z;
    transform[2][1] = up.z;
    transform[2][2] = backward.z;
    transform[2][3] = 0.f;
    transform[3][0] = 0.f;
    transform[3][1] = 0.f;
    transform[3][2] = 0.f;
    transform[3][3] = 1.f;

    RevertMatrix(transform, invert);
}

Basis Basis::Merge(const Basis &other, const float coef) const
{
    if (coef < 0.f || coef > 1.f)
	return Basis();
    return Basis(origin   * (1.f - coef) + other.origin   * coef,
		 right    * (1.f - coef) + other.right    * coef,
		 up       * (1.f - coef) + other.up       * coef,
		 backward * (1.f - coef) + other.backward * coef);
}

void Basis::Move() const
{
    glTranslatef(origin.x, origin.y, origin.z);
    glMultMatrixf(invert[0]);
}

void Basis::SetView() const
{
    glMultMatrixf(transform[0]);
    glTranslatef(-origin.x, -origin.y, -origin.z);
}

Vector Basis::Mult(const GLfloat matrix[4][4], const Vector &v)
{
    return Vector(
	matrix[0][0] * v.x + matrix[0][1] * v.y + matrix[0][2] * v.z,
	matrix[1][0] * v.x + matrix[1][1] * v.y + matrix[1][2] * v.z,
	matrix[2][0] * v.x + matrix[2][1] * v.y + matrix[2][2] * v.z);
}

bool Basis::RevertMatrix(const GLfloat m[4][4], GLfloat result[4][4])
{
    GLfloat det = m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])
		- m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0])
		+ m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
    if (det == 0.f)
	return false;
    det = 1.f / det;

    result[0][0] = det * (m[1][1] * m[2][2] - m[1][2] * m[2][1]);
    result[0][1] = det * (m[0][2] * m[2][1] - m[0][1] * m[2][2]);
    result[0][2] = det * (m[0][1] * m[1][2] - m[0][2] * m[1][1]);
    result[0][3] = 0.f;
    result[1][0] = det * (m[1][2] * m[2][0] - m[1][0] * m[2][2]);
    result[1][1] = det * (m[0][0] * m[2][2] - m[0][2] * m[2][0]);
    result[1][2] = det * (m[0][2] * m[1][0] - m[0][0] * m[1][2]);
    result[1][3] = 0.f;
    result[2][0] = det * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
    result[2][1] = det * (m[0][1] * m[2][0] - m[0][0] * m[2][1]);
    result[2][2] = det * (m[0][0] * m[1][1] - m[0][1] * m[1][0]);
    result[2][3] = 0.f;
    result[3][0] = 0.f;
    result[3][1] = 0.f;
    result[3][2] = 0.f;
    result[3][3] = 1.f;

    return true;
}

} // namespace Podz

// End of File
