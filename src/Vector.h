/*
 * ---------------------------------------------------------------------------
 *
 * Podz: A Pod Racing Game
 * Copyright (C) 2006-2018 Benjamin Gaillard & Nicolas Riegel
 *
 * ---------------------------------------------------------------------------
 *
 *        File: src/Vector.h
 * Description: Vector Manipulation Functions (Header)
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


#ifndef PODZ_VECTOR_H
#define PODZ_VECTOR_H

namespace Podz {

class Vector
{
public:
    // Coordinates
    float x, y, z;

    // Constructor
    Vector(float vx = 0.f, float vy = 0.f, float vz = 0.f)
        : x(vx), y(vy), z(vz) {}

    // Assignment operator and copy constructor
    Vector &Set(const float vx = 0.f, const float vy = 0.f,
                const float vz = 0.f)
        { x = vx, y = vy, z = vz; return *this; }
    Vector &operator =(const Vector &v)
        { x = v.x, y = v.y, z = v.z; return *this; }
    Vector(const Vector &v) { (*this) = v; }

    // Geometric transformations
    Vector &Add(const float tx, const float ty, const float tz)
        { x += tx, y += ty, z += tz; return *this; }
    Vector &Sub(const float tx, const float ty, const float tz)
        { x -= tx, y -= ty, z -= tz; return *this; }
    Vector &Add(const Vector &t) { return Add(t.x, t.y, t.z); }
    Vector &Sub(const Vector &t) { return Sub(t.x, t.y, t.z); }
    Vector &Scale(const float s)
	{ if (s != 1.f) x *= s, y *= s, z *= s; return *this; }
    Vector &Scale(const Vector &s)
        { x *= s.x, y *= s.y, z *= s.z; return *this; }
    Vector &Rotate(const float rx, const float ry, const float rz);
    Vector &Rotate(const Vector &r) { return Rotate(r.x, r.y, r.z); }

    // Scalar product (new vector)
    Vector VectorProduct(const Vector &p) const;

    // Vector length
    float Length() const;

    // Miscellaneous operations
    Vector &Reverse() { x = -x, y = -y, z = -z; return *this; }
    Vector &Normalize(const float n = 1.f) { return Scale(n / Length()); }

    // Addition/substraction operators
    Vector &operator +=(const Vector &t) { return Add(t); }
    Vector &operator -=(const Vector &t) { return Sub(t); }
    Vector operator +(const Vector &t) const { return Vector(*this) += t; }
    Vector operator -(const Vector &t) const { return Vector(*this) -= t; }

    // Scaling operators
    Vector &operator *=(const float s) { return Scale(s); }
    Vector &operator /=(const float s) { return Scale(1.f / s); }
    Vector operator *(const float s) const { return Vector(*this) *= s; }
    Vector operator /(const float s) const { return Vector(*this) /= s; }

    // Scalar product operators
    Vector operator *(const Vector &p) const { return VectorProduct(p); }
    Vector &operator *=(const Vector &p) { return (*this) *= p; }

    // Miscellaneous operators
    Vector operator -() const { return Vector(*this).Reverse(); }
    Vector &operator %=(const float n) { return Normalize(n); }
    Vector operator %(const float n) const { return Vector(*this) %= n; }

    // Incrementation and decrementation
    Vector &operator +=(const float t) { return (*this) += (*this) % t; }
    Vector &operator -=(const float t) { return (*this) -= (*this) % t; }
    Vector &operator ++() { return (*this) += 1.f; }
    Vector &operator --() { return (*this) -= 1.f; }
    Vector &operator ++(int) { return ++(*this); }
    Vector &operator --(int) { return --(*this); }

    // Comparison operators
    bool operator ==(const Vector &v) const
        { return x == v.x && y == v.y && z == v.z; }
    bool operator !=(const Vector &v) const { return !(*this == v); }
    bool operator <(const Vector &v) const { return Length() < v.Length(); }
    bool operator >(const Vector &v) const { return Length() > v.Length(); }
    bool operator <=(const Vector &v) const { return Length() <= v.Length(); }
    bool operator >=(const Vector &v) const { return Length() >= v.Length(); }
};

} // namespace Podz

#endif // !PODZ_VECTOR_H

// End of File
