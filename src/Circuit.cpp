/*
 * ---------------------------------------------------------------------------
 *
 * Podz: A Pod Racing Game
 * Copyright (C) 2006 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 *        File: src/Circuit.cpp
 * Description: Circuit Generation and Processing Functions
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

// STL
#include <vector>
#include <fstream>

// System
#include <cmath>

// OpenGL
#define PODZ_USE_GL
#include "OpenGL.h"

// This module
#include "Object.h"
#include "Vector.h"
#include "Texture.h"
#include "Circuit.h"

namespace Podz {

static const float SEG_LENGTH = 2.f;
static const float CIRC_WIDTH = 4.f;
static const float BORDER_WIDTH = .3f, BORDER_HEIGHT = .8f;

Circuit::Circuit(const char *const filename)
    : nb_segs(0), segments(0)
{
    static const char *const files[TEX_NUM] = { "circuit", "border" };

    for (int i = 0; i < TEX_NUM; ++i)
	textures[i] = new Texture(files[i]);

    std::ifstream file(filename);
    if (!file.is_open())
	return;

    int nb_pts;
    file >> nb_pts;

    Point *ptTan = new Point[nb_pts + 2];

    for (int i = 1; i <= nb_pts; ++i) {
	file >> ptTan[i].point.x
	     >> ptTan[i].point.y
	     >> ptTan[i].point.z
	     >> ptTan[i].normal.x
	     >> ptTan[i].normal.y
	     >> ptTan[i].normal.z;
	ptTan[i].normal.Normalize();
    }

    if (!file.good()) {
	delete[] ptTan;
	nb_segs = 0;
	return;
    }

    ptTan[nb_pts + 1].point = ptTan[1].point;
    ptTan[nb_pts + 1].normal = ptTan[1].normal;
    ptTan[0].point = ptTan[nb_pts].point;
    ptTan[0].normal = ptTan[nb_pts].normal;

    for (int i = 0; i < nb_pts; ++i)
	ptTan[i + 1].tangent =
	    (((ptTan[i + 1].point - ptTan[i].point) % 1) +
	     ((ptTan[i + 2].point - ptTan[i + 1].point) % 1)) % 1;
    ptTan[nb_pts + 1].tangent = ptTan[1].tangent;
    ptTan[0].tangent = ptTan[nb_pts].tangent;

    std::vector<Segment> frags;
    totalLength = 0.f;
    for (int i = 1; i <= nb_pts; ++i)
	AddSegment(ptTan[i], ptTan[i + 1], frags);

    delete[] ptTan;

    nb_segs = static_cast<int>(frags.size());
    segments = new Segment[nb_segs + 1];
    for (int i = 0; i < nb_segs; ++i)
	segments[i] = frags[i];
    segments[nb_segs] = segments[0];
}

Circuit::~Circuit()
{
    for (int i = 0; i < TEX_NUM; ++i)
	delete textures[i];

    if (segments != 0)
	delete[] segments;
}

void Circuit::DisplayConst()
{
    const int tex[3] = { TEX_BORDER, TEX_CIRCUIT, TEX_BORDER };

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(.3f, .3f, 1.f);

    for (int i = 0; i < nb_segs; ++i) {
	const Vector normals[2][3] = {
	    {
		(segments[i].basis.up * BORDER_WIDTH +
		    segments[i].basis.right * BORDER_HEIGHT) % 1.f,
		segments[i].basis.up,
		(segments[i].basis.up * BORDER_WIDTH -
		    segments[i].basis.right * BORDER_HEIGHT) % 1.f,
	    }, {
		(segments[i + 1].basis.up * BORDER_WIDTH +
		    segments[i + 1].basis.right * BORDER_HEIGHT) % 1.f,
		segments[i + 1].basis.up,
		(segments[i + 1].basis.up * BORDER_WIDTH -
		    segments[i + 1].basis.right * BORDER_HEIGHT) % 1.f,
	    }
	};

	for (int j = 0; j < 3; ++j) {
	    textures[tex[j]]->Select();
	    glBegin(GL_QUADS);

	    glNormal3f(normals[0][j].x, normals[0][j].y, normals[0][j].z);
	    glTexCoord2f(0.f, 0.f);
	    glVertex3f(segments[i].points[j].x,
		       segments[i].points[j].y,
		       segments[i].points[j].z);
	    glTexCoord2f(0.f, 1.f);
	    glVertex3f(segments[i].points[j + 1].x,
		       segments[i].points[j + 1].y,
		       segments[i].points[j + 1].z);

	    glNormal3f(normals[1][j].x, normals[1][j].y, normals[1][j].z);
	    glTexCoord2f(1.f, 1.f);
	    glVertex3f(segments[i + 1].points[j + 1].x,
		       segments[i + 1].points[j + 1].y,
		       segments[i + 1].points[j + 1].z);
	    glTexCoord2f(1.f, 0.f);
	    glVertex3f(segments[i + 1].points[j].x,
		       segments[i + 1].points[j].y,
		       segments[i + 1].points[j].z);

	    glEnd();
	}
    }
    glDisable(GL_TEXTURE_2D);
}

Basis Circuit::GetBasis(float position) const
{
    float length;
    int cursor = 0;

    while (position < 0.f)
	position += totalLength;
    while (position >= totalLength)
	position -= totalLength;

    while (position >= (length = segments[cursor].length)) {
	position -= length;
	if (++cursor == nb_segs)
	    cursor = 0;
    }

    return segments[cursor].basis.Merge(segments[cursor + 1].basis,
					position / length);
}

float Circuit::GetWidth(float position) const
{
    float length;
    int cursor = 0;

    while (position < 0.f)
	position += totalLength;
    while (position >= totalLength)
	position -= totalLength;

    while (position >= (length = segments[cursor].length)) {
	position -= length;
	if (++cursor == nb_segs)
	    cursor = 0;
    }

    return segments[cursor].width * (1.f - position) +
	   segments[cursor + 1].width * position;
}

float Circuit::GetBorderSlope()
{
    return BORDER_WIDTH / BORDER_HEIGHT;
}

void Circuit::AddSegment(const Point &start, const Point &end,
			 std::vector<Segment> &segs)
{
    const Vector diff = end.point - start.point;
    const float len = diff.Length(), len_4 = len / 4.f;

    if (len > SEG_LENGTH) {
	Point middle = {
	    ((start.point + start.tangent * len_4) +
		    (end.point - end.tangent * len_4)) / 2.f,
	    (end.normal + start.normal) * .5f,
	    diff / len
	};

	AddSegment(start, middle, segs);
	AddSegment(middle, end, segs);
    } else {
	const Vector right = (diff * start.normal) % 1,
	pt2 = start.point - right * (CIRC_WIDTH * .5f),
	pt3 = start.point + right * (CIRC_WIDTH * .5f),
	pt1 = pt2 - right * BORDER_WIDTH + start.normal * BORDER_HEIGHT,
	pt4 = pt3 + right * BORDER_WIDTH + start.normal * BORDER_HEIGHT;

	Segment newseg = {
	    { pt1, pt2, pt3, pt4 }, len, CIRC_WIDTH,
	    Basis(start.point, diff, start.normal)
	};
	segs.push_back(newseg);
	totalLength += len;
    }
}

} // namespace Podz

// End of File
