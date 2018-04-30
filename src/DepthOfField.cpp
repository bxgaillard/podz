/*
 * ---------------------------------------------------------------------------
 *
 * Podz: A Pod Racing Game
 * Copyright (C) 2006-2018 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 *        File: src/DepthOfField.cpp
 * Description: Depth of Field Post-Processing Effet
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
#include <cstring>

// OpenGL
#define PODZ_USE_GL
#define PODZ_USE_GLEXT
#define PODZ_USE_GLUT
#include "OpenGL.h"

// This module
#include "PostProcess.h"
#include "Display.h"
#include "DepthOfField.h"


namespace Podz {

IMPL_GL_FUNC(PFNGLACTIVETEXTUREARBPROC,       glActiveTextureARB,
	     DepthOfField);
IMPL_GL_FUNC(PFNGLMULTITEXCOORD2FARBPROC,     glMultiTexCoord2fARB,
	     DepthOfField);
IMPL_GL_FUNC(PFNGLCREATESHADEROBJECTARBPROC,  glCreateShaderObjectARB,
	     DepthOfField);
IMPL_GL_FUNC(PFNGLSHADERSOURCEARBPROC,        glShaderSourceARB,
	     DepthOfField);
IMPL_GL_FUNC(PFNGLCOMPILESHADERARBPROC,       glCompileShaderARB,
	     DepthOfField);
IMPL_GL_FUNC(PFNGLCREATEPROGRAMOBJECTARBPROC, glCreateProgramObjectARB,
	     DepthOfField);
IMPL_GL_FUNC(PFNGLATTACHOBJECTARBPROC,        glAttachObjectARB,
	     DepthOfField);
IMPL_GL_FUNC(PFNGLLINKPROGRAMARBPROC,         glLinkProgramARB,
	     DepthOfField);
IMPL_GL_FUNC(PFNGLUSEPROGRAMOBJECTARBPROC,    glUseProgramObjectARB,
	     DepthOfField);
IMPL_GL_FUNC(PFNGLGETUNIFORMLOCATIONARBPROC,  glGetUniformLocationARB,
	     DepthOfField);
IMPL_GL_FUNC(PFNGLUNIFORM1IARBPROC,           glUniform1iARB,
	     DepthOfField);
IMPL_GL_FUNC(PFNGLUNIFORM1FARBPROC,           glUniform1fARB,
	     DepthOfField);
IMPL_GL_FUNC(PFNGLUNIFORM2FARBPROC,           glUniform2fARB,
	     DepthOfField);


const char *DepthOfField::vertexShaderSource =
    "void main()\n"
    "{\n"
    "    gl_Position = ftransform();\n"
    "    vec4 eyeCoordPos = gl_ModelViewMatrix * gl_Vertex;\n"
    "    gl_FogFragCoord = abs(eyeCoordPos.z / eyeCoordPos.w);\n"
    "}\n";

const char *DepthOfField::fragmentShaderSource =
    "uniform float blurNear, focalNear, focalFar, blurFar;\n"
    "\n"
    "uniform sampler2D texture;\n"
    "\n"
    "float DepthBlur(float depth)\n"
    "{\n"
    "    float blur;\n"
    "    if (depth < focalNear)\n"
    "        blur = (depth - focalNear) / (blurNear - focalNear);\n"
    "    else\n"
    "        blur = max((depth - focalFar) / (blurFar - focalFar), 0.0);\n"
    "    return min(blur, 1.0);\n"
    "}\n"
    "\n"
    "void main()\n"
    "{\n"
    "    vec4 tex = texture2D(texture, gl_TexCoord[0].st) * gl_Color;\n"
    "    vec4 mixed = mix(tex, gl_Color, 1.0 - tex.a);\n"
    "    gl_FragColor = vec4(mixed.rgb, DepthBlur(gl_FogFragCoord));\n"
    "}\n";

const char *DepthOfField::blurVertexShaderSource =
    "void main()\n"
    "{\n"
    "    gl_Position = gl_Vertex;\n"
    "    gl_TexCoord[0] = gl_MultiTexCoord0;\n"
    "}\n";

const char *DepthOfField::blurFragmentShaderSource =
    "uniform vec2 texSize;\n"
    "uniform sampler2D texture;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    vec2 offset = vec2(1.0 / texSize.x, 1.0 / texSize.y);\n"
    "\n"
    "    vec4 c  = texture2D(texture, gl_TexCoord[0].st);\n"
    "    vec4 bl = texture2D(texture, gl_TexCoord[0].st +\n"
    "                                 vec2(-offset.x, -offset.y));\n"
    "    vec4 l  = texture2D(texture, gl_TexCoord[0].st +\n"
    "                                 vec2(-offset.x, 0.0      ));\n"
    "    vec4 tl = texture2D(texture, gl_TexCoord[0].st +\n"
    "                                 vec2(-offset.x, offset.y ));\n"
    "    vec4 t  = texture2D(texture, gl_TexCoord[0].st +\n"
    "                                 vec2(0.0,       offset.y ));\n"
    "    vec4 ur = texture2D(texture, gl_TexCoord[0].st +\n"
    "                                 vec2(offset.x,  offset.y ));\n"
    "    vec4 r  = texture2D(texture, gl_TexCoord[0].st +\n"
    "                                 vec2(offset.x,  0.0      ));\n"
    "    vec4 br = texture2D(texture, gl_TexCoord[0].st +\n"
    "                                 vec2(offset.x,  offset.y ));\n"
    "    vec4 b  = texture2D(texture, gl_TexCoord[0].st +\n"
    "                                 vec2(0.0,       -offset.y));\n"
    "\n"
    "    gl_FragColor = 0.25 * c + 0.125 * (l + t + r + b) +\n"
    "                   0.0625 * (bl + tl + ur + br);\n"
    "}\n";

const char *DepthOfField::dofVertexShaderSource =
    "void main()\n"
    "{\n"
    "    gl_Position = gl_Vertex;\n"
    "    gl_TexCoord[0] = gl_MultiTexCoord0;\n"
    "    gl_TexCoord[1] = gl_MultiTexCoord1;\n"
    "}\n";

const char *DepthOfField::dofFragmentShaderSource =
    "uniform sampler2D frame, blur;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    vec4 pixel = texture2D(frame, gl_TexCoord[0].st);\n"
    "    vec4 blurred = texture2D(blur, gl_TexCoord[1].st);\n"
    "    gl_FragColor = vec4(mix(pixel.rgb, blurred.rgb, pixel.a), 0.0);\n"
    "}\n";


DepthOfField::DepthOfField(const Display &disp, const float bNear,
			   const float fNear, const float fFar,
			   const float bFar)
    : initialized(false), display(disp),
      blurNear(bNear), focalNear(fNear), focalFar(fFar), blurFar(bFar)
{
    if (!IsExtensionSupported("GL_ARB_multitexture") ||
	!IsExtensionSupported("GL_ARB_shader_objects") ||
	!IsExtensionSupported("GL_ARB_shading_language_100"))
	return;

    INIT_GL_FUNC(PFNGLACTIVETEXTUREARBPROC,       glActiveTextureARB);
    INIT_GL_FUNC(PFNGLMULTITEXCOORD2FARBPROC,     glMultiTexCoord2fARB);
    INIT_GL_FUNC(PFNGLCREATESHADEROBJECTARBPROC,  glCreateShaderObjectARB);
    INIT_GL_FUNC(PFNGLSHADERSOURCEARBPROC,        glShaderSourceARB);
    INIT_GL_FUNC(PFNGLCOMPILESHADERARBPROC,       glCompileShaderARB);
    INIT_GL_FUNC(PFNGLCREATEPROGRAMOBJECTARBPROC, glCreateProgramObjectARB);
    INIT_GL_FUNC(PFNGLATTACHOBJECTARBPROC,        glAttachObjectARB);
    INIT_GL_FUNC(PFNGLLINKPROGRAMARBPROC,         glLinkProgramARB);
    INIT_GL_FUNC(PFNGLUSEPROGRAMOBJECTARBPROC,    glUseProgramObjectARB);
    INIT_GL_FUNC(PFNGLGETUNIFORMLOCATIONARBPROC,  glGetUniformLocationARB);
    INIT_GL_FUNC(PFNGLUNIFORM1IARBPROC,           glUniform1iARB);
    INIT_GL_FUNC(PFNGLUNIFORM1FARBPROC,           glUniform1fARB);
    INIT_GL_FUNC(PFNGLUNIFORM2FARBPROC,           glUniform2fARB);

    initialized = true;
}

DepthOfField::~DepthOfField()
{}

void DepthOfField::Init()
{
    if (!initialized)
	return;

    int width = 1, height = 1;

    while (width < display.GetWidth())
	width *= 2;
    while (height < display.GetHeight())
	height *= 2;

    glDisable(GL_TEXTURE_2D);
    glGenTextures(2, textures);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    const GLhandleARB fragmentShader =
	    glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
    glShaderSourceARB(fragmentShader, 1, &fragmentShaderSource, 0);
    glCompileShaderARB(fragmentShader);
    program = glCreateProgramObjectARB();
    glAttachObjectARB(program, fragmentShader);
    glLinkProgramARB(program);

    const GLhandleARB blurVertexShader =
	    glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
    glShaderSourceARB(blurVertexShader, 1, &blurVertexShaderSource, 0);
    glCompileShaderARB(blurVertexShader);
    const GLhandleARB blurFragmentShader =
	    glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
    glShaderSourceARB(blurFragmentShader, 1, &blurFragmentShaderSource, 0);
    glCompileShaderARB(blurFragmentShader);
    blurProgram = glCreateProgramObjectARB();
    glAttachObjectARB(blurProgram, blurVertexShader);
    glAttachObjectARB(blurProgram, blurFragmentShader);
    glLinkProgramARB(blurProgram);

    const GLhandleARB dofVertexShader =
	    glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
    glShaderSourceARB(dofVertexShader, 1, &dofVertexShaderSource, 0);
    glCompileShaderARB(dofVertexShader);
    const GLhandleARB dofFragmentShader =
	    glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
    glShaderSourceARB(dofFragmentShader, 1, &dofFragmentShaderSource, 0);
    glCompileShaderARB(dofFragmentShader);
    dofProgram = glCreateProgramObjectARB();
    glAttachObjectARB(dofProgram, dofVertexShader);
    glAttachObjectARB(dofProgram, dofFragmentShader);
    glLinkProgramARB(dofProgram);

    glUseProgramObjectARB(program);

    glUniform1fARB(glGetUniformLocationARB(program, "blurNear"), blurNear);
    glUniform1fARB(glGetUniformLocationARB(program, "focalNear"), focalNear);
    glUniform1fARB(glGetUniformLocationARB(program, "focalFar"), focalFar);
    glUniform1fARB(glGetUniformLocationARB(program, "blurFar"), blurFar);
}

void DepthOfField::Free()
{
    if (!initialized)
	return;

    glDeleteTextures(2, textures);
}

void DepthOfField::Apply()
{
    if (!initialized)
	return;

    int width = 1, height = 1;

    while (width < display.GetWidth())
	width *= 2;
    while (height < display.GetHeight())
	height *= 2;

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glEnable(GL_TEXTURE_2D);

    glUseProgramObjectARB(0);

    glPushMatrix();
    glLoadIdentity();
    //glTranslatef(2.f / static_cast<float>(display.GetWidth()), 2.f / static_cast<float>(display.GetHeight()), 0.f);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_LIGHTING);

    const float pixel_x = 0;//2.f / static_cast<float>(display.GetWidth());
    const float pixel_y = 0;//2.f / static_cast<float>(display.GetHeight());
    const float right = static_cast<float>(display.GetWidth())
		      / static_cast<float>(width);
    const float top = static_cast<float>(display.GetHeight())
		    / static_cast<float>(height);

    // Save rendered framre
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, width, height, 0);

    // Create 2x downscaled version
    glBegin(GL_QUADS);
    glNormal3f(0.f, 0.f, 1.f);
    glTexCoord2f(0.f, 0.f);   glVertex3f(-1.f - pixel_x, -1.f - pixel_y, 0.f);
    glTexCoord2f(right, 0.f); glVertex3f( 0.f + pixel_x, -1.f - pixel_y, 0.f);
    glTexCoord2f(right, top); glVertex3f( 0.f + pixel_x,  0.f + pixel_y, 0.f);
    glTexCoord2f(0.f, top);   glVertex3f(-1.f - pixel_x,  0.f + pixel_y, 0.f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glEnable(GL_TEXTURE_2D);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		     0, 0, width / 2, height / 2, 0);

    // Create 4x downscaled version
    glBegin(GL_QUADS);
    glNormal3f(0.f, 0.f, 1.f);
    glTexCoord2f(0.f, 0.f);   glVertex3f(-1.f - pixel_x, -1.f - pixel_y, 0.f);
    glTexCoord2f(right, 0.f); glVertex3f(-.5f + pixel_x, -1.f - pixel_y, 0.f);
    glTexCoord2f(right, top); glVertex3f(-.5f + pixel_x, -.5f + pixel_y, 0.f);
    glTexCoord2f(0.f, top);   glVertex3f(-1.f - pixel_x, -.5f + pixel_y, 0.f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glEnable(GL_TEXTURE_2D);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		     0, 0, width / 4, height / 4, 0);

    // Blur (gaussian 3x3) the downscaled version
    glUseProgramObjectARB(blurProgram);
    glUniform2fARB(glGetUniformLocationARB(blurProgram, "texSize"),
		   width/4, height/4);
    glBegin(GL_QUADS);
    glNormal3f(0.f, 0.f, 1.f);
    glTexCoord2f(0.f, 0.f);   glVertex3f(-1.f - pixel_x, -1.f - pixel_y, 0.f);
    glTexCoord2f(right, 0.f); glVertex3f(-.5f + pixel_x, -1.f - pixel_y, 0.f);
    glTexCoord2f(right, top); glVertex3f(-.5f + pixel_x, -.5f + pixel_y, 0.f);
    glTexCoord2f(0.f, top);   glVertex3f(-1.f - pixel_x, -.5f + pixel_y, 0.f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glEnable(GL_TEXTURE_2D);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, width/4, height/4, 0);

    // Merge it with the original frame using DOF filter
    glUseProgramObjectARB(dofProgram);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glEnable(GL_TEXTURE_2D);
    glUniform1iARB(glGetUniformLocationARB(dofProgram, "frame"), 0);
    glActiveTextureARB(GL_TEXTURE1_ARB);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glEnable(GL_TEXTURE_2D);
    glUniform1iARB(glGetUniformLocationARB(dofProgram, "blur"), 1);

    glBegin(GL_QUADS);
    glNormal3f(0.f, 0.f, 1.f);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0, 0.0);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0, 0.0);
    glVertex3f(-1.f, -1.f, 0.f);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, right, 0.f);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB, right, 0.f);
    glVertex3f( 1.f, -1.f, 0.f);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, right, top);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB, right, top);
    glVertex3f( 1.f,  1.f, 0.f);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.f, top);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.f, top);
    glVertex3f(-1.f,  1.f, 0.f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glActiveTextureARB(GL_TEXTURE1_ARB);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glUseProgramObjectARB(0);
    glUseProgramObjectARB(program);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_DEPTH_TEST);
}

} // namespace Podz

// End of File
