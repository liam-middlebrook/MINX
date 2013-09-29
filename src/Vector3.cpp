/*
    MINX - A C++ Graphics and Input Wrapper Library
    Copyright (C) 2013  MINX Team

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

	*/
#include "Vector3.h"
#include <math.h>
#include <iostream>

using namespace MINX;

Vector3::Vector3(float x, float y, float z) : X( x ), Y( y ), Z( z ) { }

Vector3 Vector3::operator+(const Vector3& addTo)
{
	return Vector3(X+addTo.X,Y+addTo.Y, Z+addTo.Z);
}
Vector3 Vector3::operator-(const Vector3& subtractFrom)
{
	return Vector3(X-subtractFrom.X,Y-subtractFrom.Y, Z-subtractFrom.Z);
}
Vector3 Vector3::crossMultiply(const Vector3& mult)
{
	/*|  i  j  k |
	  | x1 y1 z1 | = i(y1*z2 - z1*y2) - j(x1*z2 - z1*x2) + k(x1*y2 - y1*x2)
	  | x2 y2 z2 |
	*/
	return Vector3(Y*mult.Z - Z*mult.Y, -(X*mult.Z - Z*mult.X),X*mult.Y - y*mult.X);
}
float Vector3::operator*(const Vector3& mult)
{
	return X*mult.X + Y*mult.Y + Z*mult.Z;
}
Vector3 Vector3::operator*(const float& mult)
{
	return Vector3(X*mult,Y*mult,Z*mult);
}
Vector3 Vector3::operator-()const
{
	return Vector3(-X,-Y,-Z);
}
bool Vector3::operator==(const Vector3& compareTo)
{
	return compareTo.X == X && compareTo.Y == Y, && compareTo.Z == Z;
}
bool Vector3::operator!=(const Vector3& compareTo)
{
	return !operator==(compareTo);
}
float Vector3::lengthSquared()
{
	return X*X+Y*Y+Z*Z;
}
float Vector3::length()
{
	return sqrt(lengthSquared());
}
Vector3 Vector3::normalize()
{
	if(lengthSquared() != 0)
	{
		return Vector3(X/length(),Y/length(), Z/length());
	}
	return Vector3(0,0,0);
}
