/*
 * ---------------------------------------------------------------------------
 *
 * Podz: A Pod Racing Game
 * Copyright (C) 2006-2018 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 *        File: src/Display.h
 * Description: Display/Rendering Functions (Header)
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


#ifndef PODZ_DISPLAY_H
#define PODZ_DISPLAY_H

#include <list>


namespace Podz
{

class Object;
class PostProcess;
class Textures;

static const float VIEW_ANGLE = 60.f, VIEW_NEAR = .1f, VIEW_FAR = 1000.f;

class Display
{
public:
    Display(const int wwidth = 640, const int wheight = 480);
    ~Display();

    void AddObject(Object *object);
    void AddPostProcess(PostProcess *postproc);

    void SetFullScreen(const bool fullScreen, const bool first = false);
    void RebuildLists();

    static void DisplayText(const char *const text, const float x,
			    const float y, const float scale = 0.0005f);

    void EnableLighting(bool enabled = true) { lighting = enabled; }
    void DisableLighting() { EnableLighting(false); }
    bool IsLightingEnabled() const { return lighting; }
    void ToogleLighting() { lighting = !lighting; }

    int GetWidth() const { return realWidth; }
    int GetHeight() const { return realHeight; }

private:
    int window;
    int width, height, realWidth, realHeight;
    bool fullScreen;
    bool lighting;

    std::list<Object *> objects;
    std::list<PostProcess *> postprocs;

    void OnDisplay();
    void OnReshape(const int width, const int height);

    // GLUT callbacks
    static Display *instance;
    static void DisplayFunc();
    static void ReshapeFunc(int width, int height);
};

} // namespace Podz

#endif // !PODZ_DISPLAY_H

// End of File
