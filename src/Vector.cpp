/*
 * ---------------------------------------------------------------------------
 *
 * Podz: A Pod Racing Game
 * Copyright (C) 2006 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 *        File: src/Vector.cpp
 * Description: Vector Manipulation Functions
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

// System
#include <cmath>

// This module
#include "Vector.h"

namespace Podz {

Vector &Vector::Rotate(float rx, float ry, float rz)
{
    const float sin_rx = sinf(rx), sin_ry = sinf(ry), sin_rz = sinf(rz);
    const float cos_rx = cosf(rx), cos_ry = cosf(ry), cos_rz = cosf(rz);

    if (rx != 0.f) {
	const float y2 = y * cos_rx - z * sin_rx;
	z = y * sin_rx + z * cos_rx;
	y = y2;
    }
    if (ry != 0.f) {
	const float x2 = x * cos_ry - z * sin_ry;
	z = x * sin_ry + z * cos_ry;
	x = x2;
    }
    if (rz != 0.f) {
	const float x2 = x * cos_rz - y * sin_rz;
	y = x * sin_rz + y * cos_rz;
	x = x2;
    }

    return *this;
}

Vector Vector::VectorProduct(const Vector &p) const
{
    return Vector(y * p.z - z * p.y,
		  z * p.x - x * p.z,
		  x * p.y - y * p.x);
}

float Vector::Length() const
{
    return sqrtf(x * x + y * y + z * z);
}

} // namespace Podz

// End of File
