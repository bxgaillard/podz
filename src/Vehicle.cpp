/*
 * ---------------------------------------------------------------------------
 *
 * Podz: A Pod Racing Game
 * Copyright (C) 2006-2018 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 *        File: src/Vehicle.cpp
 * Description: Vehicle Handling Functions
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
# define _USE_MATH_DEFINES
# define snprintf _snprintf
#endif // _WIN32

// System
#include <cstdio>
#include <cmath>

// OpenGL
#define PODZ_USE_GL
#define PODZ_USE_GLU
#include "OpenGL.h"

// This module
#include "Object.h"
#include "Vector.h"
#include "Basis.h"
#include "Texture.h"
#include "Circuit.h"
#include "Display.h"
#include "Timer.h"
#include "Vehicle.h"


namespace Podz {

static const float LEVIT_HEIGHT = .3f;
static const float INERTY = .995f;
static const float ACCEL = .000005f;
static const float ROT_ANGLE = .025f;
static const float MAX_ACCEL = .0015f;
static const float GRAVITY = .001f;
static const float BORDER = .1f;
static const float GROUND_REACTION_TOUCH_FACTOR = 1.5f;
static const float GROUND_REACTION_FACTOR = 1.f;
static const float GROUND_REACTION_MAX = 4.f * GRAVITY;
static const float GROUND_REACTION_HEIGHT_MAX = LEVIT_HEIGHT * 3.f;
static const float REACTION_FACTOR = .5f;
static const float REACTION_SPEED_FACTOR = 4.f;
static const float REACTION_MIN = 1.f;

static const float SLOPE_INCREASE = .02f;
static const float SLOPE_DECREASE_FACTOR = .97f;
static const float SLOPE_MAX = static_cast<float>(M_PI) / 3.f;
static const float SLOPE_OFFSET_FACTOR = .5f;

static const int LAP_NUM = 3;

Vehicle::Vehicle(Circuit &circ)
    : circuit(circ), timer(0)
{
    static const char *const files[TEX_NUM] = {
	"cockpit", "gray-red", "gray", "back", "top-right", "top-left", "grid"
    };

    for (int i = 0; i < TEX_NUM; ++i)
	textures[i] = new Texture(files[i]);

    Init();
}

void Vehicle::SetupModelview()
{
    glLoadIdentity();
    const Vector eye = position - (direction * 1.5f);
    const Vector up = basis.backward
		    * Vector(basis.right.x, 0.f, basis.right.z);
    glTranslatef(slope * SLOPE_OFFSET_FACTOR, -.6f, 0.f);
    gluLookAt(eye.x, eye.y, eye.z,
	      position.x, position.y, position.z,
	      up.x, up.y, up.z);
}

void Vehicle::SetupLightsConst()
{
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.f, -.5f, -1.8f);

    // Lights

    // feu droite =  source 0
    const float feuDroitPosition[4] = { .1f, .7f, 0.f, 1.f };
    //const float feul_amb[] = { 0.f, 0.f, 0.f, 0.f };
    const float feuIntensiteDiffuse[4] = { 2.f, 2.f, 2.f, 0.f };
    const float feuIntensiteSpeculaire[4] = { 2.f, 2.f, 2.f, 0.f };
    const float feuDroitDirection[4] = { .2f, 0.f, -1.f };

    glLightfv(GL_LIGHT0, GL_POSITION, feuDroitPosition); // position de la source
    //glLightfv(GL_LIGHT0, GL_AMBIENT, l_amb); // intensité ambiante de la source
    glLightfv(GL_LIGHT0, GL_DIFFUSE, feuIntensiteDiffuse); // intensité diffuse de la source
    glLightfv(GL_LIGHT0, GL_SPECULAR, feuIntensiteSpeculaire); // intensité spéculaire de la source
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, feuDroitDirection); // direction de la source
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 60.f); // angle d'ouverture de la source
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 20.f); // atténuation angulaire
    // coefs d'atténuation
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.f);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, .05f);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.f);
    // on active la lumière
    glEnable(GL_LIGHT0);

    // feu gauche =  source 1
    const float feuGauchePosition[4] = { -.1f, .7f, 0.f, 1.f };
    const float feuGaucheDirection[4] = { -.2f, 0.f, -1.f };

    glLightfv(GL_LIGHT1, GL_POSITION, feuGauchePosition); // position de la source
    //glLightfv(GL_LIGHT1, GL_AMBIENT, l_amb); // intensité ambiante de la source
    glLightfv(GL_LIGHT1, GL_DIFFUSE, feuIntensiteDiffuse); // intensité diffuse de la source
    glLightfv(GL_LIGHT1, GL_SPECULAR, feuIntensiteSpeculaire); // intensité spéculaire de la source
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, feuGaucheDirection); // direction de la source
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60.f); // angle d'ouverture de la source
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 20.f); // atténuation angulaire
    // coefs d'atténuation
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, .5f);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, .05f);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.f);
    // on active la lumière
    glEnable(GL_LIGHT1);

    glPopMatrix();
}

void Vehicle::DisplayVar()
{
    // on sauvegarde la matrice
    glPushMatrix();

    Basis(position, direction, basis.up).Move();
    glTranslatef(0.f, .075f, -.55f);
    glRotatef(slope * (180.f / static_cast<float>(M_PI)), 0.f, 0.f, 1.f);

    // type d'affichage
    glPolygonMode(GL_FRONT, GL_FILL);

    // couleur
    glColor3f(.5f, .5f, .5f);

    // cote gauche haut
    const Vector point1(-.2f,   .1f,  .4f); // milieu avant
    const Vector point2(-.085f, .15f, .4f); // haut avant
    const Vector point3(-.085f, .15f, 1.f); // haut arriere
    const Vector point4(-.2f,   .1f,  1.f); // milieu arriere
    DrawQuad(point2, point3, point4, point1, textures[5], 0);

    // cote gauche bas
    const Vector point5(-.2f, 0.f, .4f); // bas avant
    // milieu avant = point1
    // milieu arriere = point4
    const Vector point6(-.2f, 0.f, 1.f); // bas arriere
    DrawQuad(point1, point4, point6, point5, textures[2], 0);

    // cote droit haut
    const Vector point7(.2f,   .1f,  1.f); // milieu arriere
    const Vector point8(.085f, .15f, 1.f); // haut arriere
    const Vector point9(.085f, .15f, .4f); // haut avant
    const Vector point10(.2f,  .1f,  .4f); // milieu avant
    DrawQuad(point8, point9, point10, point7, textures[4], 0);

    // cote droit bas
    const Vector point11(.2f, 0.f, .4f); // bas avant
    // milieu avant = point10
    // milieu arriere = point7
    const Vector point12(.2f, 0.f, 1.f); // bas arriere
    DrawQuad(point10, point7, point12, point11, textures[2], 0);

    // face du dessus arriere
    // arriere gauche = point3
    // avant gauche = point2
    // avant droit = point9
    // arriere droit = point8
    DrawQuad(point2, point9, point8, point3, textures[1], 0);

    // face du dessus avant
    // arriere gauche = point2
    const Vector point13(-.07f, .15f, .3f); // avant gauche
    const Vector point14( .07f, .15f, .3f); // avant droit
    // arriera droit = point9
    const float text1[8] = { .1f, .1f, .9f, .1f, 1.f, 0.f, 0.f, 0.f };
    DrawQuad(point13, point14, point9, point2, textures[1], text1);

    // face arriere bas
    // bas gauche = point6
    // milieu gauche = point4
    // milieu droit = point7
    // bas droit = point12
    DrawQuad(point4, point7, point12, point6, textures[3], 0);

    // face arriere haut
    // milieu gauche = point4
    // haut gauche = point3
    // haut droit = point8
    // milieu droit = point7
    const float text2[8] = { .3f, .5f, .7f, .5f, 1.f, 0.f, 0.f, 0.f };
    DrawQuad(point3, point8, point7, point4, textures[3], text2);

    // face du dessous arriere
    // arriere gauche = point6
    // avant gauche = point5
    // avant droit = point11
    // arriere droit = point12
    DrawQuad(point5, point11, point12, point6, textures[2], 0);

    // face du dessous arriere
    // arriere gauche = point5
    const Vector point15(-.07f, 0.f, 0.f); // avant gauche
    const Vector point16( .07f, 0.f, 0.f); // avant droit
    // arriere droit = point11
    DrawQuad(point15, point16, point11, point5, textures[2], 0);

    // aile droite dessus
    const Vector point17(.2f,  .07f, .8f ); // arriere gauche
    const Vector point18(.2f,  .07f, .55f); // avant
    const Vector point19(.35f, .07f, .8f ); // arriere droit
    DrawTriangle(point18, point19, point17, textures[1], 0);

    // aile droite dessous
    const Vector point20(.2f,  .03f, .8f ); // arriere gauche
    const Vector point21(.2f,  .03f, .55f); // avant
    const Vector point22(.35f, .03f, .8f ); // arriere droit
    DrawTriangle(point21, point22, point20, textures[1], 0);

    // aile droite arriere
    // bas gauche = point20
    // haut gauche = point17
    // haut droite = point19
    // bas droite = point22
    DrawQuad(point17, point19, point22, point20, textures[6], 0);

    // aile droite arriere bout
    // bas gauche = point22
    // haut gauche = point19
    const Vector point23(.4f, .05f, .8f); // milieu droite
    DrawTriangle(point19, point23, point22, textures[2], 0);

    // aile droite avant haut
    // milieu gauche = point23
    // haut gauche = point19
    // haut droite = point18
    const Vector point24(.2f, .05f, .5f); // milieu droite
    DrawQuad(point19, point18, point24, point23, textures[2], 0);

    // aile droite avant bas
    // bas gauche = point22
    // milieu gauche = point23
    // milieu droite = point24
    // bas droite = point21
    DrawQuad(point23, point24, point21, point22, textures[2], 0);

    // aile gauche dessus
    const Vector point25(-.2f,  .07f, .8f ); // arriere droit
    const Vector point26(-.2f,  .07f, .55f); // avant
    const Vector point27(-.35f, .07f, .8f ); //arriere gauche
    DrawTriangle(point26, point27, point25, textures[1], 0);

    // aile gauche dessous
    const Vector point28(-.2f,   .03f, .8f ); // arriere droit
    const Vector point29(-.2f,   .03f, .55f); // avant
    const Vector point30(-.35f, .03f, .8f ); //arriere gauche
    DrawTriangle(point29, point30, point28, textures[1], 0);

    // aile gauche arriere
    // bas gauche = point30
    // haut gauche = point27
    // haut droite = point25
    // bas droite = point28
    DrawQuad(point27, point25, point28, point30, textures[6], 0);

    // aile gauche arriere bout
    // bas droit = point30
    // haut droit = point27
    const Vector point31(-.4f, .05f, .8f); // milieu gauche
    DrawTriangle(point27, point31, point30, textures[2], 0);

    // aile gauche avant haut
    const Vector point32(-.2f, .05f, .5f); // milieu gauche
    // haut gauche = point26
    // haut droite = point27
    // milieu droite = point31
    DrawQuad(point26, point27, point31, point32, textures[2], 0);

    //  aile gauche avant bas
    // bas gauche = point29
    // milieu gauche = point32
    // milieu droite = point31
    // bas droite = point30
    DrawQuad(point32, point31, point30, point29, textures[2], 0);

    // tout devant
    const Vector point33(-.07f, 0.f,  0.f); // bas gauche
    const Vector point34(-.07f, .15f, .3f); // haut gauche
    const Vector point35( .07f, .15f, .3f); // haut droit
    const Vector point36( .07f, 0.f,  0.f); // bas droit
    DrawQuad(point34, point35, point36, point33, textures[0], 0);

    // cote gauche haut avant
    // milieu avant = point1
    // haut avant = point2
    const Vector point37(-.07f, .15f, .3f); // haut tout devant
    DrawTriangle(point2, point37, point1, textures[0], 0);

    // avant gauche haut
    // haut tout devant = point34
    // bas tout devant = point33
    // milieu arriere = point1
    DrawTriangle(point33, point1, point34, textures[0], 0);

    // avant gauche bas
    // milieu arriere = point1
    // bas arriere = point5
    // bas tout devant = point 33
    DrawTriangle(point5, point33, point1, textures[0], 0);

    // cote droit haut avant
    // milieu avant = point10
    // haut avant = point9
    const Vector point38(.07f, .15f, .3f); // haut tout devant
    DrawTriangle(point9, point38, point10, textures[0], 0);

    // avant gauche haut
    // haut tout devant = point35
    // bas tout devant = point36
    // milieu arriere = point10
    DrawTriangle(point36, point10, point35, textures[0], 0);

    // avant droit bas
    // milieu arriere = point10
    // bas arriere = point11
    // bas tout devant = point 36
    DrawTriangle(point11, point36, point10, textures[0], 0);

    // reacteur droit
    glTranslatef(.09f, .07f, 1.f);
    gluCylinder(gluNewQuadric(), .05, .07, .07, 50, 1);

    // reacteur gauche
    glTranslatef(-.18f, 0.f, 0.f);
    gluCylinder(gluNewQuadric(), .05, .07, .07, 50, 1);

    // on remet la matrice
    glPopMatrix();
}

void Vehicle::DisplayOSD()
{
    char buffer[32];

    if (lap <= LAP_NUM) {
	// Fake speed value ;)
	snprintf(buffer, sizeof(buffer), "Speed: %d km/h",
		 static_cast<int>(speed.Length() * 666.f));
	Display::DisplayText(buffer, .1f, .5f);

	snprintf(buffer, sizeof(buffer), "Lap %d/%d", lap, LAP_NUM);
	Display::DisplayText(buffer, -.72f, .5f);

	if (wrongWay)
	    Display::DisplayText("WRONG WAY!",-.63f, 0.f, .0015f);
    }

    snprintf(buffer, sizeof(buffer), "Done: %d%%",
	     static_cast<int>(circPosition /
		     (circuit.GetTotalLength() * LAP_NUM) * 100.f));
    Display::DisplayText(buffer, .3f, -.5f);
}

void Vehicle::Init()
{
    basis = circuit.GetBasis(0.f);
    position = basis.origin + basis.up * LEVIT_HEIGHT / 2.f;
    direction = -basis.backward;
    speed.Set(0.f, 0.f, 0.f);
    circPosition = 0.f;
    lapPosition = 0.f;
    circOffset = 0.f;
    acceleration = 0.f;
    angle = 0.f;
    slope = 0.f;

    accelerated = false;
    wrongWay = false;
    lap = 1;
}

void Vehicle::Move()
{
    const Vector localpos = basis.RevertPoint(position);
    const Vector localspeed = basis.RevertVector(speed);

    direction = basis.TransformVector(Vector(0.f, 0.f, -1.f)
		.Rotate(0.f, angle, 0.f));

    speed = speed * (INERTY) + direction * acceleration
	    + Vector(0.f, -GRAVITY, 0.f);

    float ground = 0.f;
    if (localpos.y < 0) {
	position += basis.up * -localpos.y;
	ground = localspeed.y * -GROUND_REACTION_TOUCH_FACTOR;
    } else if (localpos.y < GROUND_REACTION_HEIGHT_MAX) {
	const float height = LEVIT_HEIGHT / GROUND_REACTION_FACTOR;
	if (localpos.y <= height)
	    ground = GROUND_REACTION_MAX - localpos.y * ((GROUND_REACTION_MAX
		    - GRAVITY) / height);
	else
	    ground = GRAVITY - (localpos.y - height) * (GRAVITY /
		    (GROUND_REACTION_HEIGHT_MAX - height));
	ground *= GROUND_REACTION_FACTOR;
    }
    speed += basis.up * ground;

    const float diff = fabsf(localpos.x)
		     - (circuit.GetWidth(circPosition) * .5f
			+ circuit.GetBorderSlope() * localpos.y - BORDER);
    if (diff > 0) {
	position += basis.right * (2.f * (localpos.x < 0 ? diff : -diff));
	const float min = REACTION_MIN * speed.Length();
	float reaction = localspeed.x * -REACTION_FACTOR;
	if (fabsf(reaction) < min)
	    reaction = localspeed.x < 0 ? min : -min;
	speed *= REACTION_SPEED_FACTOR * fabsf(localspeed.x);
	speed += basis.right * reaction * REACTION_FACTOR;
	acceleration *= .5f;
    }

    position += speed;
    if (accelerated)
	accelerated = false;
    else
	Decelerate(ACCEL / 2.f);
    slope *= SLOPE_DECREASE_FACTOR;

    const Vector newpos = basis.RevertPoint(position);
    circOffset += newpos.x;
    if (newpos.z != 0.f) {
	const float circAdd = -basis.RevertPoint(position).z;
	wrongWay = circAdd < -.001f;
	circPosition += circAdd;
	lapPosition += circAdd;

	const Vector oldright = basis.right;
	basis = circuit.GetBasis(circPosition);

	const Vector newright = basis.RevertVector(oldright);
	if (newright.x < 1.f) {
	    if (newright.z > 0.f)
		angle += acosf(newright.x);
	    else if (newright.z < 0.f)
		angle -= acosf(newright.x);
	}
    } else
	wrongWay = false;

    if (lapPosition >= circuit.GetTotalLength()) {
	lapPosition -= circuit.GetTotalLength();
	if (++lap > LAP_NUM)
	    timer->Finish();
    }
}

void Vehicle::Accelerate()
{
    accelerated = true;
    acceleration += ACCEL;
    if (acceleration > MAX_ACCEL)
	acceleration = MAX_ACCEL;
}

void Vehicle::Brake()
{
    Decelerate(ACCEL * 4.f);
}

void Vehicle::TurnLeft()
{
    angle -= ROT_ANGLE;
    if ((slope += SLOPE_INCREASE) > SLOPE_MAX)
	slope = SLOPE_MAX;
}

void Vehicle::TurnRight()
{
    angle += ROT_ANGLE;
    if ((slope -= SLOPE_INCREASE) < -SLOPE_MAX)
	slope = -SLOPE_MAX;
}

void Vehicle::Decelerate(const float amount)
{
    if (acceleration > 0.f) {
	acceleration -= amount;
	if (acceleration < 0.f)
	    acceleration = 0.f;
    }
}

} // namespace Podz

// End of File
