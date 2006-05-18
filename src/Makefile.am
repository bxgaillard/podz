# ----------------------------------------------------------------------------
#
# Podz: A Pod Racing Game
# Copyright (C) 2006 Benjamin Gaillard & Nicolas Riegel
#
# ----------------------------------------------------------------------------
#
#        File: src/Makefile.am
# Description: Sources Automake Makefile
#
# ----------------------------------------------------------------------------
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the Free
# Software Foundation; either version 2 of the License, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc., 59
# Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#
# ----------------------------------------------------------------------------


# Flags
AM_CPPFLAGS = -DDATA_DIR="\"$(pkgdatadir)-$(PACKAGE_VERSION)\""

# Programs to compile
bin_PROGRAMS = podz

# Sources
podz_SOURCES = \
    Application.cpp \
    Application.h \
    Basis.cpp \
    Basis.h \
    Circuit.cpp \
    Circuit.h \
    Cube.cpp \
    Cube.h \
    Display.cpp \
    Display.h \
    Keyboard.cpp \
    Keyboard.h \
    Object.cpp \
    Object.h \
    Texture.cpp \
    Texture.h \
    Timer.cpp \
    Timer.h \
    Vector.cpp \
    Vector.h \
    Vehicle.cpp \
    Vehicle.h

# Libraries
podz_LDADD = -lm

# End of File
