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
#ifndef VECTOR3_H_
#define VECTOR3_H_
namespace MINX
{
	/** A Vector that holds 2 components, X and Y.
	 */
	struct Vector3
	{
		/** Initializes the Vector3 with an x and a y component.
		 * @param x The x component of the Vector3.
		 * @param y The y component of the Vector3.
		 */
			Vector3(float x, float y, float z);
		/** Adds 2 Vector3s.
		 * @param addTo the Vector3 to add.
		 */
			Vector3 operator+(const Vector3& addTo);
		/** Subtracts a Vector3 from another.
		 * @param subtractFrom the Vector3 to subtract.
		 */
			Vector3 operator-(const Vector3& subtractFrom);
		/** Cross multiplies 2 Vector3s.
		 * @param multiplyBy the Vector3 to cross multiply by.
		 */
			Vector3 crossMultiply(const Vector3& multiplyBy);
		/** Dot multiplies 2 Vector3s.
		 * @param multiplyBy the Vector3 to dot multiply by.
		 */
			float operator*(const Vector3& multiplyBy);
		/** Multiplies a Vector3 by a scalar.
		 * @param multiplyBy the scalar to multiply the length of the Vector3 by.
		 */
			Vector3 operator*(const float& multiplyBy);
		/** Negates a Vector3.
		 */
			Vector3 operator-()const;
		/** Tests equality between 2 Vector3s.
		 * @param compareTo the Vector3 to test equality with.
		 */
			bool operator==(const Vector3& compareTo);
		/** Tests inequality between 2 Vector3s.
		 * @param compareTo the Vector3 to test equality with.
		 */
			bool operator!=(const Vector3& compareTo);
		/** Returns the length of the Vector3.
		 */

			float length();
		/** Returns the length of the Vector3 squared.
		 * This is faster then squaring the result of length() because length() actually returns the square root of the mathematical result of the length squared.
		 */
			float lengthSquared();
		/** Returns a Vector3 of length 1 but the same direction as the current Vector3.
		 */
			Vector3 normalize();
		/** The X component of the Vector3.
		 */
		float X;
		/** The Y component of the Vector3.
		 */
		float Y;
		/** The Z component of the Vector3.
		 */
		float Z;
	};
}
#endif
