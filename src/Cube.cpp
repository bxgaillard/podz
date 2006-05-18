/*
 * ---------------------------------------------------------------------------
 *
 * Podz: A Pod Racing Game
 * Copyright (C) 2006 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 *        File: src/Cube.cpp
 * Description: Surrounding Cube
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

// Windows
#ifdef _WIN32
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
#endif // _WIN32

// OpenGL
#ifdef __APPLE__
# include <OpenGL/gl.h>
#else // !__APPLE__
# include <GL/gl.h>
#endif // !__APPLE__

// This module
#include "Object.h"
#include "Texture.h"
#include "Cube.h"


namespace Podz
{

Cube::Cube(const float size)
    : dim(size * .5f)
{
    static const char *const files[TEX_NUM] = {
	"bg-front", "bg-left", "bg-right", "bg-back", "bg-top", "bg-bottom"
    };

    for (int i = 0; i < TEX_NUM; ++i)
	textures[i] = new Texture(files[i]);
}

Cube::~Cube()
{
    for (int i = 0; i < TEX_NUM; ++i)
	delete textures[i];
}

void Cube::SetupLightsConst()
{
    // Ambient
    static const float ambient[] = { .4f, .4f, .4f, 1.f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
    glShadeModel(GL_SMOOTH);

    // Top Light
    // lumière 3 au dessus de l'ensemble
    float position[] = { 0.f, 50.f, 0.f, 1.f };
    float direction[] = { 0.f, -1.f, 0.f };
    float intensiteDiffuse[] = { .7f, .7f, .7f, 0.f };
    float intensiteSpeculaire[] = { .5f, .5f, .5f, 0.f };
    glLightfv(GL_LIGHT3, GL_POSITION, position); // position de la source
    //glLightfv(GL_LIGHT3, GL_AMBIENT, l_amb); // intensité ambiante de la source
    glLightfv(GL_LIGHT3, GL_DIFFUSE, intensiteDiffuse); // intensité diffuse de la source
    glLightfv(GL_LIGHT3, GL_SPECULAR, intensiteSpeculaire); // intensité spéculaire de la source
    glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, direction); // direction de la source
    glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 180.f); // angle d'ouverture de la source
    glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 40.f); // atténuation angulaire
    // coefs d'atténuation
    glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 1.f);
    glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, .01f);
    glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0.f);
    // on active la lumière
    glEnable(GL_LIGHT3);
    // matiere
    float mat_emi[] = { 0.f, 0.f, 0.f, 0.f };
    //float mat_diff[] = { .7f, .7f, .7f, 0.f };
    float mat_spec[] = { .8f, .8f, .8f, 0.f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emi);
    //glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_spec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10.f);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    //glEnable(GL_COLOR_MATERIAL);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
}

void Cube::DisplayConst()
{
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glPolygonMode(GL_FRONT, GL_FILL);
    glColor3f(0.f, 1.f, 0.f);

    textures[FRONT]->Select();
    glBegin(GL_QUADS);
    glNormal3f(0.f, 0.f, 1.f);
    glTexCoord2f(0.f, 0.f); glVertex3f(-dim,  dim, -dim);
    glTexCoord2f(1.f, 0.f); glVertex3f( dim,  dim, -dim);
    glTexCoord2f(1.f, 1.f); glVertex3f( dim, -dim, -dim);
    glTexCoord2f(0.f, 1.f); glVertex3f(-dim, -dim, -dim);
    glEnd();

    textures[LEFT]->Select();
    glBegin(GL_QUADS);
    glNormal3f(1.f, 0.f, 0.f);
    glTexCoord2f(0.f, 0.f); glVertex3f(-dim,  dim,  dim);
    glTexCoord2f(1.f, 0.f); glVertex3f(-dim,  dim, -dim);
    glTexCoord2f(1.f, 1.f); glVertex3f(-dim, -dim, -dim);
    glTexCoord2f(0.f, 1.f); glVertex3f(-dim, -dim,  dim);
    glEnd();

    textures[RIGHT]->Select();
    glBegin(GL_QUADS);
    glNormal3f(-1.f, 0.f, 0.f);
    glTexCoord2f(0.f, 0.f); glVertex3f( dim,  dim, -dim);
    glTexCoord2f(1.f, 0.f); glVertex3f( dim,  dim,  dim);
    glTexCoord2f(1.f, 1.f); glVertex3f( dim, -dim,  dim);
    glTexCoord2f(0.f, 1.f); glVertex3f( dim, -dim, -dim);
    glEnd();

    textures[BACK]->Select();
    glBegin(GL_QUADS);
    glNormal3f(0.f, 0.f, -1.f);
    glTexCoord2f(0.f, 0.f); glVertex3f( dim,  dim,  dim);
    glTexCoord2f(1.f, 0.f); glVertex3f(-dim,  dim,  dim);
    glTexCoord2f(1.f, 1.f); glVertex3f(-dim, -dim,  dim);
    glTexCoord2f(0.f, 1.f); glVertex3f( dim, -dim,  dim);
    glEnd();

    textures[TOP]->Select();
    glBegin(GL_QUADS);
    glNormal3f(0.f, -1.f, 0.f);
    glTexCoord2f(0.f, 0.f); glVertex3f(-dim,  dim,  dim);
    glTexCoord2f(1.f, 0.f); glVertex3f( dim,  dim,  dim);
    glTexCoord2f(1.f, 1.f); glVertex3f( dim,  dim, -dim);
    glTexCoord2f(0.f, 1.f); glVertex3f(-dim,  dim, -dim);
    glEnd();

    textures[BOTTOM]->Select();
    glBegin(GL_QUADS);
    glNormal3f(0.f, 1.f, 0.f);
    glTexCoord2f(0.f, 0.f); glVertex3f(-dim, -dim, -dim);
    glTexCoord2f(1.f, 0.f); glVertex3f( dim, -dim, -dim);
    glTexCoord2f(1.f, 1.f); glVertex3f( dim, -dim,  dim);
    glTexCoord2f(0.f, 1.f); glVertex3f(-dim, -dim,  dim);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

} // namespace Podz

// End of File
