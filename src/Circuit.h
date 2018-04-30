/*
 * ---------------------------------------------------------------------------
 *
 * Podz: A Pod Racing Game
 * Copyright (C) 2006-2018 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 *        File: src/Circuit.h
 * Description: Circuit Generation and Processing Functions (Header)
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


#ifndef PODZ_CIRCUIT_H
#define PODZ_CIRCUIT_H

// STL
#include <vector>

// This module
#include "Object.h"
#include "Vector.h"
#include "Basis.h"


namespace Podz {

class Texture;

class Circuit : public Object
{
public:
    Circuit(const char *const filename);
    virtual ~Circuit();

    virtual void DisplayConst();

    bool IsLoaded() const { return nb_segs != 0; };
    float GetTotalLength() const { return totalLength; }

    Basis GetBasis(float position) const;
    float GetWidth(float position) const;
    static float GetBorderSlope();

private:
    struct Point {
	Vector point, normal, tangent;
    };

    struct Segment {
	Vector points[4];
	float length, width;
	Basis basis;
    };

    enum { TEX_CIRCUIT = 0, TEX_BORDER, TEX_NUM };
    Texture *textures[TEX_NUM];

    int nb_segs;
    Segment *segments;
    float totalLength;

    void AddSegment(const Point &start, const Point &end,
		    std::vector<Segment> &segs);
};

} // namespace Podz

#endif // !PODZ_CIRCUIT_H

// End of File
