/*
    MINX - A C++ Graphics and Input Wrapper Library
    Copyright (C) 2013-2014  MINX Team

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

	*/
#ifndef I_GENERIC_HID_H_
#define I_GENERIC_HID_H_

#include "../GameTime.h"
#include "../GameComponent.h"
#include <GL/glfw3.h>
#include "Button.h"
#include "Axis.h"
#include "../Game.h"
#include <vector>

namespace MINX
{
	namespace Input
	{
		/** Represents any Human Interface Device that has axes and/or buttons.
		 * Extended by Keyboard, Mouse, and GamePad classes.
		 * This class should almost NEVER be used directly, but rather a subclass should be used.
		 */
		class IGenericHID : public GameComponent
		{
			public:
				/** Constructs an IGenericHID. Fills the button and axis vectors with zeroed values.
				 * @param game A pointer to the Game to attach to.
				 * @param buttonVectorSize The total number of buttons that the device has.
				 * @param axisVectorSize The total number of axes that the device has.
				 */
				IGenericHID(Game * game, unsigned int buttonVectorSize, unsigned int axisVectorSize);
				
				/** Contains all of the buttons used by the device.
				 */
				vector<Button> * buttons;
				
				/** Contains all of the axes used by the device.
				 */
				vector<Axis> * axes;
				
				/** Returns a Button from the specified index in the button vector.
				 */
				Button GetButton(unsigned int id);
				
				/** Returns an Axis from the specified index in the axis vector.
				 */
				Axis GetAxis(unsigned int id);
		};
	}
}
#endif
