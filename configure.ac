dnl --------------------------------------------------------------------------
dnl
dnl Podz: A Pod Racing Game
dnl Copyright (C) 2006 Benjamin Gaillard & Nicolas Riegel
dnl
dnl --------------------------------------------------------------------------
dnl
dnl        File: configure.ac
dnl Description: Autoconf Script
dnl
dnl --------------------------------------------------------------------------
dnl
dnl This program is free software; you can redistribute it and/or modify it
dnl under the terms of the GNU General Public License as published by the Free
dnl Software Foundation; either version 2 of the License, or (at your option)
dnl any later version.
dnl
dnl This program is distributed in the hope that it will be useful, but
dnl WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
dnl or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
dnl for more details.
dnl
dnl You should have received a copy of the GNU General Public License along
dnl with this program; if not, write to the Free Software Foundation, Inc., 59
dnl Temple Place - Suite 330, Boston, MA 02111-1307, USA.
dnl
dnl --------------------------------------------------------------------------


dnl Process this file with autoconf to produce a configure script.

dnl Autotools initialization
AC_PREREQ(2.52g)
AC_INIT([Podz], [1.1.0], [b.gaillard@powercode.net], [podz])
AC_CONFIG_SRCDIR([src/Application.cpp])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_AUX_DIR([autotools])
AC_CONFIG_MACRO_DIR([autotools])
AM_INIT_AUTOMAKE([1.7 no-define dist-bzip2 -Wall])

dnl Checks for programs
AC_LANG([C++])
AC_PROG_CXX

dnl Checks for headers
AC_CHECK_HEADER([GL/gl.h],   [
    AC_CHECK_HEADER([GL/glu.h],  [], [AC_MSG_ERROR([GL/glu.h required])] )
    AC_CHECK_HEADER([GL/glut.h], [], [AC_MSG_ERROR([GL/glut.h required])])

    dnl Checks for libraries
    AC_CHECK_LIB([GL], [glBegin], [LIBS="-lGL $LIBS"],
                 [AC_MSG_ERROR([libGL required])])
    AC_CHECK_LIB([GLU], [gluPerspective], [LIBS="-lGLU $LIBS"],
                 [AC_MSG_ERROR([libGLU required])])
    AC_CHECK_LIB([glut], [glutInit], [LIBS="-lglut $LIBS"],
                 [AC_MSG_ERROR([libglut required])])
], [
    dnl If OpenGL/gl.h exists, assume we are under Darwin/Mac OS X
    AC_CHECK_HEADER([OpenGL/gl.h],
    [LIBS="-framework OpenGL -framework GLUT -framework Foundation $LIBS"],
                    [AC_MSG_ERROR([GL/gl.h or OpenGL/gl.h required])])
])

dnl Enable G++ warnings
if test "x$GXX" = xyes; then
    CXXFLAGS="-std=c++98 -pedantic -Wall -W $CXXFLAGS"
    CXXFLAGS="-ffast-math -fno-exceptions -fno-rtti $CXXFLAGS"
fi

dnl Generated files
AC_CONFIG_FILES([
    Makefile
    src/Makefile
    data/Makefile
    doc/Makefile
])
AC_OUTPUT

dnl End of File
