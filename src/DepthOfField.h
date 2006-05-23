/*
 * ---------------------------------------------------------------------------
 *
 * Podz: A Pod Racing Game
 * Copyright (C) 2006 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 *        File: src/DepthOfField.h
 * Description: Depth of Field Post-Processing Effet (Header)
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


#ifndef PODZ_DEPTHOFFIELD_H
#define PODZ_DEPTHOFFIELD_H

// OpenGL
#define PODZ_USE_GL
#define PODZ_USE_GLEXT
#include "OpenGL.h"

#include "PostProcess.h"


namespace Podz {

class Display;

class DepthOfField : public PostProcess
{
public:
    DepthOfField(const Display &disp, const float bNear,
		 const float fNear, const float fFar, const float bFar);
    virtual ~DepthOfField();

    virtual void Init();
    virtual void Free();
    virtual void Apply();

private:
    bool initialized;
    const Display &display;

    float blurNear, focalNear, focalFar, blurFar;
    GLuint program, copyProgram, blurProgram, dofProgram;
    GLuint textures[2];
    static const char *fragmentShaderSource;
    static const char *blurVertexShaderSource, *blurFragmentShaderSource;
    static const char *dofVertexShaderSource, *dofFragmentShaderSource;

    DECL_GL_FUNC(PFNGLACTIVETEXTUREARBPROC,       glActiveTextureARB);
    DECL_GL_FUNC(PFNGLMULTITEXCOORD2FARBPROC,     glMultiTexCoord2fARB);
    DECL_GL_FUNC(PFNGLCREATESHADEROBJECTARBPROC,  glCreateShaderObjectARB);
    DECL_GL_FUNC(PFNGLSHADERSOURCEARBPROC,        glShaderSourceARB);
    DECL_GL_FUNC(PFNGLCOMPILESHADERARBPROC,       glCompileShaderARB);
    DECL_GL_FUNC(PFNGLCREATEPROGRAMOBJECTARBPROC, glCreateProgramObjectARB);
    DECL_GL_FUNC(PFNGLATTACHOBJECTARBPROC,        glAttachObjectARB);
    DECL_GL_FUNC(PFNGLLINKPROGRAMARBPROC,         glLinkProgramARB);
    DECL_GL_FUNC(PFNGLUSEPROGRAMOBJECTARBPROC,    glUseProgramObjectARB);
    DECL_GL_FUNC(PFNGLGETUNIFORMLOCATIONARBPROC,  glGetUniformLocationARB);
    DECL_GL_FUNC(PFNGLUNIFORM1IARBPROC,           glUniform1iARB);
    DECL_GL_FUNC(PFNGLUNIFORM1FARBPROC,           glUniform1fARB);
    DECL_GL_FUNC(PFNGLUNIFORM2FARBPROC,           glUniform2fARB);

    // No assignment
    void operator =(const DepthOfField &) const;
};

} // namespace Podz

#endif // !PODZ_DEPTHOFFIELD_H

// End of File
