/*
 * ---------------------------------------------------------------------------
 *
 * Podz: A Pod Racing Game
 * Copyright (C) 2006-2018 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 *        File: src/Texture.h
 * Description: Texture Loader (Header)
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


#ifndef PODZ_TEXTURE_H
#define PODZ_TEXTURE_H

// STL
#include <string>
#include <istream>
#include <list>

// OpenGL
#define PODZ_USE_GL
#include "OpenGL.h"


namespace Podz {

class Texture
{
public:
    Texture(const char *const filename);
    ~Texture();

    bool IsLoaded() const { return id != 0; }
    bool Select() const;
    void Free();
    static void LoadAll();
    static void FreeAll();

    static bool IsTexturingEnabled() { return texturing; }
    static void EnableTexturing(bool enable = true) { texturing = enable; }
    static void DisableTexturing() { EnableTexturing(false); }
    static void ToogleTexturing() { texturing = !texturing; }

private:
    const char *filename;
    static bool texturing;
    GLuint id;

    static unsigned GetInt(std::istream &stream, const unsigned bytes);
    bool Load(const char *const filename);
    void Reload() { Load(filename); }

    static std::list<Texture *> all;
    std::list<Texture *>::iterator iterator;

    // No copy/assignment
    Texture(const Texture &);
    void operator =(const Texture &);
};

} // namespace Podz

#endif // !PODZ_TEXTURE_H

// End of File
