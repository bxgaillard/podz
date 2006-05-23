/*
 * ---------------------------------------------------------------------------
 *
 * Podz: A Pod Racing Game
 * Copyright (C) 2006 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 *        File: src/PostProcess.h
 * Description: Post-Processing Effets Base Class (Header)
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


#ifndef PODZ_POSTPROCESS_H
#define PODZ_POSTPROCESS_H

namespace Podz {

#define DECL_GL_FUNC(type, name) static type name
#define IMPL_GL_FUNC(type, name, class) type class::name
#define INIT_GL_FUNC(type, name) (name = (type) glutGetProcAddress(#name))


class PostProcess
{
public:
    virtual ~PostProcess() = 0;

    virtual void Init();
    virtual void Free();
    virtual void Apply();

    void Enable(bool enable = true) { enabled = enable; }
    void Disable() { Enable(false); }
    bool IsEnabled() const { return enabled; }

protected:
    explicit PostProcess(bool enable = true) : enabled(enable) {}

    static bool IsExtensionSupported(const char *extension);

private:
    bool enabled;
};

} // namespace Podz

#endif // !PODZ_POSTPROCESS_H

// End of File
